/**
 * \file nwkRouteDiscovery.c
 *
 * \brief Route discovery implementation
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
 * $Id: nwkRouteDiscovery.c 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "sysTypes.h"
#include "sysTimer.h"
#include "sysConfig.h"
#include "nwk.h"
#include "nwkTx.h"
#include "nwkFrame.h"
#include "nwkRoute.h"
#include "nwkGroup.h"
#include "nwkCommand.h"
#include "nwkRouteDiscovery.h"

#ifdef NWK_ENABLE_ROUTE_DISCOVERY

/*- Definitions ------------------------------------------------------------*/
#define NWK_ROUTE_DISCOVERY_BEST_LINK_QUALITY    255
#define NWK_ROUTE_DISCOVERY_NO_LINK              0
#define NWK_ROUTE_DISCOVERY_TIMER_INTERVAL       100 // ms

/*- Types ------------------------------------------------------------------*/
enum
{
  NWK_RD_STATE_WAIT_FOR_ROUTE = 0x40,
};

typedef struct NwkRouteDiscoveryTableEntry_t
{
  uint16_t   srcAddr;
  uint16_t   dstAddr;
  uint8_t    multicast;
  uint16_t   senderAddr;
  uint8_t    forwardLinkQuality;
  uint8_t    reverseLinkQuality;
  uint16_t   timeout;
} NwkRouteDiscoveryTableEntry_t;

/*- Prototypes -------------------------------------------------------------*/
static NwkRouteDiscoveryTableEntry_t *nwkRouteDiscoveryFindEntry(uint16_t src,
    uint16_t dst, uint8_t multicast);
static NwkRouteDiscoveryTableEntry_t *nwkRouteDiscoveryNewEntry(void);
static void nwkRouteDiscoveryTimerHandler(SYS_Timer_t *timer);
static bool nwkRouteDiscoverySendRequest(NwkRouteDiscoveryTableEntry_t *entry, uint8_t lq);
static void nwkRouteDiscoverySendReply(NwkRouteDiscoveryTableEntry_t *entry, uint8_t flq, uint8_t rlq);
static void nwkRouteDiscoveryDone(NwkRouteDiscoveryTableEntry_t *entry, bool status);
static uint8_t nwkRouteDiscoveryUpdateLq(uint8_t lqa, uint8_t lqb);

/*- Variables --------------------------------------------------------------*/
static NwkRouteDiscoveryTableEntry_t nwkRouteDiscoveryTable[NWK_ROUTE_DISCOVERY_TABLE_SIZE];
static SYS_Timer_t nwkRouteDiscoveryTimer;

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
  @brief Initializes the Route Discovery module
*****************************************************************************/
void nwkRouteDiscoveryInit(void)
{
  for (uint8_t i = 0; i < NWK_ROUTE_DISCOVERY_TABLE_SIZE; i++)
    nwkRouteDiscoveryTable[i].timeout = 0;

  nwkRouteDiscoveryTimer.interval = NWK_ROUTE_DISCOVERY_TIMER_INTERVAL;
  nwkRouteDiscoveryTimer.mode = SYS_TIMER_INTERVAL_MODE;
  nwkRouteDiscoveryTimer.handler = nwkRouteDiscoveryTimerHandler;
}

/*************************************************************************//**
*****************************************************************************/
void nwkRouteDiscoveryRequest(NwkFrame_t *frame)
{
  NwkFrameHeader_t *header = &frame->header;
  NwkRouteDiscoveryTableEntry_t *entry;
  
  entry = nwkRouteDiscoveryFindEntry(nwkIb.addr, header->nwkDstAddr, header->nwkFcf.multicast);

  if (entry)
  {
    frame->state = NWK_RD_STATE_WAIT_FOR_ROUTE;
    return;
  }

  entry = nwkRouteDiscoveryNewEntry();

  if (entry)
  {
    entry->srcAddr = nwkIb.addr;
    entry->dstAddr = header->nwkDstAddr;
    entry->multicast = header->nwkFcf.multicast;
    entry->senderAddr = NWK_BROADCAST_ADDR;

    if (nwkRouteDiscoverySendRequest(entry, NWK_ROUTE_DISCOVERY_BEST_LINK_QUALITY))
    {
      frame->state = NWK_RD_STATE_WAIT_FOR_ROUTE;
      return;
    }
  }

  nwkTxConfirm(frame, NWK_NO_ROUTE_STATUS);
}

/*************************************************************************//**
*****************************************************************************/
static NwkRouteDiscoveryTableEntry_t *nwkRouteDiscoveryFindEntry(uint16_t src,
    uint16_t dst, uint8_t multicast)
{
  for (uint8_t i = 0; i < NWK_ROUTE_DISCOVERY_TABLE_SIZE; i++)
  {
    if (nwkRouteDiscoveryTable[i].timeout > 0 &&
        nwkRouteDiscoveryTable[i].srcAddr == src &&
        nwkRouteDiscoveryTable[i].dstAddr == dst &&
        nwkRouteDiscoveryTable[i].multicast == multicast)
      return &nwkRouteDiscoveryTable[i];
  }

  return NULL;
}

/*************************************************************************//**
*****************************************************************************/
static NwkRouteDiscoveryTableEntry_t *nwkRouteDiscoveryNewEntry(void)
{
  NwkRouteDiscoveryTableEntry_t *entry = NULL;

  for (uint8_t i = 0; i < NWK_ROUTE_DISCOVERY_TABLE_SIZE; i++)
  {
    if (0 == nwkRouteDiscoveryTable[i].timeout)
    {
      entry = &nwkRouteDiscoveryTable[i];
      break;
    }
  }

  if (entry)
  {
    entry->forwardLinkQuality = NWK_ROUTE_DISCOVERY_NO_LINK;
    entry->reverseLinkQuality = NWK_ROUTE_DISCOVERY_NO_LINK;
    entry->timeout = NWK_ROUTE_DISCOVERY_TIMEOUT;
    SYS_TimerStart(&nwkRouteDiscoveryTimer);
  }

  return entry;
}

/*************************************************************************//**
*****************************************************************************/
static void nwkRouteDiscoveryTimerHandler(SYS_Timer_t *timer)
{
  NwkRouteDiscoveryTableEntry_t *entry;
  bool restart = false;

  for (uint8_t i = 0; i < NWK_ROUTE_DISCOVERY_TABLE_SIZE; i++)
  {
    entry = &nwkRouteDiscoveryTable[i];

    if (entry->timeout > NWK_ROUTE_DISCOVERY_TIMER_INTERVAL)
    {
      entry->timeout -= NWK_ROUTE_DISCOVERY_TIMER_INTERVAL;
      restart = true;
    }
    else
    {
      entry->timeout = 0;

      if (entry->srcAddr == nwkIb.addr)
        nwkRouteDiscoveryDone(entry, entry->reverseLinkQuality > 0);
    }
  }

  if (restart)
    SYS_TimerStart(timer);
}

/*************************************************************************//**
*****************************************************************************/
static bool nwkRouteDiscoverySendRequest(NwkRouteDiscoveryTableEntry_t *entry, uint8_t lq)
{
  NwkFrame_t *req;
  NwkCommandRouteRequest_t *command;

  if (NULL == (req = nwkFrameAlloc()))
    return false;

  nwkFrameCommandInit(req);

  req->size += sizeof(NwkCommandRouteRequest_t);
  req->tx.confirm = NULL;

  req->header.nwkFcf.linkLocal = 1;
  req->header.nwkDstAddr = NWK_BROADCAST_ADDR;

  command = (NwkCommandRouteRequest_t *)req->payload;
  command->id = NWK_COMMAND_ROUTE_REQUEST;
  command->srcAddr = entry->srcAddr;
  command->dstAddr = entry->dstAddr;
  command->multicast = entry->multicast;
  command->linkQuality = lq;

  nwkTxFrame(req);

  return true;
}

/*************************************************************************//**
*****************************************************************************/
bool nwkRouteDiscoveryRequestReceived(NWK_DataInd_t *ind)
{
  NwkCommandRouteRequest_t *command = (NwkCommandRouteRequest_t *)ind->data;
  NwkRouteDiscoveryTableEntry_t *entry;
  uint8_t linkQuality;
  bool reply = false;

  if (sizeof(NwkCommandRouteRequest_t) != ind->size)
    return false;

#ifdef NWK_ENABLE_MULTICAST
  if (1 == command->multicast && NWK_GroupIsMember(command->dstAddr))
    reply = true;
#endif

  if (0 == command->multicast && command->dstAddr == nwkIb.addr)
    reply = true;

  if (command->srcAddr == nwkIb.addr)
    return true;

  if (false == reply && nwkIb.addr & NWK_ROUTE_NON_ROUTING)
    return true;

  linkQuality = nwkRouteDiscoveryUpdateLq(command->linkQuality, ind->lqi);

  entry = nwkRouteDiscoveryFindEntry(command->srcAddr, command->dstAddr, command->multicast);

  if (entry)
  {
    if (linkQuality <= entry->forwardLinkQuality)
      return true;
  }
  else
  {
    if (NULL == (entry = nwkRouteDiscoveryNewEntry()))
      return true;
  }

  entry->srcAddr = command->srcAddr;
  entry->dstAddr = command->dstAddr;
  entry->multicast = command->multicast;
  entry->senderAddr = ind->srcAddr;
  entry->forwardLinkQuality = linkQuality;

  if (reply)
  {
    nwkRouteUpdateEntry(command->srcAddr, 0, ind->srcAddr, linkQuality);
    nwkRouteDiscoverySendReply(entry, linkQuality, NWK_ROUTE_DISCOVERY_BEST_LINK_QUALITY);
  }
  else
  {
    nwkRouteDiscoverySendRequest(entry, linkQuality);
  }

  return true;
}

/*************************************************************************//**
*****************************************************************************/
static void nwkRouteDiscoverySendReply(NwkRouteDiscoveryTableEntry_t *entry, uint8_t flq, uint8_t rlq)
{
  NwkFrame_t *req;
  NwkCommandRouteReply_t *command;

  if (NULL == (req = nwkFrameAlloc()))
    return;

  nwkFrameCommandInit(req);

  req->size += sizeof(NwkCommandRouteReply_t);
  req->tx.confirm = NULL;
  req->tx.control = NWK_TX_CONTROL_DIRECT_LINK;

  req->header.nwkDstAddr = entry->senderAddr;

  command = (NwkCommandRouteReply_t *)req->payload;
  command->id = NWK_COMMAND_ROUTE_REPLY;
  command->srcAddr = entry->srcAddr;
  command->dstAddr = entry->dstAddr;
  command->multicast = entry->multicast;
  command->forwardLinkQuality = flq;
  command->reverseLinkQuality = rlq;

  nwkTxFrame(req);
}

/*************************************************************************//**
*****************************************************************************/
bool nwkRouteDiscoveryReplyReceived(NWK_DataInd_t *ind)
{
  NwkCommandRouteReply_t *command = (NwkCommandRouteReply_t *)ind->data;
  NwkRouteDiscoveryTableEntry_t *entry;
  uint8_t linkQuality;

  if (sizeof(NwkCommandRouteReply_t) != ind->size)
    return false;

  entry = nwkRouteDiscoveryFindEntry(command->srcAddr, command->dstAddr, command->multicast);

  linkQuality = nwkRouteDiscoveryUpdateLq(command->reverseLinkQuality, ind->lqi);

  if (entry && command->forwardLinkQuality > entry->reverseLinkQuality)
  {
    entry->reverseLinkQuality = command->forwardLinkQuality;

    if (command->srcAddr == nwkIb.addr)
    {
      nwkRouteUpdateEntry(command->dstAddr, command->multicast, ind->srcAddr, command->forwardLinkQuality);
      //nwkRouteDiscoveryDone(entry, true);
    }
    else
    {
      nwkRouteUpdateEntry(command->dstAddr, command->multicast,  ind->srcAddr, linkQuality);
      nwkRouteUpdateEntry(command->srcAddr, 0, entry->senderAddr, entry->forwardLinkQuality);
      nwkRouteDiscoverySendReply(entry, command->forwardLinkQuality, linkQuality);
    }
  }

  return true;
}

/*************************************************************************//**
*****************************************************************************/
static void nwkRouteDiscoveryDone(NwkRouteDiscoveryTableEntry_t *entry, bool status)
{
  NwkFrame_t *frame = NULL;

  while (NULL != (frame = nwkFrameNext(frame)))
  {
    if (NWK_RD_STATE_WAIT_FOR_ROUTE != frame->state)
      continue;

    if (entry->dstAddr != frame->header.nwkDstAddr ||
        entry->multicast != frame->header.nwkFcf.multicast)
      continue;

    if (status)
      nwkTxFrame(frame);
    else
      nwkTxConfirm(frame, NWK_NO_ROUTE_STATUS);
  }
}

/*************************************************************************//**
*****************************************************************************/
static uint8_t nwkRouteDiscoveryUpdateLq(uint8_t lqa, uint8_t lqb)
{
  return ((uint16_t)lqa * lqb) >> 8;
}

#endif // NWK_ENABLE_ROUTE_DISCOVERY
