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

