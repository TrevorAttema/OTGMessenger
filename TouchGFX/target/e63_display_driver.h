/*
 * e63_display_driver.h
 *
 *  Created on: Jun 11, 2021
 *      Author: Trevor Attema
 */

#ifndef TARGET_E63_DISPLAY_DRIVER_H_
#define TARGET_E63_DISPLAY_DRIVER_H_

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif


int touchgfxDisplayDriverTransmitActive();
void touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
int touchgfxDisplayDriverShouldTransferBlock(uint16_t bottom);
void signal_vsync(void);

#ifdef __cplusplus
}
#endif

#endif /* TARGET_E63_DISPLAY_DRIVER_H_ */
