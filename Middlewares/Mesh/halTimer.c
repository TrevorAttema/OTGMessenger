/**
 * \file halTimer.c
 *
 * \brief ATmega1281 timer implementation
 *
 * Copyright (C) 2012-2014, Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 * Modification and other use of this code is subject to Atmel's Limited
 * License Agreement (license.txt).
 *
 * $Id: halTimer.c 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdint.h>
#include "hal.h"
#include "halTimer.h"

/*- Definitions ------------------------------------------------------------*/
//#define TIMER_PRESCALER     8

/*- Variables --------------------------------------------------------------*/
//volatile uint8_t halTimerIrqCount;

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*****************************************************************************/
void HAL_TimerInit(void)
{
//  halTimerIrqCount = 0;
//
//  OCR4A = ((F_CPU / 1000ul) / TIMER_PRESCALER) * HAL_TIMER_INTERVAL;
//  TCCR4B = (1 << WGM12);              // CTC mode
//  TCCR4B |= (1 << CS11);              // Prescaler 8
//  TIMSK4 |= (1 << OCIE4A);            // Enable TC4 interrupt
}

/*************************************************************************//**
*****************************************************************************/
void HAL_TimerDelay(uint16_t us)
{
//  PRAGMA(diag_suppress=Pa082);
//
//  OCR4B = TCNT4 + us;
//  if (OCR4B > OCR4A)
//    OCR4B -= OCR4A;
//
//  TIFR4 = (1 << OCF4B);
//  while (0 == (TIFR4 & (1 << OCF4B)));
//
//  PRAGMA(diag_default=Pa082);
}

/*************************************************************************//**
*****************************************************************************/
//ISR(TIMER4_COMPA_vect)
//{
////  halTimerIrqCount++;
//}
