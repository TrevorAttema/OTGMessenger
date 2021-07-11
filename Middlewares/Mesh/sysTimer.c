/**
 * \file sysTimer.c
 *
 * \brief System timer implementation
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
 * $Id: sysTimer.c 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include "hal.h"
#include "halTimer.h"
#include "sysTimer.h"
#include "timer.h"

#include "utilities_conf.h"

/*- Prototypes -------------------------------------------------------------*/
static void placeTimer(SYS_Timer_t *timer);

/*- Variables --------------------------------------------------------------*/
static SYS_Timer_t *timers;

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*****************************************************************************/
void SYS_TimerInit(void)
{
  timers = NULL;
}

/*************************************************************************//**
*****************************************************************************/
void SYS_TimerStart(SYS_Timer_t *timer)
{
  if (!SYS_TimerStarted(timer))
    placeTimer(timer);
}

/*************************************************************************//**
*****************************************************************************/
void SYS_TimerStop(SYS_Timer_t *timer)
{
  SYS_Timer_t *prev = NULL;

  for (SYS_Timer_t *t = timers; t; t = t->next)
  {
    if (t == timer)
    {
      if (prev)
        prev->next = t->next;
      else
        timers = t->next;

      if (t->next)
        t->next->timeout += timer->timeout;

      break;
    }
    prev = t;
  }
}

/*************************************************************************//**
*****************************************************************************/
bool SYS_TimerStarted(SYS_Timer_t *timer)
{
  for (SYS_Timer_t *t = timers; t; t = t->next)
    if (t == timer)
      return true;
  return false;
}

/*************************************************************************//**
*****************************************************************************/
void SYS_TimerTaskHandler(void)
{
  uint32_t elapsed;
  uint8_t cnt;

  if (0 == halTimerIrqCount)
    return;

  UTIL_TIMER_ENTER_CRITICAL_SECTION();
    cnt = halTimerIrqCount;
    halTimerIrqCount = 0;
    UTILS_EXIT_CRITICAL_SECTION();

  elapsed = cnt * HAL_TIMER_INTERVAL;

  while (timers && (timers->timeout <= elapsed))
  {
    SYS_Timer_t *timer = timers;

    elapsed -= timers->timeout;
    timers = timers->next;
    if (SYS_TIMER_PERIODIC_MODE == timer->mode)
      placeTimer(timer);
    timer->handler(timer);
  }

  if (timers)
    timers->timeout -= elapsed;
}

/*************************************************************************//**
*****************************************************************************/
static void placeTimer(SYS_Timer_t *timer)
{
//  if (timers)
//  {
//    SYS_Timer_t *prev = NULL;
//    uint32_t timeout = timer->interval;
//
//    for (SYS_Timer_t *t = timers; t; t = t->next)
//    {
//      if (timeout < t->timeout)
//      {
//         t->timeout -= timeout;
//         break;
//      }
//      else
//        timeout -= t->timeout;
//
//      prev = t;
//    }
//
//    timer->timeout = timeout;
//
//    if (prev)
//    {
//      timer->next = prev->next;
//      prev->next = timer;
//    }
//    else
//    {
//      timer->next = timers;
//      timers = timer;
//    }
//  }
//  else
//  {
//    timer->next = NULL;
//    timer->timeout = timer->interval;
//    timers = timer;
//  }
}
