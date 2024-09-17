/*
 * Copyright (c) 2021 hpmicro
 *
 * Change Logs:
 * Date         Author          Notes
 * 2021-08-13   Fan YANG        first version
 *
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "rtt_board.h"

#include "current_calibration/current_calibration.h"
#include "electrical_angle_calibration/electrical_angle_calibration.h"
#include "board.h"
#include "foc/fast_sin.h"
#include "foc/foc_core.h"
#include "foc/foc_pid.h"
#include "foc/foc_pll.h"
#include "adc_init.h"
#include "current.h"
#include "encoder.h"

#include "pwm_init.h"
#include "trgm.h"

#include "project_config.h"
#include "stdbool.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "hpm_gptmr_drv.h"
#include "hpm_rtt_interrupt_util.h"
#define APP_BOARD_GPTMR               BOARD_GPTMR
#define APP_BOARD_GPTMR_CH            BOARD_GPTMR_CHANNEL
#define APP_BOARD_GPTMR_IRQ           BOARD_GPTMR_IRQ
#define APP_BOARD_GPTMR_CLOCK         BOARD_GPTMR_CLK_NAME

#define APP_TICK_MS                   (10)
__IO uint16_t read_raw_angel=0;
static void timer_config(void);
static void mt6701_isr_callback(uint32_t isr_flag);

typedef struct
{
    float data[15];
    uint8_t tail[4];
} just_float_data;

#define BUF_NUM (2048 * 2 / sizeof(just_float_data))
volatile int write_ptr;
DMA_ATTR just_float_data vofa_data[2];

void tick_ms_isr(void)
{
    if (gptmr_check_status(APP_BOARD_GPTMR, GPTMR_CH_RLD_STAT_MASK(APP_BOARD_GPTMR_CH))) {
        gptmr_clear_status(APP_BOARD_GPTMR, GPTMR_CH_RLD_STAT_MASK(APP_BOARD_GPTMR_CH));
//        board_led_toggle();
        //100 Hz
        mt6701_isr_callback(0);

    }
}
SDK_DECLARE_EXT_ISR_M(APP_BOARD_GPTMR_IRQ, tick_ms_isr);

static void timer_config(void)
{
    uint32_t gptmr_freq;
    gptmr_channel_config_t config;

    gptmr_channel_get_default_config(APP_BOARD_GPTMR, &config);

    gptmr_freq = clock_get_frequency(APP_BOARD_GPTMR_CLOCK);
    config.reload = gptmr_freq / 1000*  APP_TICK_MS;
    gptmr_channel_config(APP_BOARD_GPTMR, APP_BOARD_GPTMR_CH, &config, false);
    gptmr_start_counter(APP_BOARD_GPTMR, APP_BOARD_GPTMR_CH);

    gptmr_enable_irq(APP_BOARD_GPTMR, GPTMR_CH_RLD_IRQ_MASK(APP_BOARD_GPTMR_CH));
    intc_m_enable_irq_with_priority(APP_BOARD_GPTMR_IRQ, 1);
}


float VBUS; //!<@brief 母线电压
MotorClass_t motor0;
static void mt6701_isr_callback(uint32_t isr_flag)
{
//    uint8_t mt6701_status = encoder_get_status();
//
//    if (mt6701_status & 0xf)
//    {
//        // disable_all_pwm_output();
////        board_led_write(BOARD_LED_GPIO_RED_PIN, BOARD_LED_ON_LEVEL);
//        rt_kprintf( "MT6701 Error %x\n", mt6701_status);
//    }

    // 电压保护，复位芯片恢复
//    VBUS = adc_voltage(adc_GetRaw_VBUS()) * VOLTAGE_AMP;
//    if (VBUS < 9 || VBUS > 30)
//    {
//        pwm_disable_all_output();
////        board_led_write(BOARD_LED_GPIO_RED_PIN, BOARD_LED_ON_LEVEL);
//        rt_kprintf( "VBUS Error: %dmV\n", (int)(VBUS * 1000));
//    }
    motor0.bus_voltage = 12.0;//VBUS

    Motor_RunFoc(&motor0);
}
void motor0_get_uvw_current(MotorClass_t *motor, foc_uvw_current_t *uvw_current)
{
    current_get_cal(&motor->current_cal, adc_GetRaw_V(), adc_GetRaw_W(), uvw_current);
}

uint16_t motor0_get_raw_angle(MotorClass_t *motor)
{
//    return encoder_get_rawAngle();
    return read_raw_angel;
}

void motor0_set_pwm(MotorClass_t *motor, const foc_pwm_t *pwm)
{
    pwm_setvalue(pwm);
}

void motor0_enable_pwm(MotorClass_t *motor, bool en)
{
    en ? pwm_enable_all_output() : pwm_disable_all_output();
}
int main(void)
{
    app_init_led_pins();
    /* PLL测速参数 */
       // motor0.speed_pll.pi.kp = 0.2;
       // motor0.speed_pll.pi.ki = 0.1;
       // motor0.speed_pll.pi.integral_limit = 5;
    /*
    motor0.speed_pll.pi.kp = 0.05f;
    motor0.speed_pll.pi.output_limit = 200;

    */
    motor0.angle_pid.kp = 0.0015f;
    motor0.angle_pid.ki = 0.00f;
    motor0.angle_pid.integral_limit = 100;
    motor0.angle_pid.output_limit = 100;


    //motor0.speed_exp
    motor0.speed_pid.kp = 0.05f;
    motor0.speed_pid.ki = 0.00f;
    motor0.speed_pid.integral_limit = 5;
    motor0.speed_pid.output_limit = 10;


    motor0.current_iq_pid.kp = 1.0f;
    motor0.current_iq_pid.ki = 0.001f;
    motor0.current_iq_pid.kd = -0.6f;
    motor0.current_iq_pid.integral_limit = 40;
    motor0.current_iq_pid.output_limit = 300;


    motor0.current_id_pid.kp = 0.07f;
    motor0.current_id_pid.ki = 0.0f;
    motor0.current_id_pid.integral_limit = 3;


   motor0.get_uvw_current_cb = motor0_get_uvw_current;
   motor0.get_raw_angle_cb = motor0_get_raw_angle;
   motor0.set_pwm_cb = motor0_set_pwm;
   motor0.enable_pwm = motor0_enable_pwm;
   Motor_Init(&motor0);
   /* PWM初始化 */
   pwm_init(20, 50);

   /* ADC初始化 */
   adc_init(30);
   /* 连接PWMCH8、ADCX_PTRGI0A */
   trgm_connect(HPM_TRGM0_INPUT_SRC_PWM0_CH8REF, HPM_TRGM0_OUTPUT_SRC_ADCX_PTRGI0A, trgm_output_same_as_input, false);
   /* 连接PWMCH9、ADCX_PTRGI0B */
   trgm_connect(HPM_TRGM0_INPUT_SRC_PWM0_CH9REF, HPM_TRGM0_OUTPUT_SRC_ADCX_PTRGI0B, trgm_output_same_as_input, false);

   /* adc中值校准程序 */
   rt_kprintf("adc中值校准程序 ing\r\n");
   current_calibration(&motor0);

   /* 编码器初始化 */
#if ENCODER_TYPE == ENCODER_MT6701
   board_init_sei_pins(BOARD_SEI, BOARD_SEI_CTRL);
   sei_trigger_input_config_t mt6701_trigger_config = {.trig_in0_enable = true, .trig_in0_select = 0};
   // encoder_set_callback(mt6701_isr_callback);
   mt6701_ssi_init(13000000, &mt6701_trigger_config);
   intc_m_enable_irq_with_priority(BOARD_SEI_IRQn, 1);
   /* 连接PWMCH8、SEI_TRIG_IN0 */
   trgm_connect(HPM_TRGM0_INPUT_SRC_PWM1_CH9REF, HPM_TRGM0_OUTPUT_SRC_SEI_TRIG_IN0, trgm_output_same_as_input, false);
#endif

   /* 电角度校准 */
   rt_kprintf("电角度校准 ing\r\n");
   if (electrical_angle_calibration(&motor0) == 0)
   {
       rt_kprintf("设置电机工作模式\r\n");
       Motor_SetMode(&motor0, ANGLE_MODE);
       //Motor_SetMode(&motor0, CURRENT_MODE);
   }
//   init_gptmr_pins(APP_BOARD_GPTMR);
   rt_kprintf("gptmr timer basic test\n");
   timer_config();
   uint32_t count=0;
   uint16_t last_angle = 0;
   int16_t angle_diff = 0;
    while(1){
       count++;
       read_raw_angel=encoder_get_rawAngle();

       motor0.angle_exp = ((int)((read_raw_angel + 256)/512))*512;

       rt_thread_mdelay(1);
       if(count %50==0)
       {
           board_led_toggle();
       }
//           app_led_write(0, APP_LED_OFF);
//           rt_thread_mdelay(500);
       }

    return 0;
}

typedef struct
{
    const char *name;
    void (*fun)(float);
    float *tar_val;
} CmdCallback_t;

void set_expang(float ang)
{
    motor0.angle_exp = ang;
}

CmdCallback_t cmd_list_FOC[] = {
    {"exp_iq", NULL, &motor0.qd_current_exp.iq},
    {"exp_id", NULL, &motor0.qd_current_exp.id},
    {"id_p", NULL, &motor0.current_id_pid.kp},
    {"id_i", NULL, &motor0.current_id_pid.ki},
    {"id_d", NULL, &motor0.current_id_pid.kd},
    {"id_il", NULL, &motor0.current_id_pid.integral_limit},
    {"iq_p", NULL, &motor0.current_iq_pid.kp},
    {"iq_i", NULL, &motor0.current_iq_pid.ki},
    {"iq_d", NULL, &motor0.current_iq_pid.kd},
    {"iq_il", NULL, &motor0.current_iq_pid.integral_limit},
    {"speed_p", NULL, &motor0.speed_pid.kp},
    {"speed_i", NULL, &motor0.speed_pid.ki},
    {"speed_d", NULL, &motor0.speed_pid.kd},
    {"exp_speed", NULL, &motor0.speed_exp},
    {"ang_p", NULL, &motor0.angle_pid.kp},
    {"ang_i", NULL, &motor0.angle_pid.ki},
    {"exp_ang", set_expang, NULL},
    {"speed_filter", NULL, &motor0.speed_pll.pi.kp},
};
static void M_SET_CMD(int argc, char *argv[])
{
    if (argc < 2)
    {
        rt_kprintf("ERRO INPUT\n");
        return;
    }
    char *name;

    float value = 0;
//    char *start = data;
    name=argv[1];
    value= atof(argv[2]);

    for (int index = 0; index < sizeof(cmd_list_FOC) / sizeof(CmdCallback_t); index++)
    {
      if (strcmp(cmd_list_FOC[index].name, name) == 0)
      {
          rt_kprintf("set %s %d.%2d\n", cmd_list_FOC[index].name, (int)value,(int)(value*100)%100 );
          if (cmd_list_FOC[index].fun == NULL)
          {
              *(cmd_list_FOC[index].tar_val) = value;
          }
          else
          {
              cmd_list_FOC[index].fun(value);
          }
          break;
      }


    }

}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(M_SET_CMD, MOTOR SET);
