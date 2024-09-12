/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-09-11     starry       the first version
 */
#ifndef BOARD_DRV_AS5600_H_
#define BOARD_DRV_AS5600_H_

#include "stdint.h"

float getAngle_Without_track();
uint16_t getRawAngle();
float getAngle();
#endif /* BOARD_DRV_AS5600_H_ */
