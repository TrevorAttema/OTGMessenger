/*
 * main.cpp
 *
 *  Created on: 12 Jun 2021
 *      Author: Trevor Attema
 */

#include "main2.h"


/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#ifdef __cplusplus
extern "C" {
#endif

	#include "app_touchgfx.h"

#ifdef __cplusplus
}
#endif

extern "C" void gfx_thread(void);

void gfx_thread()
{
	MX_TouchGFX_Process();
}

void post_keypress(uint8_t key)
{

}
