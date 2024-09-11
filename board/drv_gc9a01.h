#ifndef __DRV_GC9A01_H__
#define __DRV_GC9A01_H__

#include "stdint.h"

#define Cmd_SLPIN       0x10
#define Cmd_SLPOUT      0x11
#define Cmd_INVOFF      0x20
#define Cmd_INVON       0x21
#define Cmd_DISPOFF     0x28
#define Cmd_DISPON      0x29
#define Cmd_CASET       0x2A
#define Cmd_RASET       0x2B
#define Cmd_RAMWR       0x2C
#define Cmd_TEON        0x35    // Tearing effect line ON
#define Cmd_MADCTL      0x36    // Memory data access control
#define Cmd_COLMOD      0x3A   // Pixel format set

#define Cmd_DisplayFunctionControl    0xB6
#define Cmd_PWCTR1       0xC1    // Power control 1
#define Cmd_PWCTR2       0xC3    // Power control 2
#define Cmd_PWCTR3       0xC4    // Power control 3
#define Cmd_PWCTR4       0xC9    // Power control 4
#define Cmd_PWCTR7       0xA7    // Power control 7

#define Cmd_FRAMERATE      0xE8
#define Cmd_InnerReg1Enable   0xFE
#define Cmd_InnerReg2Enable   0xEF

#define Cmd_GAMMA1       0xF0    // Set gamma 1
#define Cmd_GAMMA2       0xF1    // Set gamma 2
#define Cmd_GAMMA3       0xF2    // Set gamma 3
#define Cmd_GAMMA4       0xF3    // Set gamma 4

#define ColorMode_RGB_16bit  0x50
#define ColorMode_RGB_18bit  0x60
#define ColorMode_MCU_12bit  0x03
#define ColorMode_MCU_16bit  0x05
#define ColorMode_MCU_18bit  0x06

#define MADCTL_MY        0x80
#define MADCTL_MX        0x40
#define MADCTL_MV        0x20
#define MADCTL_ML        0x10
#define MADCTL_BGR       0x08
#define MADCTL_MH        0x04

#define GC9A01_Width    240
#define GC9A01_Height   240


void GC9A01_SleepMode(uint8_t Mode);
void GC9A01_DisplayPower(uint8_t On);
void GC9A01_DrawPixel(int16_t x, int16_t y, uint16_t color);
void GC9A01_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void GC9A01_ColorFill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void GC9A01_Update();
void GC9A01_SetBL(uint8_t Value);
uint16_t GC9A01_GetPixel(int16_t x, int16_t y);

void GC9A01_Screen_Shot(uint16_t x,uint16_t y,uint16_t width ,uint16_t height,uint16_t * Buffer);
void GC9A01_Screen_Load(uint16_t x,uint16_t y,uint16_t width ,uint16_t height,uint16_t * Buffer);
void GC9A01_Clear(uint16_t color);

#endif /* __DRV_GC9A01_H__ */
