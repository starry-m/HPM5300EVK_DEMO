# 简介

本示例演示了基于RT-Thread的 LED 灯闪烁例程。

# 开始

硬件连接
PWMA,26
PWMB,27
PWMC,14
ENA,12 不用

I2C SCL,5
I2C SDA,23

r,0.01
gain,50
IA,35
IB,34


## 记录

`#include "hpm_adc.h"`报错，
修改`libraries\hpm_sdk\components\SConscript   path = [ os.path.join(cwd, 'debug_console'), os.path.join(cwd, 'touch'), os.path.join(cwd, 'usb'), os.path.join(cwd, 'dma_mgr'), os.path.join(cwd, 'adc')]
`



# 一、项目概述
本项目为基于HPM5300 EVK开发板、无刷电机驱动板和2208无刷电机组成的FOC旋钮。使用FOC的高度可控性，模拟各种旋钮的转动效果。
## 目标功能
- 配合无刷电机驱动板，完成FOC闭环控制
- 圆形屏幕跑LVGL，做图形交互
- USB HID 接入，模拟实际设备
  
# 二、设计思路
- 初期使用现成的开发板、驱动板和屏幕做功能验证，三大部分分别调试完成，连接完成功能展示
- 做集成化设计，重新设计版图和结构，做成X-Knob这种形式的。USB 5V升压驱动电机，只需连接USB，不需要其他电源供电
  
## 三、实现过程

### 1、硬件介绍

- 硬件连接
  
|HPM5300 EVK 引脚|外接模块|说明|
|-|-|-|
|PA26|M0_IN1|FOC驱动板|
|PA27|NC|-|
|PA28|M0_IN2|-|
|PA29|NC|-|
|PA30|M0_IN3|-|
|PA31|NC|-|
|PB13|IA|-|
|PB14|IB|-|
|PB02|SCL|AS5600|
|PB03|SDA|-|
|PB05|SCK|GC9A01|
|PB07|MOSI|-|
|PB10|DC|-|

- 磁编码器部分
我所使用的无刷电机套装传感器使用的是磁编码器AS5600，并且给出的接口是I2C形式，因此需要使用开发板的I2C接口。  

- 驱动板部分
这里使用的是`灯哥开源`的V3P电机驱动板，这是一个两路无刷驱动板，本次只使用到了其中一路。
- 屏幕部分
这里使用的是1.28寸 GC9A01 240X240 SPI屏，CS已接下拉电阻，RST已接复位电路，背光无法控制，默认打开，因此这三个角不需要接。

### 2、软件实现

本工程使用的FOC算法部分，移植自[HPM5361MiniFOC](https://github.com/LX050724/HPM5361MiniFOC)
，在此感谢开源。本工程基于`RT Thread studio` IDE创建和调试。

- FOC算法适配
参考工程项目使用HPM的SEI接口连接传感器，获取角度数据，并依靠PWM连接触发，完成环路计算（在终中断中实现硬实时）。但我这套硬件只能用I2C读取，但是因为使用了RTT 标准I2C驱动，读写中有加锁，无法在中断中使用，会直接断言报错。且原工程环路计算频率极高（50K），在RTOS中直接使用不现实，一是I2C跟不上，二是影响RTOS运行。再者，本项目的目标是做旋钮效果模拟，因此选择降低环路计算频率。
下面主要讲下适配过程中的一些重要的地方。
`1:`编码器型号更换，原始角度从16位变为12位。
`2:`project_config中修改采样电阻和放大倍数。  
`3:`PID系数重新调整。  
`4:`环路计算改为定时100Hz运行一次。  
但是因为计算频率的降低，导致使用电流环做反馈时一直碰到卡顿和震荡现象。尝试连续几天的PID调参，一直无法解决，遂选择放弃电流环，选择最简单的办法，使用传感器的角度直接做位置环路控制，进行计算变换。
并且因为时间没安排好，以及工作太多，导致功能实现进度极慢，目前这部分还只完成了简单的多档旋钮模拟。
- 界面LVGL适配
屏幕的驱动参考自[此处仓库](https://github.com/liyanboy74/gc9a01-esp-idf)。  
``` c
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
```
随后是LVGL接口的适配，
- USB HID适配


