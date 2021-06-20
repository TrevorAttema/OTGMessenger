/*
 * MC2PA8201.h
 *
 *  Created on: 29 May 2021
 *      Author: Trevor Attema
 */

#ifndef BSP_INC_MC2PA8201_H_
#define BSP_INC_MC2PA8201_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


typedef enum _COLOR_MODE {
  COLOR_12BIT = 0x03,
  COLOR_16BIT = 0x05,
  COLOR_18BIT = 0x06,
  COLOR_24BIT = 0x07
} COLOR_MODE;

typedef enum _ORIENTATION_MODE {
  ORIENTATION_PORTRAIT = 0xA0,
  ORIENTATION_LANDSCAPE = 0x00,
  ORIENTATION_PORTRAIT_REV = 0x60,
  ORIENTATION_LANDSCAPE_REV = 0xC0
} ORIENTATION_MODE;

typedef enum MC2PA8201_GAMMA {
  GAMMA_CURVE1 = 1,
  GAMMA_CURVE2 = 2,
  GAMMA_CURVE3 = 4,
  GAMMA_CURVE4 = 8
} GAMMA_VALUE;

typedef enum MC2PA8201_TE {
  TE_VBLANK = 0x00,
  TE_VBLANK_HBLANK = 0x01,
} TE;

typedef enum _FONT_SIZE {
  FONT_6x8 = 0,
  FONT_8x8 = 1,
  FONT_8x14 = 2
} FONT_SIZE;

enum {
  MC2PA8201_OK = 1,
  MC2PA8201_ERROR = 0
};

enum COMMANDS {
  NOP                               = 0x00,
  SOFTWARE_RESET                    = 0x01,
  READ_DISPLAY_ID                   = 0x04,
  READ_DISPLAY_STATUS               = 0x09,
  READ_DISPLAY_POWER_MODE           = 0x0a,
  READ_DISPLAY_MADCTL               = 0x0b,
  READ_DISPLAY_PIXEL_FORMAT         = 0x0c,
  READ_DISPLAY_IMAGE_MODE           = 0x0d,
  READ_DISPLAY_SIGNAL_MODE          = 0x0e,
  READ_DISPLAY_SELF_DIAGNOSTICS     = 0x0f,
  SLEEP_IN                          = 0x10,
  SLEEP_OUT                         = 0x11,
  PARTIAL_MODE_ON                   = 0x12,
  NORMAL_DISPLAY_MODE_ON            = 0x13,
  DISPLAY_INVERSION_OFF             = 0x20,
  DISPLAY_INVERSION_ON              = 0x21,
  GAMMA_SET                         = 0x26,
  DISPLAY_OFF                       = 0x28,
  DISPLAY_ON                        = 0x29,
  COLUMN_ADDRESS_SET                = 0x2a,
  PAGE_ADDRESS_SET                  = 0x2b,
  MEMORY_WRITE                      = 0x2c,
  COLOUR_SET                        = 0x2d,
  MEMORY_READ                       = 0x2e,
  PARTIAL_AREA                      = 0x30,
  VERTICAL_SCROLLING_DEFINITION     = 0x33,
  TEARING_EFFECT_LINE_OFF           = 0x34,
  TEARING_EFFECT_LINE_ON            = 0x35,
  MEMORY_ACCESS_CONTROL             = 0x36,
  VERTICAL_SCROLLING_START_ADDRESS  = 0x37,
  IDLE_MODE_OFF                     = 0x38,
  IDLE_MODE_ON                      = 0x39,
  INTERFACE_PIXEL_FORMAT            = 0x3a,
  READ_ID1                          = 0xda,
  READ_ID2                          = 0xdb,
  READ_ID3                          = 0xdc
};

#define MAC_BGR_BIT								((uint8_t)0b00001000)

//#define LCD_REG8     (*((volatile uint8_t*)0x60000000))
//#define LCD_DAT8     (*((volatile uint8_t*)0x60040000))

//STM32H7 remapped memory address
#define LCD_REG8     (*((volatile uint8_t*)0xC0000000))
#define LCD_DAT8     (*((volatile uint8_t*)0xC0040000))

uint8_t MC2PA8201_Init(uint8_t AddressSetupTime,uint8_t DataSetupTime);
void MC2PA8201_ColorMode(COLOR_MODE color_mode);
void MC2PA8201_OrientationMode(ORIENTATION_MODE orientation_mode);
void MC2PA8201_SetLUT_params(COLOR_MODE color_mode,uint8_t LUT_size,uint8_t *LUT);
// if dimensions are non standard (Standard: landscape width=320,height=240; portrait width=240,height=320)
void MC2PA8201_SetDimensions(uint16_t width,uint16_t height);
// if MADCTL parameters are non standard (Standard: landscape=0x00; portrait=0xA0; landscape_rev=0xC0; portait_rev=0x60)
void MC2PA8201_SetMADCTL_params(uint8_t landscape,uint8_t portrait,uint8_t landscape_rev,uint8_t portrait_rev);

void MC2PA8201_ClearScreen(uint32_t color);
void MC2PA8201_SetPixel(uint16_t x, uint16_t y, uint32_t color);
void MC2PA8201_FillPixel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint32_t *color);
void MC2PA8201_FillFromBuffer(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,const uint8_t *data);
void MC2PA8201_Fill(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint32_t color);
void MC2PA8201_SetScrollPosition(uint16_t pos);
void MC2PA8201_ScrollArea(uint16_t y,uint16_t pos);
void MC2PA8201_DisplayOff(void);
void MC2PA8201_DisplayOn(void);
void MC2PA8201_Sleep(void);
void MC2PA8201_Wakeup(void);
void MC2PA8201_Gamma(GAMMA_VALUE val);
void MC2PA8201_IdleModeOn(void);
void MC2PA8201_IdleModeOff(void);
void MC2PA8201_DisplayInversionOn(void);
void MC2PA8201_DisplayInversionOff(void);
void MC2PA8201_TearingEffectLineOn(TE val);
void MC2PA8201_TearingEffectLineOff(void);
void MC2PA8201_PartialArea(uint16_t y0,uint16_t y1);
void MC2PA8201_PartialMode(void);
void MC2PA8201_NormalDisplayMode(void);
void MC2PA8201_SetWindow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1);
void MC2PA8201_SetWriteWindow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1);
void MC2PA8201_SetReadWindow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1);
uint16_t MC2PA8201_GetWidth(void);
uint16_t MC2PA8201_GetHeight(void);

void MC2PA8201_ReadRegister(uint8_t reg,uint8_t length,uint8_t *val);
void MC2PA8201_ReadMemory(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint8_t *buf);

void MC2PA8201_On();
void MC2PA8201_Off();
void MC2PA8201_BacklightOff();
void MC2PA8201_BacklightOn();

void MC2PA8201_SetBGROrder(uint8_t val);


#ifdef __cplusplus
}
#endif

#endif /* BSP_INC_MC2PA8201_H_ */
