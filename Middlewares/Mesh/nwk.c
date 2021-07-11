/**
 * \file nwk.c
 *
 * \brief Network layer management functions implementation
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
 * $Id: nwk.c 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "phy.h"
#include "sysConfig.h"
#include "nwkRx.h"
#include "nwkTx.h"
#include "nwkGroup.h"
#include "nwkFrame.h"
#include "nwkRoute.h"
#include "nwkSecurity.h"
#include "nwkRouteDiscovery.h"

/*- Variables --------------------------------------------------------------*/
NwkIb_t nwkIb;

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
  @brief Initializes all network layer modules
*****************************************************************************/
void NWK_Init(void)
{
  nwkIb.nwkSeqNum = 0;
  nwkIb.macSeqNum = 0;
  nwkIb.addr = 0;
  nwkIb.lock = 0;

  for (uint8_t i = 0; i < NWK_ENDPOINTS_AMOUNT; i++)
    nwkIb.endpoint[i] = NULL;

  nwkTxInit();
  nwkRxInit();
  nwkFrameInit();
  nwkDataReqInit();

#ifdef NWK_ENABLE_ROUTING
  nwkRouteInit();
#endif

#ifdef NWK_ENABLE_SECURITY
  nwkSecurityInit();
#endif

#ifdef NWK_ENABLE_MULTICAST
  nwkGroupInit();
#endif

#ifdef NWK_ENABLE_ROUTE_DISCOVERY
  nwkRouteDiscoveryInit();
#endif
}

/*************************************************************************//**
  @brief Sets network address of the node
  @param[in] addr Adddress to set
*****************************************************************************/
void NWK_SetAddr(uint16_t addr)
{
  nwkIb.addr = addr;
  PHY_SetShortAddr(addr);
}

/*************************************************************************//**
  @brief Sets network identifier (PAN) of the node
  @param[in] panId PAN ID to set
*****************************************************************************/
void NWK_SetPanId(uint16_t panId)
{
  nwkIb.panId = panId;
  PHY_SetPanId(panId);
}

/*************************************************************************//**
  @brief Registers callback @a ind for the endpoint @a endpoint
  @param[in] id Endpoint index (1-15)
  @param[in] handler Pointer to the callback function
*****************************************************************************/
void NWK_OpenEndpoint(uint8_t id, bool (*handler)(NWK_DataInd_t *ind))
{
  nwkIb.endpoint[id] = handler;
}

/*************************************************************************//**
  @brief Checks if network layer is ready for sleep
  @return @c true if network layer is ready for sleep or @c false otherwise
*****************************************************************************/
bool NWK_Busy(void)
{
  return nwkIb.lock > 0;
}

/*************************************************************************//**
  @brief Increases the lock counter and sets a busy state
*****************************************************************************/
void NWK_Lock(void)
{
  nwkIb.lock++;
}

/*************************************************************************//**
  @brief Decreases the lock counter and sets a free state if counter reaches 0
*****************************************************************************/
void NWK_Unlock(void)
{
  nwkIb.lock--;
}

/*************************************************************************//**
  @brief Puts network layer to a sleeping state
*****************************************************************************/
void NWK_SleepReq(void)
{
  PHY_Sleep();
}

/*************************************************************************//**
  @brief Puts network layer to an active state
*****************************************************************************/
void NWK_WakeupReq(void)
{
  PHY_Wakeup();
}

/*************************************************************************//**
  @brief Calculates linearized value for the given value of the LQI
  @param[in] lqi LQI value as provided by the transceiver
  @return linearized value directly proportional to the probability of delivery
*****************************************************************************/
uint8_t NWK_LinearizeLqi(uint8_t lqi)
{
  const uint8_t val[] = { 3, 8, 26, 64, 128, 190, 230, 247, 252 };
  uint8_t cl = 25;

  for (uint8_t i = 0; i < sizeof(val); i++)
  {
    if (lqi < cl)
      return val[i];
    cl += 25;
  }

  return 255;
}

/*************************************************************************//**
  @brief Network layer task handler
*****************************************************************************/
void NWK_TaskHandler(void)
{
  nwkRxTaskHandler();
  nwkTxTaskHandler();
  nwkDataReqTaskHandler();
#ifdef NWK_ENABLE_SECURITY
  nwkSecurityTaskHandler();
#endif
}
