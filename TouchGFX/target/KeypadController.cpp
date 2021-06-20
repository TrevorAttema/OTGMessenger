/*
 * KeypadController.cpp
 *
 *  Created on: 14 Jun 2021
 *      Author: Trevor Attema
 */
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"

#include <KeypadController.h>


extern osMessageQueueId_t keypadQueueHandle;

namespace touchgfx
{

	bool I2CKeypadController::sample(uint8_t& key)
	{

		if (osMessageQueueGet(keypadQueueHandle, &key, NULL, 0) == osOK)
		{
			return true;
		}

		return false;
	}
} /* namespace touchgfx */
