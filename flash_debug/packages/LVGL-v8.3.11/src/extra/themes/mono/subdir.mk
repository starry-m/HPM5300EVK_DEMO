################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../packages/LVGL-v8.3.11/src/extra/themes/mono/lv_theme_mono.c 

OBJS += \
./packages/LVGL-v8.3.11/src/extra/themes/mono/lv_theme_mono.o 

C_DEPS += \
./packages/LVGL-v8.3.11/src/extra/themes/mono/lv_theme_mono.d 


# Each subdirectory must supply rules for building sources it contributes
packages/LVGL-v8.3.11/src/extra/themes/mono/%.o: ../packages/LVGL-v8.3.11/src/extra/themes/mono/%.c
	riscv32-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-strict-align -mno-save-restore -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -gdwarf-2 -DDEBUG -DUSE_NONVECTOR_MODE=1 -DFLASH_XIP=1 -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\applications" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\board" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\drivers" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\arch" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\components\adc" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\components\debug_console" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\components\dma_mgr" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\components\touch" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\components\usb" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\drivers\inc" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\soc\HPM5300\HPM5361\boot" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\soc\HPM5300\HPM5361" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\hpm_sdk\soc\HPM5300\ip" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\misc\rtt_interrupt_util" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\libraries\misc\rtt_os_tick" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\env_support\rt-thread" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\core" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\draw\arm2d" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\draw\nxp\pxp" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\draw\nxp\vglite" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\draw\renesas" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\draw\sdl" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\draw\stm32_dma2d" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\draw\swm341_dma2d" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\draw\sw" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\draw" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\layouts\flex" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\layouts\grid" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\layouts" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\libs\bmp" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\libs\ffmpeg" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\libs\freetype" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\libs\fsdrv" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\libs\gif" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\libs\png" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\libs\qrcode" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\libs\rlottie" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\libs\sjpg" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\libs\tiny_ttf" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\libs" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\others\fragment" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\others\gridnav" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\others\ime" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\others\imgfont" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\others\monkey" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\others\msg" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\others\snapshot" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\others" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\themes\basic" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\themes\default" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\themes\mono" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\themes" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\animimg" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\calendar" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\chart" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\colorwheel" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\imgbtn" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\keyboard" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\led" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\list" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\menu" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\meter" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\msgbox" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\span" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\spinbox" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\spinner" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\tabview" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\tileview" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets\win" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra\widgets" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\extra" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\font" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\hal" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\misc" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src\widgets" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\packages\LVGL-v8.3.11\src" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\drivers\include" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\drivers\spi" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\finsh" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\compilers\common\include" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\compilers\newlib" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\posix\io\epoll" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\posix\io\eventfd" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\posix\io\poll" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\components\libc\posix\ipc" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\include" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rt-thread\libcpu\risc-v\common" -I"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\startup\HPM5361" -include"D:\workTools\RT-ThreadStudio\workspace\HPM5300EVK_DEMO\rtconfig_preinc.h" -std=gnu11 -ffunction-sections -fdata-sections -fno-common   -mcmodel=medlow -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

