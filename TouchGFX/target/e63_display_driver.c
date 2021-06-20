/*
 * e63_display_driver.c
 *
 *  Created on: Jun 11, 2021
 *      Author: Trevor Attema
 */


#include "e63_display_driver.h"
#include "MC2PA8201.h"

static int isTransmittingData = 0;

int touchgfxDisplayDriverTransmitActive()
{
	return isTransmittingData;
}

void touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	isTransmittingData = 1;
	MC2PA8201_FillFromBuffer(x,y, x+w-1, y+h-1, pixels);
	isTransmittingData = 0;

	DisplayDriver_TransferCompleteCallback();
}

int touchgfxDisplayDriverShouldTransferBlock(uint16_t bottom)
{
	return 1;
}

/*
int transmitActive()
{
  return touchgfxDisplayDriverTransmitActive();
}

*
 * Check if a Frame Buffer Block ending at bottom should be transferred.

int shouldTransferBlock(uint16_t bottom)
{
  return touchgfxDisplayDriverShouldTransferBlock(bottom);
}

*
 * Transmit a Frame Buffer Block.

void transmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  touchgfxDisplayDriverTransmitBlock(pixels, x, y, w, h);
}
*/
