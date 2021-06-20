/*
 * SX1509B.h
 *
 *  Created on: 22 May 2021
 *      Author: Trevor Attema
 */

#ifndef BSP_INC_SX1509B_H_
#define BSP_INC_SX1509B_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>


#define KEY_NOVAL			-1

#define KEY_STATUS_RESET		((uint8_t)0)
#define KEY_STATUS_SHIFT		((uint8_t)0b00000001)
#define KEY_STATUS_FUNCT		((uint8_t)0b00000010)

typedef enum {
	KEY_SW_L			= ((uint8_t)210),
	KEY_SW_R 			= ((uint8_t)211),
	KEY_CHR 			= ((uint8_t)212),
	KEY_SEND 			= ((uint8_t)213),
	KEY_BACKSPACE = ((uint8_t)214),
	KEY_SHIFT 		= ((uint8_t)215),
	KEY_CALENDAR 	= ((uint8_t)216),
	KEY_HOME 			= ((uint8_t)217),
	KEY_ENTER 		= ((uint8_t)218),
	KEY_CTRL 			= ((uint8_t)219),
	KEY_SPACE 		= ((uint8_t)220),
	KEY_FN 				= ((uint8_t)221),
	KEY_CONTACTS 	= ((uint8_t)222),
	KEY_MAIL 			= ((uint8_t)223),
	KEY_UP 				= ((uint8_t)224),
	KEY_DOWN 			= ((uint8_t)225),
	KEY_LEFT 			= ((uint8_t)226),
	KEY_RIGHT 		= ((uint8_t)227),
	KEY_SELECT 		= ((uint8_t)228),
	KEY_POWER 		= ((uint8_t)229)
} extended_keys_t;


typedef struct ascii_value
{
	uint16_t					scancode;
	uint16_t					key;
	uint8_t						shift;
	uint8_t						func;
} ascii_value_t;


#define SX1509B_I2C							((uint16_t)0x71 << 1)
#define SX1509B_RegCount 				0x80

#define SX159B_KEYDATA1_REG			((uint8_t)0x27)
#define SX159B_KEYDATA2_REG			((uint8_t)0x28)

#define SX159B_REGPULLUPB				((uint8_t)0x06)
#define SX159B_REGPULLUPA				((uint8_t)0x07)

#define SX159B_REGOPENDRAINB		((uint8_t)0x0A)
#define SX159B_REGOPENDRAINA		((uint8_t)0x0B)

#define SX159B_REGDIRB					((uint8_t)0x0E)
#define SX159B_REGDIRA					((uint8_t)0x0F)

#define SX159B_REGDEBOUNCECFG 	((uint8_t)0x22)

#define SX159B_REGDEBOUNCEB 		((uint8_t)0x23)
#define SX159B_REGDEBOUNCEA 		((uint8_t)0x24)

#define SX159B_REGKEYCONFIG1  	((uint8_t)0x25)
#define SX159B_REGKEYCONFIG2  	((uint8_t)0x26)

#define SX159B_REGCLOCK 				((uint8_t)0x1E)

#define SX159B_REGINTB 					((uint8_t)0x12)
#define SX159B_REGINTA 					((uint8_t)0x13)

#define SX159B_ROWBITS_DIR			((uint8_t)0b00000001)						// Rows are output bits. 0 = output. 1 = input
#define SX159B_ROWBITS_DRAIN		((uint8_t)0b11111110)						// 1 = open drain. 0 = push-pull

#define SX159B_COLUMNBITS_DIR		((uint8_t)0b11111111)						// Columns are input bits. 0 = output. 1 = input
#define SX159B_COLBITS_PULLUP		((uint8_t)0b11111111)						// 1 = pull-up enabled. 0 pull-up disabled
#define SX159B_COLBITS_DEBOUNCE	((uint8_t)0b11111111)						// 1 = debounce enabled. 0 debounce disabled

#define SX159B_DEBOUNCE   			((uint8_t)0b00000010)						// 0b00000100	= debounce time
#define SX159B_SCAN   					((uint8_t)0b00000100)						// 0b00000100	= scan time. Must be above debounce time
#define SX159B_ROWCOL_SCAN			((uint8_t)0b00111111)						// Scan all rows and columns

#define SX159B_INTERNCLOCK			((uint8_t)0b01000000)						// use the internal 2Mhz clock

#define SX159B_LEDBIT						((uint8_t)0b00000001)

//#define SX1509B_REG_SCR	((uint16_t)0x0)									// Source Control Register

HAL_StatusTypeDef SX1509B_WriteReg(uint8_t reg, uint8_t value);
HAL_StatusTypeDef SX1509B_ReadReg(uint8_t reg, uint8_t * value);
HAL_StatusTypeDef SX1509B_ReadRegs(uint8_t regs[], uint8_t size);
HAL_StatusTypeDef SX1509B_ReadKeyRow(uint8_t * row);
HAL_StatusTypeDef SX1509B_ReadKeyColumn(uint8_t * column);

HAL_StatusTypeDef SX1509B_InitKeypad();
HAL_StatusTypeDef SX1509B_NavLedSet(bool state);
void 							SX1509B_KeyboardLedSet(GPIO_PinState state);
HAL_StatusTypeDef SX1509B_ReadScanCode( uint16_t * code );
uint8_t					SX1509B_TranslateScanCode( uint16_t code);

#ifdef __cplusplus
}
#endif


#endif /* BSP_INC_SX1509B_H_ */
