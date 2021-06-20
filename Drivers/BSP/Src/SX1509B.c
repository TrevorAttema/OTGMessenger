/*
 * SX1509B.c
 *
 *  Created on: 22 May 2021
 *      Author: Trevor Attema
 */
#include <stdlib.h>

#include <i2c.h>
#include <gpio.h>
#include <SX1509B.h>

ascii_value_t ascii_values[] = {
		{ 0x0201, 'a', 'A', KEY_NOVAL},
		{ 0x0410, 'b', 'B', 8},
		{ 0x0408, 'c', 'C', KEY_NOVAL},
		{ 0x0202, 'd', 'D', KEY_NOVAL},
		{ 0x0104, 'e', 'E', KEY_NOVAL},
		{ 0x0208, 'f', 'F', 4},
		{ 0x0210, 'g', 'G', 5},
		{ 0x0220, 'h', 'H', 6},
		{ 0x0801, 'i', 'I', '+'},
		{ 0x0240, 'j', 'J', '#'},
		{ 0x1001, 'k', 'K', '-'},
		{ 0x1002, 'l', 'L', KEY_NOVAL},
		{ 0x0440, 'm', 'M', 0},
		{ 0x0420, 'n', 'N', 9},
		{ 0x0802, 'o', 'O', '='},
		{ 0x0804, 'p', 'P', KEY_NOVAL},
		{ 0x0101, 'q', 'Q', KEY_NOVAL},
		{ 0x0108, 'r', 'R', 1},
		{ 0x0204, 's', 'S', KEY_NOVAL},
		{ 0x0110, 't', 'T', 2},
		{ 0x0140, 'u', 'U', '*'},
		{ 0x0404, 'v', 'V', 7},
		{ 0x0102, 'w', 'W', KEY_NOVAL},
		{ 0x0402, 'x', 'X', KEY_NOVAL},
		{ 0x0120, 'y', 'Y', 3},
		{ 0x0401, 'z', 'Z', KEY_NOVAL},
		{ 0x4001, '@', '(', KEY_NOVAL},
		{ 0x4002, ' ', KEY_NOVAL, KEY_NOVAL},
		{ 0x2001, ',', ';', KEY_NOVAL},
		{ 0x2002, '.', ':', KEY_NOVAL},
		{ 0x0810, '\'', '&', KEY_NOVAL},
		{ 0x2020, '?', '!', KEY_NOVAL},
		{ 0x1010, '/', ')', KEY_NOVAL},
		{ 0x0820, KEY_SW_L, KEY_NOVAL, KEY_NOVAL},			// SW L
		{ 0x2008, KEY_SW_R, KEY_NOVAL, KEY_NOVAL},			// SW R
		{ 0x0808, KEY_CHR, KEY_NOVAL, KEY_NOVAL},
		{ 0x0840, KEY_SEND, KEY_NOVAL, KEY_NOVAL},
		{ 0x1004, KEY_BACKSPACE, KEY_NOVAL, KEY_NOVAL},
		{ 0x1008, KEY_SHIFT, KEY_NOVAL, KEY_NOVAL},
		{ 0x1020, KEY_CALENDAR, KEY_NOVAL, KEY_NOVAL},
		{ 0x1040, KEY_HOME, KEY_NOVAL, KEY_NOVAL},
		{ 0x2004, KEY_ENTER, KEY_NOVAL, KEY_NOVAL},
		{ 0x2010, KEY_CTRL, KEY_NOVAL, KEY_NOVAL},
		{ 0x4002, KEY_SPACE, KEY_NOVAL, KEY_NOVAL},
		{ 0x4004, KEY_FN, KEY_NOVAL, KEY_NOVAL},
		{ 0x4008, KEY_CONTACTS, KEY_NOVAL, KEY_NOVAL},
		{ 0x4010, KEY_MAIL, KEY_NOVAL, KEY_NOVAL},
		{ 0x8001, KEY_UP, KEY_NOVAL, KEY_NOVAL},
		{ 0x8002, KEY_DOWN, KEY_NOVAL, KEY_NOVAL},
		{ 0x8004, KEY_LEFT, KEY_NOVAL, KEY_NOVAL},
		{ 0x8008, KEY_RIGHT, KEY_NOVAL, KEY_NOVAL},
		{ 0x8010, KEY_SELECT, KEY_NOVAL, KEY_NOVAL},
		{ 0x0000, KEY_POWER, KEY_NOVAL, KEY_NOVAL}
};

static int ascii_compare  (const void * a, const void * b);

HAL_StatusTypeDef SX1509B_WriteReg(uint8_t reg, uint8_t value) {
	HAL_StatusTypeDef status;

	status = HAL_I2C_Mem_Write( &hi2c1, SX1509B_I2C, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 10000);

	return status;
}

HAL_StatusTypeDef SX1509B_ReadReg(uint8_t reg, uint8_t * value) {

	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Read(&hi2c1, SX1509B_I2C, reg, I2C_MEMADD_SIZE_8BIT, value, 1, 10000);

	return status;
}

HAL_StatusTypeDef SX1509B_ReadRegs(uint8_t regs[], uint8_t size) {

	HAL_StatusTypeDef status;
	uint8_t index = 0;

	for(; index < size; index++) {
		if((status = SX1509B_ReadReg(index, &regs[index])) != HAL_OK)
			return status;
	}

	return HAL_OK;
}

HAL_StatusTypeDef SX1509B_ReadKeyRow(uint8_t * row) {
	HAL_StatusTypeDef status = HAL_OK;
	status = SX1509B_ReadReg(SX159B_KEYDATA1_REG, row);
	*row = ~*row;

	return status;
}

HAL_StatusTypeDef SX1509B_ReadKeyColumn(uint8_t * column) {
	HAL_StatusTypeDef status = HAL_OK;
	status = SX1509B_ReadReg(SX159B_KEYDATA2_REG, column);
	*column = ~*column;

	return status;
}

HAL_StatusTypeDef SX1509B_InitKeypad() {
	HAL_StatusTypeDef status = HAL_OK;

	// turn on the internal clock
	status |= SX1509B_WriteReg(SX159B_REGCLOCK, SX159B_INTERNCLOCK);

	// row config
	status |= SX1509B_WriteReg(SX159B_REGDIRA, SX159B_ROWBITS_DIR);
	status |= SX1509B_WriteReg(SX159B_REGOPENDRAINA, SX159B_ROWBITS_DRAIN);

	// column config
	status |= SX1509B_WriteReg(SX159B_REGDIRB, SX159B_COLUMNBITS_DIR);
	status |= SX1509B_WriteReg(SX159B_REGPULLUPB, SX159B_COLBITS_PULLUP);
	status |= SX1509B_WriteReg(SX159B_REGDEBOUNCECFG, SX159B_DEBOUNCE);
	status |= SX1509B_WriteReg(SX159B_REGDEBOUNCEB, SX159B_COLBITS_DEBOUNCE);

	// start keypad engine
	status |= SX1509B_WriteReg(SX159B_REGKEYCONFIG1, SX159B_SCAN);
	status |= SX1509B_WriteReg(SX159B_REGKEYCONFIG2, SX159B_ROWCOL_SCAN);

	qsort(ascii_values, sizeof(ascii_values)/sizeof(ascii_values[0]), sizeof(ascii_value_t), ascii_compare);

	return status;
}

HAL_StatusTypeDef SX1509B_NavLedSet(bool state) {
	HAL_StatusTypeDef status = HAL_OK;

	uint8_t reg = 0;

	status = SX1509B_ReadReg( SX159B_REGDIRA, &reg);

	if( state == true )
		SET_BIT(reg, SX159B_LEDBIT);
	else
		CLEAR_BIT(reg, SX159B_LEDBIT);

	status |= SX1509B_WriteReg( SX159B_REGDIRA, reg);

	return status;
}

void SX1509B_KeyboardLedSet(GPIO_PinState state) {

	HAL_GPIO_WritePin(GPIOC, KB_LED_EN_Pin, state);

}

HAL_StatusTypeDef SX1509B_ReadScanCode(uint16_t * code) {

	HAL_StatusTypeDef status = HAL_OK;
	uint8_t row, column;

	status = SX1509B_ReadKeyRow(&row);
	status |= SX1509B_ReadKeyColumn(&column);

	*code = 0;
	if(status == HAL_OK && row > 0 && column > 0) {

		column = column >>1;	// update SX1509B bit order to adjust Nokia keyboard matrix output

		// convert row and column into 16 bit code
		*code = row << 8;
		*code |= column;
	}

	return status;
}

static int ascii_compare (const void * a, const void * b)
{

	ascii_value_t * entryA = (ascii_value_t*)a;
	ascii_value_t * entryB = (ascii_value_t*)b;

  return entryA->scancode - entryB->scancode;
}

uint8_t key_status = KEY_STATUS_RESET;

uint8_t  SX1509B_TranslateScanCode( uint16_t code)
{
	ascii_value_t * scan_code = bsearch(&code, ascii_values, sizeof(ascii_values)/sizeof(ascii_values[0]), sizeof(ascii_value_t), ascii_compare);

	if((key_status & KEY_STATUS_SHIFT) == KEY_STATUS_SHIFT) {
		key_status = KEY_STATUS_RESET;
		return scan_code->shift;
	}

	if((key_status & KEY_STATUS_FUNCT) == KEY_STATUS_FUNCT) {
		key_status = KEY_STATUS_RESET;
		return scan_code->func;
	}

	switch(scan_code->key){
		case KEY_SHIFT:
			key_status = KEY_STATUS_SHIFT;
			return KEY_SHIFT;
		case KEY_FN:
			key_status = KEY_STATUS_FUNCT;
			return KEY_FN;
	}

	return scan_code->key;
}
