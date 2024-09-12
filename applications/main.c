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
    VBUS = adc_voltage(adc_GetRaw_VBUS()) * VOLTAGE_AMP;
    if (VBUS < 9 || VBUS > 30)
    {
        pwm_disable_all_output();
//        board_led_write(BOARD_LED_GPIO_RED_PIN, BOARD_LED_ON_LEVEL);
        rt_kprintf( "VBUS Error: %dmV\n", (int)(VBUS * 1000));
    }

    motor0.bus_voltage = VBUS;

    Motor_RunFoc(&motor0);

    /* 使能DTR才发送数据，方便vofa静止查看波形 */
//    if (dtr_enable)
//    {
//        vofa_data[write_ptr].data[0] = motor0.qd_current.iq;
//        vofa_data[write_ptr].data[1] = motor0.qd_current_exp.iq;
//        vofa_data[write_ptr].data[2] = motor0.qd_current.id;
//        vofa_data[write_ptr].data[3] = motor0.qd_current_exp.id;
//        vofa_data[write_ptr].data[4] = motor0.speed;
//        vofa_data[write_ptr].data[5] = motor0.speed_exp;
//
//        // vofa_data[write_ptr].data[2] = motor0.speed_pll.theta;
//        // vofa_data[write_ptr].data[3] =
//        //     motor0.speed_pll.speed / motor0.encoder.pole_pairs / (2 * F_PI) * 60 * SPEED_PID_FREQUENCY;
//        // vofa_data[write_ptr].data[5] = foc_para.currentdpipar.outval;
//
//        vofa_data[write_ptr].data[6] = motor0.uvw_current.iu;
//        vofa_data[write_ptr].data[7] = motor0.uvw_current.iv;
//        vofa_data[write_ptr].data[8] = motor0.uvw_current.iw;
//
//        vofa_data[write_ptr].data[9] = motor0.raw_angle;
//
//        vofa_data[write_ptr].data[10] = VBUS;
//        vofa_data[write_ptr].data[11] = motor0.power;
//
//        // vofa_data[write_ptr].data[14] = pll_speed * PWM_FREQUENCY * 60 / 2 / F_PI;
//
//        // vofa_data[write_ptr].data[12] = pwm.pwm_u;
//        // vofa_data[write_ptr].data[13] = pwm.pwm_v;
//        // vofa_data[write_ptr].data[14] = pwm.pwm_w;
//
//        // vofa_data[write_ptr].data[14] = (float)hpm_csr_get_core_cycle() / hpm_core_clock;
//
//        if (write_ptr == 0)
//        {
//            if (!ep_tx_busy_flag)
//            {
//                ep_tx_busy_flag = true;
//                usbd_ep_start_write(0x81, &vofa_data[BUF_NUM / 2], sizeof(just_float_data) * BUF_NUM / 2);
//            }
//        }
//
//        if (write_ptr == BUF_NUM / 2)
//        {
//            if (!ep_tx_busy_flag)
//            {
//                ep_tx_busy_flag = true;
//                usbd_ep_start_write(0x81, &vofa_data[0], sizeof(just_float_data) * BUF_NUM / 2);
//            }
//        }
//        write_ptr = (write_ptr + 1) % BUF_NUM;
//    }

    // gpio_write_pin(HPM_GPIO0, GPIO_OE_GPIOB, 5, 0);
}
void motor0_get_uvw_current(MotorClass_t *motor, foc_uvw_current_t *uvw_current)
{
    current_get_cal(&motor->current_cal, adc_GetRaw_V(), adc_GetRaw_W(), uvw_current);
}

uint16_t motor0_get_raw_angle(MotorClass_t *motor)
{
    return encoder_get_rawAngle();
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
   motor0.speed_pll.pi.output_limit = 2000;

   motor0.speed_pll.pi.kp = 0.05f;
   motor0.speed_pll.pi.output_limit = 200;

   motor0.angle_pid.kp = 0.08f;
   motor0.angle_pid.ki = 0.002f;
   motor0.angle_pid.integral_limit = 300;
   motor0.angle_pid.output_limit = 1000;

   motor0.speed_pid.kp = 0.01f;
   motor0.speed_pid.ki = 0.01f;
   motor0.speed_pid.integral_limit = 5;
   motor0.speed_pid.output_limit = 10;

   motor0.current_iq_pid.kp = 1.6f;
   motor0.current_iq_pid.ki = 0.07f;
   motor0.current_iq_pid.integral_limit = 6;
   motor0.current_id_pid.kp = 1.6f;
   motor0.current_id_pid.ki = 0.03f;
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
       Motor_SetMode(&motor0, ANGLE_MODE);
//           encoder_set_callback(mt6701_isr_callback);

   }
    while(1){
           app_led_write(0, APP_LED_ON);
           rt_thread_mdelay(500);
           app_led_write(0, APP_LED_OFF);
           rt_thread_mdelay(500);
       }

    return 0;
}


