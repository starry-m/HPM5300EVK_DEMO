#include "drv_gc9a01.h"
#include <rtthread.h>
#include <rtdevice.h>
#include "rtt_board.h"
#include "stdlib.h"
#define DBG_TAG "LCD"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
/**
 * CS:PA25
 * SCK:A27
 * MOSI:A29
 * DC:PA30
 *
 * * CS:PB04
 * SCK:PB05
 * MOSI:PB07
 * DC:PB10
 * **/
#if (CONFIG_GC9A01_RESET_USED)
#define RESET_HIGH()           gpio_set_level(CONFIG_GC9A01_PIN_NUM_RST,1)
#define RESET_LOW()            gpio_set_level(CONFIG_GC9A01_PIN_NUM_RST,0)
#endif

#if(CONFIG_GC9A01_CONTROL_BACK_LIGHT_USED)
#define BLK_HIGH()             gpio_set_level(CONFIG_GC9A01_PIN_NUM_BCKL,1)
#define BLK_LOW()              gpio_set_level(CONFIG_GC9A01_PIN_NUM_BCKL,0)
#endif
#define SPI_MAX_DMA_LEN 1024
static struct rt_spi_device *lcd_device;
#define SPI_LCD_DEVICE_NAME "spi3-lcd"
#define LCD_DC_PIN rt_pin_get("PB10")
static void spi_write_byte(uint8_t data) { rt_spi_send(lcd_device, &data, 1); }
//https://github.com/liyanboy74/gc9a01-esp-idf
void GC9A01_Init();
static int LCD_Device_Init(void)
{
    lcd_device = RT_NULL;

    lcd_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device));
    if(RT_NULL == lcd_device)
    {
        LOG_E("Failed to malloc the spi device.");
        return -RT_ENOMEM;
    }
    if (RT_EOK != rt_spi_bus_attach_device_cspin(lcd_device, SPI_LCD_DEVICE_NAME, "spi3",rt_pin_get("PB04"), RT_NULL))
    {
        LOG_E("Failed to attach the spi device.");
        return -RT_ERROR;
    }

    if (RT_NULL == rt_device_find(SPI_LCD_DEVICE_NAME))
    {
        LOG_E("Failed to probe the lcd.");
        return -RT_ERROR;
    }
    //配置spi参数
    struct rt_spi_configuration cfg;
    {
        cfg.data_width = 8;//数据宽度为八位
        cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
        cfg.max_hz = 80 * 1000 * 1000;//
    }
    rt_spi_configure(lcd_device, &cfg);
    //前面的频率配置没用，锁在了80M。
    rt_pin_mode(LCD_DC_PIN, PIN_MODE_OUTPUT);
    GC9A01_Init();
    GC9A01_FillRect(0,0,239,239,0XF800);
    return RT_EOK;
}
/* 导出到自动初始化 */
INIT_DEVICE_EXPORT(LCD_Device_Init);
static int LCD_refresh_test(void)
{
    uint8_t tick=10;
    uint16_t color_tab[]={0XF800,0x07E0,0x001F,0xFFFF,0x0000};
    uint8_t index=0;
    while(tick--)
    {
//        GC9A01_FillRect(0,0,239,239,color_tab[(index++)%4]);
        GC9A01_Clear(color_tab[4- (index++)%5]);
        rt_thread_mdelay(200);
    }

}
//INIT_APP_EXPORT(LCD_refresh_test);

uint8_t GC9A01_X_Start = 0, GC9A01_Y_Start = 0;
/*
 The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct.
*/
typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

static const lcd_init_cmd_t lcd_init_cmds[]={
    {0xef,{0},0},
    {0xeb,{0x14},1},
    {0xfe,{0},0},
    {0xef,{0},0},
    {0xeb,{0x14},1},
    {0x84,{0x40},1},
    {0x85,{0xff},1},
    {0x86,{0xff},1},
    {0x87,{0xff},1},
    {0x88,{0x0a},1},
    {0x89,{0x21},1},
    {0x8a,{0x00},1},
    {0x8b,{0x80},1},
    {0x8c,{0x01},1},
    {0x8d,{0x01},1},
    {0x8e,{0xff},1},
    {0x8f,{0xff},1},
    {Cmd_DisplayFunctionControl,{0x00,0x20},2},// Scan direction S360 -> S1
    //{Cmd_MADCTL,{0x08},1},//MemAccessModeSet(0, 0, 0, 1);
    //{Cmd_COLMOD,{ColorMode_MCU_16bit&0x77},1},
    {0x90,{0x08,0x08,0x08,0x08},4},
    {0xbd,{0x06},1},
    {0xbc,{0x00},1},
    {0xff,{0x60,0x01,0x04},3},
    {Cmd_PWCTR2,{0x13},1},
    {Cmd_PWCTR3,{0x13},1},
    {Cmd_PWCTR4,{0x22},1},
    {0xbe,{0x11},1},
    {0xe1,{0x10,0x0e},2},
    {0xdf,{0x21,0x0c,0x02},3},
    {Cmd_GAMMA1,{0x45,0x09,0x08,0x08,0x26,0x2a},6},
    {Cmd_GAMMA2,{0x43,0x70,0x72,0x36,0x37,0x6f},6},
    {Cmd_GAMMA3,{0x45,0x09,0x08,0x08,0x26,0x2a},6},
    {Cmd_GAMMA4,{0x43,0x70,0x72,0x36,0x37,0x6f},6},
    {0xed,{0x1b,0x0b},2},
    {0xae,{0x77},1},
    {0xcd,{0x63},1},
    {0x70,{0x07,0x07,0x04,0x0e,0x0f,0x09,0x07,0x08,0x03},9},
    {Cmd_FRAMERATE,{0x34},1},// 4 dot inversion
    {0x62,{0x18,0x0D,0x71,0xED,0x70,0x70,0x18,0x0F,0x71,0xEF,0x70,0x70},12},
    {0x63,{0x18,0x11,0x71,0xF1,0x70,0x70,0x18,0x13,0x71,0xF3,0x70,0x70},12},
    {0x64,{0x28,0x29,0xF1,0x01,0xF1,0x00,0x07},7},
    {0x66,{0x3C,0x00,0xCD,0x67,0x45,0x45,0x10,0x00,0x00,0x00},10},
    {0x67,{0x00,0x3C,0x00,0x00,0x00,0x01,0x54,0x10,0x32,0x98},10},
    {0x74,{0x10,0x85,0x80,0x00,0x00,0x4E,0x00},7},
    {0x98,{0x3e,0x07},2},
    {Cmd_TEON,{0},0},// Tearing effect line on
    {0, {0}, 0xff},//END
};





/* Send a command to the LCD. Uses spi_device_polling_transmit, which waits
 * until the transfer is complete.
 *
 * Since command transactions are usually small, they are handled in polling
 * mode for higher speed. The overhead of interrupt transactions is more than
 * just waiting for the transaction to complete.
 */
void lcd_cmd(uint8_t cmd)
{
    uint8_t data;
    data = cmd; // command mode 
    //D/C needs to be set to 0
    rt_pin_write(LCD_DC_PIN,0);
    rt_spi_send(lcd_device, &data, 1);
}

/* Send data to the LCD. Uses spi_device_polling_transmit, which waits until the
 * transfer is complete.
 *
 * Since data transactions are usually small, they are handled in polling
 * mode for higher speed. The overhead of interrupt transactions is more than
 * just waiting for the transaction to complete.
 */
void lcd_data(const uint8_t *data, int len)
{
    if (len==0) return;             //no need to send anything
    rt_uint16_t tx_len=0;
    rt_uint8_t *tx_buffer;
    //D/C needs to be set to 1
    rt_pin_write(LCD_DC_PIN,1);
    /*
    On certain MC's the max SPI DMA transfer length might be smaller than the buffer. We then have to split the transmissions.
    */
    rt_uint16_t offset = 0;
    do {
        tx_len = ((len - offset) < SPI_MAX_DMA_LEN) ? (len - offset) : SPI_MAX_DMA_LEN;
        tx_buffer= data + offset;                //Data
        rt_spi_send(lcd_device, tx_buffer, tx_len);
        offset += tx_len;                          // Add the transmission length to the offset
    }
    while (offset < len);
}

void lcd_send_byte(uint8_t Data)
{
    lcd_data(&Data,1);
}

static void delay_ms (uint32_t Delay_ms)
{
    rt_thread_mdelay(Delay_ms);
}

uint16_t GC9A01_GetWidth() {
    return GC9A01_Width;
}

uint16_t GC9A01_GetHeight() {
    return GC9A01_Height;
}

void GC9A01_HardReset(void) {
    #if (CONFIG_GC9A01_RESET_USED)
    RESET_LOW();
    delay_ms(10);
    RESET_HIGH();
    delay_ms(150);
    #endif
}

void GC9A01_SleepMode(uint8_t Mode) {
    if (Mode)
        lcd_cmd(Cmd_SLPIN);
    else
        lcd_cmd(Cmd_SLPOUT);

    delay_ms(500);
}

void GC9A01_InversionMode(uint8_t Mode) {
    if (Mode)
        lcd_cmd(Cmd_INVON);
    else
        lcd_cmd(Cmd_INVOFF);
}

void GC9A01_DisplayPower(uint8_t On) {
    if (On)
        lcd_cmd(Cmd_DISPON);
    else
        lcd_cmd(Cmd_DISPOFF);
}

static void ColumnSet(uint16_t ColumnStart, uint16_t ColumnEnd) {
    if (ColumnStart > ColumnEnd)
        return;
    if (ColumnEnd > GC9A01_Width)
        return;

    ColumnStart += GC9A01_X_Start;
    ColumnEnd += GC9A01_X_Start;

    lcd_cmd(Cmd_CASET);
    lcd_send_byte(ColumnStart >> 8);
    lcd_send_byte(ColumnStart & 0xFF);
    lcd_send_byte(ColumnEnd >> 8);
    lcd_send_byte(ColumnEnd & 0xFF);
}

static void RowSet(uint16_t RowStart, uint16_t RowEnd) {
    if (RowStart > RowEnd)
        return;
    if (RowEnd > GC9A01_Height)
        return;

    RowStart += GC9A01_Y_Start;
    RowEnd += GC9A01_Y_Start;

    lcd_cmd(Cmd_RASET);
    lcd_send_byte(RowStart >> 8);
    lcd_send_byte(RowStart & 0xFF);
    lcd_send_byte(RowEnd >> 8);
    lcd_send_byte(RowEnd & 0xFF);
}

void GC9A01_SetWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    ColumnSet(x0, x1);
    RowSet(y0, y1);

    lcd_cmd(Cmd_RAMWR);
}

static void ColorModeSet(uint8_t ColorMode) {
    lcd_cmd(Cmd_COLMOD);
    lcd_send_byte(ColorMode & 0x77);
}

static void MemAccessModeSet(uint8_t Rotation, uint8_t VertMirror,
        uint8_t HorizMirror, uint8_t IsBGR) {
    uint8_t Ret=0;
    Rotation &= 7;

    lcd_cmd(Cmd_MADCTL);

    switch (Rotation) {
    case 0:
        Ret = 0;
        break;
    case 1:
        Ret = MADCTL_MX;
        break;
    case 2:
        Ret = MADCTL_MY;
        break;
    case 3:
        Ret = MADCTL_MX | MADCTL_MY;
        break;
    case 4:
        Ret = MADCTL_MV;
        break;
    case 5:
        Ret = MADCTL_MV | MADCTL_MX;
        break;
    case 6:
        Ret = MADCTL_MV | MADCTL_MY;
        break;
    case 7:
        Ret = MADCTL_MV | MADCTL_MX | MADCTL_MY;
        break;
    }

    if (VertMirror)
        Ret = MADCTL_ML;
    if (HorizMirror)
        Ret = MADCTL_MH;

    if (IsBGR)
        Ret |= MADCTL_BGR;

    lcd_send_byte(Ret);
}

void GC9A01_SetBL(uint8_t Value)
{
    if (Value > 100) Value = 100;
    #if(CONFIG_GC9A01_CONTROL_BACK_LIGHT_USED)
        #if(CONFIG_GC9A01_CONTROL_BACK_LIGHT_MODE)
        LEDC_PWM_Duty_Set(Value);
        #else
        if (Value) BLK_HIGH();
        else BLK_LOW();
        #endif
    #endif
}

//Direct Mode
#if (!CONFIG_GC9A01_BUFFER_MODE)

void GC9A01_RamWrite(uint16_t *pBuff, uint16_t Len)
{
    while (Len--)
    {
        lcd_send_byte(*pBuff >> 8);
        lcd_send_byte(*pBuff & 0xFF);
    }
}

void GC9A01_DrawPixel(int16_t x, int16_t y, uint16_t color)
{
if ((x < 0) ||(x >= GC9A01_Width) || (y < 0) || (y >= GC9A01_Height))
    return;

    GC9A01_SetWindow(x, y, x, y);
    GC9A01_RamWrite(&color, 1);
}


void GC9A01_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    if ((x >= GC9A01_Width) || (y >= GC9A01_Height))
        return;

    if ((x + w) > GC9A01_Width)
        w = GC9A01_Width - x;

    if ((y + h) > GC9A01_Height)
        h = GC9A01_Height - y;

    GC9A01_SetWindow(x, y, x + w - 1, y + h - 1);
    uint8_t color_buff[2]={color>>8,color&0xff};
    rt_pin_write(LCD_DC_PIN,1);
    for (uint32_t i = 0; i < (h * w); i++)
    {
        rt_spi_send(lcd_device, color_buff, 2);
    }
//        GC9A01_RamWrite(&color, 1);

}
void GC9A01_ColorFill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{

}
//uint16_t color_buffer[2400];
void GC9A01_Clear(uint16_t color)
{
    GC9A01_SetWindow(0, 0, 239,239);
    //配置spi参数
//   struct rt_spi_configuration cfg;
//
//   cfg.data_width = 16;//数据宽度为16位
//   cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
//   cfg.max_hz = 80 * 1000 * 1000;//
//
//   rt_spi_configure(lcd_device, &cfg);
//    memset(color_buffer,color,sizeof(color));
    rt_pin_write(LCD_DC_PIN,1);
    uint8_t color_buff[2]={color>>8,color&0xff};
    for(uint32_t i=0;i<240*240;i++)
        rt_spi_send(lcd_device, color_buff, 2);

//    cfg.data_width = 8;
//    rt_spi_configure(lcd_device, &cfg);
}
//Buffer mode
#else

    static void SwapBytes(uint16_t *color) {
        uint8_t temp = *color >> 8;
        *color = (*color << 8) | temp;
    }

    void GC9A01_DrawPixel(int16_t x, int16_t y, uint16_t color) {
        if ((x < 0) || (x >= GC9A01_Width) || (y < 0) || (y >= GC9A01_Height))
            return;

        SwapBytes(&color);

        ScreenBuff[y * GC9A01_Width + x] = color;
    }

    uint16_t GC9A01_GetPixel(int16_t x, int16_t y) {
        if ((x < 0) || (x >= GC9A01_Width) || (y < 0) || (y >= GC9A01_Height))
            return 0;

        uint16_t color = ScreenBuff[y * GC9A01_Width + x];
        SwapBytes(&color);
        return color;
    }

    void GC9A01_FillRect(int16_t x, int16_t y, int16_t w, int16_t h,
            uint16_t color) {
        if ((w <= 0) || (h <= 0) || (x >= GC9A01_Width) || (y >= GC9A01_Height))
            return;

        if (x < 0) {
            w += x;
            x = 0;
        }
        if (y < 0) {
            h += y;
            y = 0;
        }

        if ((w <= 0) || (h <= 0))
            return;

        if ((x + w) > GC9A01_Width)
            w = GC9A01_Width - x;
        if ((y + h) > GC9A01_Height)
            h = GC9A01_Height - y;

        SwapBytes(&color);

        for (uint16_t row = 0; row < h; row++) {
            for (uint16_t col = 0; col < w; col++)
                //GC9A01_DrawPixel(col, row, color);
                ScreenBuff[(y + row) * GC9A01_Width + x + col] = color;
        }
    }

    void GC9A01_Update()
    {
        int len = GC9A01_Width * GC9A01_Height;
        GC9A01_SetWindow(0, 0, GC9A01_Width - 1, GC9A01_Height - 1);
        lcd_data((uint8_t*) &ScreenBuff[0], len*2);
    }

    void GC9A01_Clear(void)
    {
        GC9A01_FillRect(0, 0, GC9A01_Width, GC9A01_Height, 0x0000);
    }

#endif


#if(CONFIG_GC9A01_CONTROL_BACK_LIGHT_USED)
#if(CONFIG_GC9A01_CONTROL_BACK_LIGHT_MODE)
void LEDC_PWM_Duty_Set(uint8_t DutyP)
{
    uint16_t Duty,MaxD;

    MaxD=(1<<(int)ledc_tConfig.duty_resolution)-1;

    if(DutyP>=100)Duty=MaxD;
    else
    {
        Duty=DutyP*(MaxD/(float)100);
    }
    ledc_cConfig.duty=Duty;
    ledc_channel_config(&ledc_cConfig);
}

void LEDC_Channel_Config(void)
{
    ledc_cConfig.gpio_num=CONFIG_GC9A01_PIN_NUM_BCKL;
    ledc_cConfig.speed_mode=LEDC_LOW_SPEED_MODE;
    ledc_cConfig.channel=LEDC_CHANNEL_0;
    ledc_cConfig.intr_type=LEDC_INTR_DISABLE;
    ledc_cConfig.timer_sel=LEDC_TIMER_0;
    ledc_cConfig.duty=0;
    ledc_cConfig.hpoint=0;
    ledc_channel_config(&ledc_cConfig);
}

void LEDC_Timer_Config(void)
{
    ledc_tConfig.speed_mode=LEDC_LOW_SPEED_MODE ;
    ledc_tConfig.duty_resolution=LEDC_TIMER_8_BIT;
    //ledc_tConfig.bit_num=LEDC_TIMER_8_BIT;
    ledc_tConfig.timer_num=LEDC_TIMER_0;
    ledc_tConfig.freq_hz=1000;
    ledc_tConfig.clk_cfg=LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_tConfig);
}
#endif
#endif

void GC9A01_Init()
{
    int cmd=0;

    GC9A01_X_Start = 0;
    GC9A01_Y_Start = 0;

    #if (CONFIG_GC9A01_BUFFER_MODE_PSRAM)
    if (ScreenBuff == NULL) {
        // ScreenBuffer has not yet been allocated
        ScreenBuff = heap_caps_malloc((GC9A01_Height * GC9A01_Width) * 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT );
    }
    #endif



    #if(CONFIG_GC9A01_CONTROL_BACK_LIGHT_USED)
    #if(CONFIG_GC9A01_CONTROL_BACK_LIGHT_MODE)
    LEDC_Timer_Config();
    LEDC_Channel_Config();
    #endif
    #endif

    #if(CONFIG_GC9A01_RESET_USED)
    GC9A01_HardReset();
    #endif

    //Send all the commands
    while (lcd_init_cmds[cmd].databytes!=0xff)
    {
        lcd_cmd(lcd_init_cmds[cmd].cmd);
        lcd_data(lcd_init_cmds[cmd].data, lcd_init_cmds[cmd].databytes&0x1F);
        if (lcd_init_cmds[cmd].databytes&0x80)
        {
            delay_ms(100);
        }
        cmd++;
    }

    MemAccessModeSet(3,0,0,1);
    ColorModeSet(ColorMode_MCU_16bit);

    GC9A01_InversionMode(1);
    GC9A01_SleepMode(0);

    delay_ms(120);
    GC9A01_DisplayPower(1);
    delay_ms(20);

    #if(CONFIG_GC9A01_BUFFER_MODE)
    GC9A01_Clear();
    GC9A01_Update();
    delay_ms(30);
    #endif

    #if(CONFIG_GC9A01_CONTROL_BACK_LIGHT_USED)
    GC9A01_SetBL(100);
    #endif
}


#if(CONFIG_GC9A01_BUFFER_MODE)

#if (CONFIG_GC9A01_BUFFER_MODE_PSRAM)
void GC9A01_Free(void) {
    if (ScreenBuff != NULL) {
        free(ScreenBuff);
    }
}
#endif


void GC9A01_Screen_Shot(uint16_t x,uint16_t y,uint16_t width ,uint16_t height,uint16_t * Buffer)
{
    uint16_t i,j;
    for (i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            #if(!CONFIG_GC9A01_BUFFER_SCREEN_FAST_MODE)
            Buffer[i*width+j]=GC9A01_GetPixel(x+j,y+i);
            #else
            Buffer[i*width+j]=ScreenBuff[((y+i) * GC9A01_Width )+ (x+j)];
            #endif
        }
    }
}
void GC9A01_Screen_Load(uint16_t x,uint16_t y,uint16_t width ,uint16_t height,uint16_t * Buffer)
{
    uint16_t i,j;
    for (i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            #if(!CONFIG_GC9A01_BUFFER_SCREEN_FAST_MODE)
            GC9A01_DrawPixel(x+j,y+i,Buffer[i*width+j]);
            #else
            ScreenBuff[((y+i) * GC9A01_Width )+ (x+j)] = Buffer[i*width+j];
            #endif
        }
    }
}

#endif
