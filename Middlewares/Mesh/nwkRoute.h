/**
 * \file nwkRoute.h
 *
 * \brief Routing interface
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
 * $Id: nwkRoute.h 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

#ifndef _NWK_ROUTE_H_
#define _NWK_ROUTE_H_

/*- Includes ---------------------------------------------------------------*/
#include <stdint.h>
#include "sysTypes.h"
#include "nwkRx.h"
#include "nwkFrame.h"

/*- Definitions ------------------------------------------------------------*/
#define NWK_ROUTE_UNKNOWN            0xffff
#define NWK_ROUTE_NON_ROUTING        0x8000

#ifdef NWK_ENABLE_ROUTING

/*- Types ------------------------------------------------------------------*/
typedef struct NWK_RouteTableEntry_t
{
  uint8_t  fixed     : 1;
  uint8_t  multicast : 1;
  uint8_t  reserved  : 2;
  uint8_t  score     : 4;
  uint16_t dstAddr;
  uint16_t nextHopAddr;
  uint8_t  rank;
  uint8_t  lqi;
} NWK_RouteTableEntry_t;

/*- Prototypes -------------------------------------------------------------*/
NWK_RouteTableEntry_t *NWK_RouteFindEntry(uint16_t dst, uint8_t multicast);
NWK_RouteTableEntry_t *NWK_RouteNewEntry(void);
void NWK_RouteFreeEntry(NWK_RouteTableEntry_t *entry);
uint16_t NWK_RouteNextHop(uint16_t dst, uint8_t multicast);
NWK_RouteTableEntry_t *NWK_RouteTable(void);

void nwkRouteInit(void);
void nwkRouteRemove(uint16_t dst, uint8_t multicast);
void nwkRouteFrameReceived(NwkFrame_t *frame);
void nwkRouteFrameSent(NwkFrame_t *frame);
void nwkRoutePrepareTx(NwkFrame_t *frame);
void nwkRouteFrame(NwkFrame_t *frame);
bool nwkRouteErrorReceived(NWK_DataInd_t *ind);
void nwkRouteUpdateEntry(uint16_t dst, uint8_t multicast, uint16_t nextHop, uint8_t lqi);

#endif // NWK_ENABLE_ROUTING

#endif // _NWK_ROUTE_H_
