/**
 * \file nwkRoute.c
 *
 * \brief Routing implementation
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
 * $Id: nwkRoute.c 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "sysTypes.h"
#include "sysConfig.h"
#include "nwk.h"
#include "nwkTx.h"
#include "nwkFrame.h"
#include "nwkRoute.h"
#include "nwkGroup.h"
#include "nwkCommand.h"
#include "nwkRouteDiscovery.h"

#ifdef NWK_ENABLE_ROUTING

/*- Definitions ------------------------------------------------------------*/
#define NWK_ROUTE_MAX_RANK         255
#define NWK_ROUTE_DEFAULT_RANK     128

/*- Prototypes -------------------------------------------------------------*/
static void nwkRouteSendRouteError(uint16_t src, uint16_t dst, uint8_t multicast);
static void nwkRouteNormalizeRanks(void);

/*- Variables --------------------------------------------------------------*/
static NWK_RouteTableEntry_t nwkRouteTable[NWK_ROUTE_TABLE_SIZE];

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
  @brief Initializes the Routing module
*****************************************************************************/
void nwkRouteInit(void)
{
  for (uint8_t i = 0; i < NWK_ROUTE_TABLE_SIZE; i++)
  {
    nwkRouteTable[i].dstAddr = NWK_ROUTE_UNKNOWN;
    nwkRouteTable[i].fixed = 0;
    nwkRouteTable[i].rank = 0;
  }
}

/*************************************************************************//**
*****************************************************************************/
NWK_RouteTableEntry_t *NWK_RouteFindEntry(uint16_t dst, uint8_t multicast)
{
  for (uint8_t i = 0; i < NWK_ROUTE_TABLE_SIZE; i++)
  {
    if (nwkRouteTable[i].dstAddr == dst &&
        nwkRouteTable[i].multicast == multicast)
      return &nwkRouteTable[i];
  }

  return NULL;
}

/*************************************************************************//**
*****************************************************************************/
NWK_RouteTableEntry_t *NWK_RouteNewEntry(void)
{
  NWK_RouteTableEntry_t *iter = nwkRouteTable;
  NWK_RouteTableEntry_t *entry = NULL;

  for (uint8_t i = 0; i < NWK_ROUTE_TABLE_SIZE; i++, iter++)
  {
    if (iter->fixed)
      continue;

    if (0 == iter->rank)
    {
      entry = iter;
      break;
    }

    if (NULL == entry || iter->rank < entry->rank)
      entry = iter;
  }

  entry->multicast = 0;
  entry->score = NWK_ROUTE_DEFAULT_SCORE;
  entry->rank = NWK_ROUTE_DEFAULT_RANK;

  return entry;
}

/*************************************************************************//**
*****************************************************************************/
void NWK_RouteFreeEntry(NWK_RouteTableEntry_t *entry)
{
  if (entry->fixed)
    return;
  entry->dstAddr = NWK_ROUTE_UNKNOWN;
  entry->rank = 0;
}

/*************************************************************************//**
*****************************************************************************/
uint16_t NWK_RouteNextHop(uint16_t dst, uint8_t multicast)
{
  NWK_RouteTableEntry_t *entry;

  entry = NWK_RouteFindEntry(dst, multicast);
  if (entry)
    return entry->nextHopAddr;

  return NWK_ROUTE_UNKNOWN;
}

/*************************************************************************//**
*****************************************************************************/
NWK_RouteTableEntry_t *NWK_RouteTable(void)
{
  return nwkRouteTable;
}

/*************************************************************************//**
*****************************************************************************/
void nwkRouteUpdateEntry(uint16_t dst, uint8_t multicast, uint16_t nextHop, uint8_t lqi)
{
  NWK_RouteTableEntry_t *entry;

  entry = NWK_RouteFindEntry(dst, multicast);

  if (NULL == entry)
    entry = NWK_RouteNewEntry();

  entry->dstAddr = dst;
  entry->nextHopAddr = nextHop;
  entry->multicast = multicast;
  entry->score = NWK_ROUTE_DEFAULT_SCORE;
  entry->rank = NWK_ROUTE_DEFAULT_RANK;
  entry->lqi = lqi;
}

/*************************************************************************//**
*****************************************************************************/
void nwkRouteRemove(uint16_t dst, uint8_t multicast)
{
  NWK_RouteTableEntry_t *entry;

  entry = NWK_RouteFindEntry(dst, multicast);
  if (entry)
    NWK_RouteFreeEntry(entry);
}

/*************************************************************************//**
*****************************************************************************/
void nwkRouteFrameReceived(NwkFrame_t *frame)
{
#ifndef NWK_ENABLE_ROUTE_DISCOVERY
  NwkFrameHeader_t *header = &frame->header;
  NWK_RouteTableEntry_t *entry;

  if ((header->macSrcAddr & NWK_ROUTE_NON_ROUTING) &&
      (header->macSrcAddr != header->nwkSrcAddr))
    return;

  if (NWK_BROADCAST_PANID == header->macDstPanId)
    return;

  entry = NWK_RouteFindEntry(header->nwkSrcAddr, false);

  if (entry)
  {
    bool discovery = (NWK_BROADCAST_ADDR == header->macDstAddr &&
        nwkIb.addr == header->nwkDstAddr);

    if ((entry->nextHopAddr != header->macSrcAddr && frame->rx.lqi > entry->lqi) || discovery)
    {
      entry->nextHopAddr = header->macSrcAddr;
      entry->score = NWK_ROUTE_DEFAULT_SCORE;
    }
  }
  else
  {
    entry = NWK_RouteNewEntry();

    entry->dstAddr = header->nwkSrcAddr;
    entry->nextHopAddr = header->macSrcAddr;
  }

  entry->lqi = frame->rx.lqi;
#else
  (void)frame;
#endif
}

/*************************************************************************//**
*****************************************************************************/
void nwkRouteFrameSent(NwkFrame_t *frame)
{
  NWK_RouteTableEntry_t *entry;

  if (NWK_BROADCAST_ADDR == frame->header.nwkDstAddr)
    return;

  entry = NWK_RouteFindEntry(frame->header.nwkDstAddr, frame->header.nwkFcf.multicast);

  if (NULL == entry || entry->fixed)
    return;

  if (NWK_SUCCESS_STATUS == frame->tx.status)
  {
    entry->score = NWK_ROUTE_DEFAULT_SCORE;

    if (NWK_ROUTE_MAX_RANK == ++entry->rank)
      nwkRouteNormalizeRanks();
  }
  else
  {
    if (0 == --entry->score)
      NWK_RouteFreeEntry(entry);
  }
}

/*************************************************************************//**
*****************************************************************************/
void nwkRoutePrepareTx(NwkFrame_t *frame)
{
  NwkFrameHeader_t *header = &frame->header;

  if (NWK_BROADCAST_ADDR == header->nwkDstAddr)
  {
    header->macDstAddr = NWK_BROADCAST_ADDR;
  }

  else if (header->nwkFcf.linkLocal)
  {
    header->macDstAddr = header->nwkDstAddr;
  }

#ifdef NWK_ENABLE_MULTICAST
  else if (header->nwkFcf.multicast && NWK_GroupIsMember(header->nwkDstAddr))
  {
    header->macDstAddr = NWK_BROADCAST_ADDR;
    header->nwkFcf.linkLocal = 1;
  }
#endif

  else
  {
    header->macDstAddr = NWK_RouteNextHop(header->nwkDstAddr, header->nwkFcf.multicast);

  #ifdef NWK_ENABLE_ROUTE_DISCOVERY
    if (NWK_ROUTE_UNKNOWN == header->macDstAddr)
      nwkRouteDiscoveryRequest(frame);
  #endif
  }
}

/*************************************************************************//**
*****************************************************************************/
void nwkRouteFrame(NwkFrame_t *frame)
{
  NwkFrameHeader_t *header = &frame->header;

  if (NWK_ROUTE_UNKNOWN != NWK_RouteNextHop(header->nwkDstAddr, header->nwkFcf.multicast))
  {
    frame->tx.confirm = NULL;
    frame->tx.control = NWK_TX_CONTROL_ROUTING;
    nwkTxFrame(frame);
  }
  else
  {
    nwkRouteSendRouteError(header->nwkSrcAddr, header->nwkDstAddr, header->nwkFcf.multicast);
    nwkFrameFree(frame);
  }
}

/*************************************************************************//**
*****************************************************************************/
static void nwkRouteSendRouteError(uint16_t src, uint16_t dst, uint8_t multicast)
{
  NwkFrame_t *frame;
  NwkCommandRouteError_t *command;

  if (NULL == (frame = nwkFrameAlloc()))
    return;

  nwkFrameCommandInit(frame);

  frame->size += sizeof(NwkCommandRouteError_t);
  frame->tx.confirm = NULL;

  frame->header.nwkDstAddr = src;

  command = (NwkCommandRouteError_t *)frame->payload;
  command->id = NWK_COMMAND_ROUTE_ERROR;
  command->srcAddr = src;
  command->dstAddr = dst;
  command->multicast = multicast;

  nwkTxFrame(frame);
}

/*************************************************************************//**
*****************************************************************************/
bool nwkRouteErrorReceived(NWK_DataInd_t *ind)
{
  NwkCommandRouteError_t *command = (NwkCommandRouteError_t *)ind->data;

  if (sizeof(NwkCommandRouteError_t) != ind->size)
    return false;

  nwkRouteRemove(command->dstAddr, command->multicast);

  return true;
}

/*************************************************************************//**
*****************************************************************************/
static void nwkRouteNormalizeRanks(void)
{
  for (uint8_t i = 0; i < NWK_ROUTE_TABLE_SIZE; i++)
    nwkRouteTable[i].rank = (nwkRouteTable[i].rank >> 1) + 1;
}

#endif // NWK_ENABLE_ROUTING
