/*
 * MC2PA8201.c
 *
 *  Created on: 29 May 2021
 *      Author: Trevor Attema
 */


#include <gpio.h>

#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

#include "MC2PA8201.h"
#include "MC2PA8201/colors.h"
#include <string.h>
#include "e63_display_driver.h"

static COLOR_MODE MC2PA8201_color_mode;
static ORIENTATION_MODE MC2PA8201_orientation_mode;

static uint8_t RGB12bit_size;
static uint8_t *RGB12bit;
static uint8_t RGB16bit_size;
static uint8_t *RGB16bit;
static uint8_t RGB18bit_size;
static uint8_t *RGB18bit;

static uint16_t display_width=320;
static uint16_t display_height=240;

static uint8_t MADCTL_portrait=0xA0;
static uint8_t MADCTL_landscape=0x00;
static uint8_t MADCTL_portrait_rev=0x60;
static uint8_t MADCTL_landscape_rev=0xC0;

EXTI_HandleTypeDef hDisplayEXTI;


static void MC2PA8201_IRQHandler(void);

static void lcd_rst(void) {

	HAL_GPIO_WritePin(LCD_RESX_GPIO_Port, LCD_RESX_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(LCD_RESX_GPIO_Port, LCD_RESX_Pin, GPIO_PIN_SET);
  HAL_Delay(120);
}

static __inline uint8_t rd_reg(uint8_t index) {
  LCD_REG8 = (uint8_t)index;
  return (LCD_DAT8);
}

static __inline uint8_t rd_dat(void) {
  return (LCD_DAT8);
}

static __inline void wr_cmd(uint8_t index) {
  LCD_REG8 = index;
}

static __inline void wr_dat(uint8_t val) {
  LCD_DAT8 = val;
}

static __inline void wr_reg(uint8_t index,uint8_t val) {
  LCD_REG8 = index;
  LCD_DAT8 = val;
}

uint8_t MC2PA8201_Init(uint8_t AddressSetupTime,uint8_t DataSetupTime) {

  //GPIO_Configuration();
  //FSMC_LCD_Init(AddressSetupTime,DataSetupTime);

	HAL_EXTI_GetHandle(&hDisplayEXTI, EXTI_LINE_15);
	HAL_EXTI_RegisterCallback(&hDisplayEXTI, HAL_EXTI_COMMON_CB_ID, MC2PA8201_IRQHandler);
  //HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0x5, 0x00);
  //HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  lcd_rst();

  wr_cmd(SLEEP_OUT);
  wr_cmd(DISPLAY_INVERSION_OFF);
  wr_cmd(IDLE_MODE_OFF);
  wr_cmd(NORMAL_DISPLAY_MODE_ON);

  HAL_Delay(125);
  wr_cmd(DISPLAY_ON);

//  HAL_GPIO_WritePin(LCD_BACKLIGHT_EN_GPIO_Port, LCD_BACKLIGHT_EN_Pin, GPIO_PIN_SET);
//  HAL_GPIO_WritePin(LCD_BACKLIGHT_PWM_GPIO_Port, LCD_BACKLIGHT_PWM_Pin, GPIO_PIN_SET);

  MC2PA8201_TearingEffectLineOn(TE_VBLANK);
  MC2PA8201_BacklightOn();

  MC2PA8201_ColorMode(COLOR_24BIT);
  MC2PA8201_OrientationMode(ORIENTATION_LANDSCAPE);
  MC2PA8201_SetBGROrder(MAC_BGR_BIT);

  MC2PA8201_ClearScreen(BLACK);

  return MC2PA8201_OK;
}

void MC2PA8201_SetLUT_params(COLOR_MODE color_mode,uint8_t LUT_size,uint8_t *LUT) {

  switch(color_mode) {
    case COLOR_12BIT:
      RGB12bit_size=LUT_size;
      RGB12bit=LUT;
      break;
    case COLOR_16BIT:
      RGB16bit_size=LUT_size;
      RGB16bit=LUT;
      break;
    case COLOR_18BIT:
      RGB18bit_size=LUT_size;
      RGB18bit=LUT;
      break;
    case COLOR_24BIT:
      break;
  }
}

void MC2PA8201_SetDimensions(uint16_t width,uint16_t height) {

  display_width=width;
  display_height=height;
}

void MC2PA8201_ColorMode(COLOR_MODE color_mode) {

  uint32_t i;

  MC2PA8201_color_mode=color_mode;
  wr_reg(INTERFACE_PIXEL_FORMAT,MC2PA8201_color_mode);

  switch(MC2PA8201_color_mode) {
    case COLOR_12BIT:
      wr_cmd(COLOUR_SET);
      for(i=0;i<RGB12bit_size;i++) {
        wr_dat(RGB12bit[i]);
      }
      break;
    case COLOR_16BIT:
      wr_cmd(COLOUR_SET);
      for(i=0;i<RGB16bit_size;i++) {
        wr_dat(RGB16bit[i]);
      }
      break;
    case COLOR_18BIT:
      wr_cmd(COLOUR_SET);
      for(i=0;i<RGB18bit_size;i++) {
        wr_dat(RGB18bit[i]);
      }
      break;
    case COLOR_24BIT:
      break;
  }
}

void MC2PA8201_SetMADCTL_params(uint8_t landscape,uint8_t portrait,uint8_t landscape_rev,uint8_t portrait_rev) {

  MADCTL_landscape=landscape;
  MADCTL_portrait=portrait;
  MADCTL_landscape_rev=landscape_rev;
  MADCTL_portrait_rev=portrait_rev;
}

void MC2PA8201_OrientationMode(ORIENTATION_MODE orientation_mode) {

  uint8_t MADCTL_param;

  switch(orientation_mode){
    case ORIENTATION_LANDSCAPE:
      MADCTL_param=MADCTL_landscape;
      break;
    case ORIENTATION_LANDSCAPE_REV:
      MADCTL_param=MADCTL_landscape_rev;
      break;
    case ORIENTATION_PORTRAIT:
      MADCTL_param=MADCTL_portrait;
      break;
    case ORIENTATION_PORTRAIT_REV:
      MADCTL_param=MADCTL_portrait_rev;
      break;
  }
  MC2PA8201_orientation_mode=orientation_mode;
  wr_reg(MEMORY_ACCESS_CONTROL,MADCTL_param);
}

void MC2PA8201_ClearScreen(uint32_t color) {

  MC2PA8201_Fill(0,0,MC2PA8201_GetWidth()-1,MC2PA8201_GetHeight()-1,color);
}

void MC2PA8201_Fill(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint32_t color) {

  uint32_t i,j=(x1-x0+1)*(y1-y0+1);
  uint8_t b1,b2,b3,r,g,b;

  MC2PA8201_SetWriteWindow(x0,y0,x1,y1);

  r=color>>16;
  g=color>>8;
  b=color;

  switch(MC2PA8201_color_mode) {
    case COLOR_12BIT:
      b1=(r&0xF0)|(g>>4);
      b2=(b&0xF0)|(r>>4);
      b3=(g&0xF0)|(b>>4);
      for(i=0;i<j;i+=2) {
        wr_dat(b1);
        wr_dat(b2);
        wr_dat(b3);
      }
      break;
    case COLOR_16BIT:
      b1=(r&0xF8)|((g&0xE0)>>5);
      b2=((g&0x1C)<<3)|((b&0xF8)>>3);
      for(i=0;i<j;i++) {
        wr_dat(b1);
        wr_dat(b2);
      }
      break;
    case COLOR_18BIT:
      for(i=0;i<j;i++) {
        wr_dat(r&0xFC);
        wr_dat(g&0xFC);
        wr_dat(b&0xFC);
      }
      break;
    case COLOR_24BIT:
       for(i=0;i<j;i++) {
        wr_dat(r);
        wr_dat(g);
        wr_dat(b);
      }
      break;
  }
}

void MC2PA8201_SetWindow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1) {

  wr_cmd(COLUMN_ADDRESS_SET);
  wr_dat(x0>>8);
  wr_dat(x0);
  wr_dat(x1>>8);
  wr_dat(x1);

  wr_cmd(PAGE_ADDRESS_SET);
  wr_dat(y0>>8);
  wr_dat(y0);
  wr_dat(y1>>8);
  wr_dat(y1);
}

void MC2PA8201_SetWriteWindow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1) {

  MC2PA8201_SetWindow(x0,y0,x1,y1);
  wr_cmd(MEMORY_WRITE);
}

void MC2PA8201_SetReadWindow(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1) {

  MC2PA8201_SetWindow(x0,y0,x1,y1);
  wr_cmd(MEMORY_READ);
}

void MC2PA8201_FillPixel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint32_t *color) {

  uint32_t i,j=(x1-x0+1)*(y1-y0+1);

  MC2PA8201_SetWriteWindow(x0,y0,x1,y1);

  switch(MC2PA8201_color_mode) {
    case COLOR_12BIT:
      for(i=0;i<j;i+=2) {
        wr_dat(((color[i]>>16)&0xF0)|((color[i]>>12)&0x0F));
        wr_dat(((color[i])&0xF0)|((color[i+1]>>16)&0x0F));
        wr_dat(((color[i+1]>>8)&0xF0)|((color[i+1]>>4)&0x0F));
      }
      break;
    case COLOR_16BIT:
      for(i=0;i<j;i++) {
        wr_dat(((color[i]>>16)&0xF8)|(((color[i]>>13)&0x07)));
        wr_dat(((color[i]>>5)&0xE0)|((color[i]>>3)&0x1F));
      }
      break;
    case COLOR_18BIT:
      for(i=0;i<j;i++) {
        wr_dat((color[i]>>16)&0xFC);
        wr_dat((color[i]>>8)&0xFC);
        wr_dat((color[i])&0xFC);
      }
      break;
    case COLOR_24BIT:
       for(i=0;i<j;i++) {
        wr_dat(color[i]>>16);
        wr_dat(color[i]>>8);
        wr_dat(color[i]);
      }
      break;
  }
}

void MC2PA8201_FillFromBuffer(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1, const uint8_t *data) {

  uint32_t i,j=(x1-x0+1)*(y1-y0+1);

  MC2PA8201_SetWriteWindow(x0,y0,x1,y1);

  switch(MC2PA8201_color_mode) {
    case COLOR_12BIT:
      // always writing even count of pixels
      for(i=0;i<j;i+=2) {
        wr_dat(*data++);
        wr_dat(*data++);
        wr_dat(*data++);
      }
      break;
    case COLOR_16BIT:
      for(i=0;i<j;i++) {
        wr_dat(*data++);
        wr_dat(*data++);
      }
      break;
    case COLOR_18BIT:
    case COLOR_24BIT:
      for(i=0;i<j;i++) {
        wr_dat(*data++);
        wr_dat(*data++);
        wr_dat(*data++);
      }
      break;
  }
}

void MC2PA8201_SetPixel(uint16_t x, uint16_t y, uint32_t color) {

  uint8_t r,g,b;

  MC2PA8201_SetWriteWindow(x,y,x,y);

  r=color>>16;
  g=color>>8;
  b=color;

  switch(MC2PA8201_color_mode) {
    case COLOR_12BIT:
      wr_dat((r&0xF0)|(g>>4));
      wr_dat((b&0xF0)|(r>>4));
      wr_dat((g&0xF0)|(b>>4));
      break;
    case COLOR_16BIT:
      wr_dat((r&0xF8)|((g>>5)&0x07));
      wr_dat(((g<<3)&0xE0)|((b>>3)&0x1F));
      break;
    case COLOR_18BIT:
      wr_dat(r&0xFC);
      wr_dat(g&0xFC);
      wr_dat(b&0xFC);
      break;
    case COLOR_24BIT:
      wr_dat(r);
      wr_dat(g);
      wr_dat(b);
      break;
  }
}

void MC2PA8201_SetScrollPosition(uint16_t pos) {
  wr_cmd(VERTICAL_SCROLLING_START_ADDRESS);
  wr_dat(pos>>8);
  wr_dat(pos);
}

void MC2PA8201_ScrollArea(uint16_t y,uint16_t pos) {
  uint16_t bfa=240-pos-y;
  wr_cmd(VERTICAL_SCROLLING_DEFINITION);
  wr_dat(y>>8);
  wr_dat(y);
  wr_dat(pos>>8);
  wr_dat(pos);
  wr_dat(bfa>>8);
  wr_dat(bfa);
}

void MC2PA8201_PartialArea(uint16_t y0,uint16_t y1) {
  wr_cmd(PARTIAL_AREA);
  wr_dat(y0>>8);
  wr_dat(y0);
  wr_dat(y1>>8);
  wr_dat(y1);
}

void MC2PA8201_PartialMode(void) {
  wr_cmd(PARTIAL_MODE_ON);
}

void MC2PA8201_NormalDisplayMode(void) {
  wr_cmd(NORMAL_DISPLAY_MODE_ON);
}

void MC2PA8201_DisplayOff(void) {
  wr_cmd(DISPLAY_OFF);
}

void MC2PA8201_DisplayOn(void) {
  wr_cmd(DISPLAY_ON);
}

void MC2PA8201_Sleep(void) {
  wr_cmd(DISPLAY_OFF);
  wr_cmd(SLEEP_IN);
  HAL_Delay(50);
}

void MC2PA8201_Wakeup(void) {
  wr_cmd(SLEEP_OUT);
  HAL_Delay(1200);
  wr_cmd(DISPLAY_ON);
  HAL_Delay(50);
}

void MC2PA8201_Gamma(GAMMA_VALUE val) {
  wr_reg(GAMMA_SET,val);
}

void MC2PA8201_IdleModeOn(void) {
  wr_cmd(IDLE_MODE_ON);
}

void MC2PA8201_IdleModeOff(void) {
  wr_cmd(IDLE_MODE_OFF);
}

void MC2PA8201_DisplayInversionOn(void) {
  wr_cmd(DISPLAY_INVERSION_ON);
}

void MC2PA8201_DisplayInversionOff(void) {
  wr_cmd(DISPLAY_INVERSION_OFF);
}

void MC2PA8201_TearingEffectLineOn(TE val) {
  wr_cmd(TEARING_EFFECT_LINE_ON);
  wr_dat(val);
}

void MC2PA8201_TearingEffectLineOff(void) {
  wr_cmd(TEARING_EFFECT_LINE_OFF);
}

uint16_t MC2PA8201_GetWidth(void) {

  uint16_t ret;

  switch(MC2PA8201_orientation_mode){
    case ORIENTATION_LANDSCAPE:
    case ORIENTATION_LANDSCAPE_REV:
      ret=display_width;
    break;
    case ORIENTATION_PORTRAIT:
    case ORIENTATION_PORTRAIT_REV:
      ret=display_height;
    break;
  }
  return ret;
}

uint16_t MC2PA8201_GetHeight(void) {

  uint16_t ret;

  switch(MC2PA8201_orientation_mode){
    case ORIENTATION_LANDSCAPE:
    case ORIENTATION_LANDSCAPE_REV:
      ret=display_height;
    break;
    case ORIENTATION_PORTRAIT:
    case ORIENTATION_PORTRAIT_REV:
      ret=display_width;
    break;
  }
  return ret;
}

void MC2PA8201_ReadRegister(uint8_t reg,uint8_t length,uint8_t *val) {

  uint8_t i;

  // first read is dummy read
  val[0]=rd_reg(reg);

  for(i=0;i<length;i++) {
    val[i]=rd_dat();
  }
}

void MC2PA8201_ReadMemory(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint8_t *data) {

  uint8_t i,j=(x1-x0+1)*(y1-y0+1);

  MC2PA8201_SetReadWindow(x0,y0,x1,y1);
  // first read is dummy read
  *data=rd_dat();

  switch(MC2PA8201_color_mode) {
    case COLOR_12BIT:
      // always reading even count of pixels
      for(i=0;i<j;i+=2) {
        *data++=rd_dat();
        *data++=rd_dat();
        *data++=rd_dat();
      }
      break;
    case COLOR_16BIT:
      for(i=0;i<j;i++) {
        *data++=rd_dat();
        *data++=rd_dat();
      }
      break;
    case COLOR_18BIT:
    case COLOR_24BIT:
      for(i=0;i<j;i++) {
        *data++=rd_dat();
        *data++=rd_dat();
        *data++=rd_dat();
      }
      break;
  }
}

void MC2PA8201_On()
{
  wr_cmd(DISPLAY_ON);
}

void MC2PA8201_Off()
{
  wr_cmd(DISPLAY_OFF);
}

void MC2PA8201_BacklightOff()
{
  HAL_GPIO_WritePin(LCD_BACKLIGHT_EN_GPIO_Port, LCD_BACKLIGHT_EN_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_BACKLIGHT_PWM_GPIO_Port, LCD_BACKLIGHT_PWM_Pin, GPIO_PIN_RESET);
}

void MC2PA8201_BacklightOn()
{
  HAL_GPIO_WritePin(LCD_BACKLIGHT_EN_GPIO_Port, LCD_BACKLIGHT_EN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LCD_BACKLIGHT_PWM_GPIO_Port, LCD_BACKLIGHT_PWM_Pin, GPIO_PIN_SET);
}

void MC2PA8201_SetBGROrder(uint8_t val) {
  wr_cmd(MEMORY_ACCESS_CONTROL);
  wr_dat(val);
}

static void MC2PA8201_IRQHandler(void)
{
	signal_vsync();
	//osEventFlagsSet(keypadEventHandle, EVENT_KEYBOARD_INT);
  //HAL_EXTI_IRQHandler(&hRADIO_DIO_exti[DIO]);
}
