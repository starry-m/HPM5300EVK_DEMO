/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-09-12     starry       the first version
 */
#ifndef BOARD_PWM_INIT_H_
#define BOARD_PWM_INIT_H_

#include "board.h"
#include "foc/foc_core.h"
#include "hpm_pwm_drv.h"
#include <stdint.h>


int pwm_init(uint32_t ch8_cmp, uint32_t dead_time_ns);

static inline void pwm_enable_all_output(void)
{
    pwm_disable_sw_force(BOARD_BLDCPWM);
}

static inline void pwm_disable_all_output(void)
{
    pwm_enable_sw_force(BOARD_BLDCPWM);
}

void pwm_setvalue(const foc_pwm_t *par);


#endif /* BOARD_PWM_INIT_H_ */
