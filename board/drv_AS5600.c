/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-09-11     starry       the first version
 */
#include "drv_AS5600.h"

#include <rtthread.h>
#include <rtdevice.h>
#include "rtt_board.h"
#include "stdlib.h"
#define DBG_TAG "ENCODER"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define AS5600_BUS "i2c0"
#define Slave_Addr 0x36
#define Write_Bit 0
#define Read_Bit 1
#define Angle_Hight_Register_Addr 0x0C
#define Angle_Low_Register_Addr 0x0D
#define PI 3.14159f

int ledtime = 0;
int32_t full_rotations=0; // full rotation tracking;
float angle_prev=0; 

static struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C总线设备句柄 */
/* 写传感器寄存器 */
static rt_err_t write_reg(struct rt_i2c_bus_device *bus)
{
    struct rt_i2c_msg msgs;
    rt_uint8_t buf = Angle_Hight_Register_Addr;

    msgs.addr = Slave_Addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = &buf;
    msgs.len = 1;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

/* 读传感器寄存器数据 */
static rt_err_t read_regs(struct rt_i2c_bus_device *bus, rt_uint8_t len, rt_uint8_t *buf)
{
    struct rt_i2c_msg msgs;

    msgs.addr = Slave_Addr;
    msgs.flags = RT_I2C_RD;
    msgs.buf = buf;
    msgs.len = len;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}
static void hall_init(const char *name)
{
    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(name);

    if (i2c_bus == RT_NULL)
    {
        LOG_E("can't find %s device!\n", name);
    }
    else
    {

    }
}
static void read_angle()
{
    rt_uint8_t temp[2];

    write_reg(i2c_bus);      /* 发送命令 */
    read_regs(i2c_bus, 2, temp); /* 获取传感器数据 */

    temp[0] = temp[0] & 0xF;
    rt_uint16_t angle = temp[0]*256 + temp[1];
    LOG_I("angle:%d",angle);
}

uint16_t getRawAngle()
{
    rt_uint8_t temp[2];
    write_reg(i2c_bus);      /* 发送命令 */
    read_regs(i2c_bus, 2, temp); /* 获取传感器数据 */
    temp[0] = temp[0] & 0xF;
    uint16_t angle = temp[0]*256 + temp[1];
    // LOG_I("angle:%d",angle);
    return angle;
}

float getAngle_Without_track()
{
  return getRawAngle()*0.08789f* PI / 180;    //得到弧度制的角度
}

float getAngle()
{
    float val = getAngle_Without_track();
    float d_angle = val - angle_prev;
    //计算旋转的总圈数
    //通过判断角度变化是否大于80%的一圈(0.8f*6.28318530718f)来判断是否发生了溢出，如果发生了，则将full_rotations增加1（如果d_angle小于0）或减少1（如果d_angle大于0）。
    if(abs(d_angle) > (0.8f*6.28318530718f) ) full_rotations += ( d_angle > 0 ) ? -1 : 1; 
    angle_prev = val;
    return (float)full_rotations * 6.28318530718f + angle_prev;
    
}
static void AS5600_sample(int argc, char *argv[])
{
    hall_init(AS5600_BUS);
    float val=0.0;
    while(1)
    {
        rt_thread_mdelay(10);
        val=getAngle();
        LOG_I("GET angel:%d.%d",(uint16_t)val,(uint16_t)(val*100)%100);
    }
//    read_angle();
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(AS5600_sample, i2c hall sample);

static int ENCODER_Device_Init(void)
{
    hall_init(AS5600_BUS);
    LOG_I("AS5600 INIT success !");
}
/* 导出到自动初始化 */
INIT_DEVICE_EXPORT(ENCODER_Device_Init);
