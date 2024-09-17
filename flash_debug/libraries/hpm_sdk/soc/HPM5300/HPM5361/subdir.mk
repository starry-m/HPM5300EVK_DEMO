################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_clock_drv.c \
../libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_l1c_drv.c \
../libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_otp_drv.c \
../libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_sysctl_drv.c \
../libraries/hpm_sdk/soc/HPM5300/HPM5361/system.c 

O_SRCS += \
../libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_clock_drv.o \
../libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_l1c_drv.o \
../libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_otp_drv.o \
../libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_sysctl_drv.o \
../libraries/hpm_sdk/soc/HPM5300/HPM5361/system.o 

OBJS += \
./libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_clock_drv.o \
./libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_l1c_drv.o \
./libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_otp_drv.o \
./libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_sysctl_drv.o \
./libraries/hpm_sdk/soc/HPM5300/HPM5361/system.o 

C_DEPS += \
./libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_clock_drv.d \
./libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_l1c_drv.d \
./libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_otp_drv.d \
./libraries/hpm_sdk/soc/HPM5300/HPM5361/hpm_sysctl_drv.d \
./libraries/hpm_sdk/soc/HPM5300/HPM5361/system.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/hpm_sdk/soc/HPM5300/HPM5361/%.o: ../libraries/hpm_sdk/soc/HPM5300/HPM5361/%.c
	riscv32-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-strict-align -mno-save-restore -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -gdwarf-2 -DDEBUG -DUSE_NONVECTOR_MODE=1 -DFLASH_XIP=1 -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\applications" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\board" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\drivers" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\arch" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\components\adc" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\components\debug_console" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\components\dma_mgr" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\components\touch" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\components\usb" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\drivers\inc" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\soc\HPM5300\HPM5361\boot" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\soc\HPM5300\HPM5361" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\soc\HPM5300\ip" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\misc\rtt_interrupt_util" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\misc\rtt_os_tick" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\drivers\include" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\drivers\spi" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\finsh" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\compilers\common\include" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\compilers\newlib" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\posix\io\epoll" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\posix\io\eventfd" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\posix\io\poll" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\posix\ipc" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\include" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\libcpu\risc-v\common" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\startup\HPM5361" -include"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rtconfig_preinc.h" -std=gnu11 -ffunction-sections -fdata-sections -fno-common   -mcmodel=medlow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

