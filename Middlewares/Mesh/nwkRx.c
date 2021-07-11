/**
 * \file nwkRx.c
 *
 * \brief Receive routines implementation
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
 * $Id: nwkRx.c 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "phy.h"
#include "sysConfig.h"
#include "sysTimer.h"
#include "nwk.h"
#include "nwkTx.h"
#include "nwkFrame.h"
#include "nwkGroup.h"
#include "nwkRoute.h"
#include "nwkCommand.h"
#include "nwkSecurity.h"
#include "nwkRouteDiscovery.h"

/*- Definitions ------------------------------------------------------------*/
#define NWK_RX_DUPLICATE_REJECTION_TIMER_INTERVAL   100 // ms
#define DUPLICATE_REJECTION_TTL \
            ((NWK_DUPLICATE_REJECTION_TTL / NWK_RX_DUPLICATE_REJECTION_TIMER_INTERVAL) + 1)
#define NWK_SERVICE_ENDPOINT_ID    0

/*- Types ------------------------------------------------------------------*/
enum
{
  NWK_RX_STATE_RECEIVED = 0x20,
  NWK_RX_STATE_DECRYPT  = 0x21,
  NWK_RX_STATE_INDICATE = 0x22,
  NWK_RX_STATE_ROUTE    = 0x23,
  NWK_RX_STATE_FINISH   = 0x24,
};

typedef struct NwkDuplicateRejectionEntry_t
{
  uint16_t src;
  uint8_t  seq;
  uint8_t  mask;
  uint8_t  ttl;
} NwkDuplicateRejectionEntry_t;

/*- Prototypes -------------------------------------------------------------*/
static void nwkRxDuplicateRejectionTimerHandler(SYS_Timer_t *timer);
static bool nwkRxServiceDataInd(NWK_DataInd_t *ind);

/*- Variables --------------------------------------------------------------*/
static NwkDuplicateRejectionEntry_t nwkRxDuplicateRejectionTable[NWK_DUPLICATE_REJECTION_TABLE_SIZE];
static uint8_t nwkRxAckControl;
static SYS_Timer_t nwkRxDuplicateRejectionTimer;

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
  @brief Initializes the Rx module
*****************************************************************************/
void nwkRxInit(void)
{
  for (uint8_t i = 0; i < NWK_DUPLICATE_REJECTION_TABLE_SIZE; i++)
    nwkRxDuplicateRejectionTable[i].ttl = 0;

  nwkRxDuplicateRejectionTimer.interval = NWK_RX_DUPLICATE_REJECTION_TIMER_INTERVAL;
  nwkRxDuplicateRejectionTimer.mode = SYS_TIMER_INTERVAL_MODE;
  nwkRxDuplicateRejectionTimer.handler = nwkRxDuplicateRejectionTimerHandler;

  NWK_OpenEndpoint(NWK_SERVICE_ENDPOINT_ID, nwkRxServiceDataInd);
}

/*************************************************************************//**
*****************************************************************************/
void PHY_DataInd(PHY_DataInd_t *ind)
{
  NwkFrame_t *frame;

  if (0x88 != ind->data[1] || (0x61 != ind->data[0] && 0x41 != ind->data[0]) ||
      ind->size < sizeof(NwkFrameHeader_t))
    return;

  if (NULL == (frame = nwkFrameAlloc()))
    return;

  frame->state = NWK_RX_STATE_RECEIVED;
  frame->size = ind->size;
  frame->rx.lqi = ind->lqi;
  frame->rx.rssi = ind->rssi;
  memcpy(frame->data, ind->data, ind->size);
}

/*************************************************************************//**
*****************************************************************************/
static void nwkRxSendAck(NwkFrame_t *frame)
{
  NwkFrame_t *ack;
  NwkCommandAck_t *command;

  if (NULL == (ack = nwkFrameAlloc()))
    return;

  nwkFrameCommandInit(ack);

  ack->size += sizeof(NwkCommandAck_t);
  ack->tx.confirm = NULL;

  ack->header.nwkFcf.security = frame->header.nwkFcf.security;
  ack->header.nwkDstAddr = frame->header.nwkSrcAddr;

  command = (NwkCommandAck_t *)ack->payload;
  command->id = NWK_COMMAND_ACK;
  command->control = nwkRxAckControl;
  command->seq = frame->header.nwkSeq;

  nwkTxFrame(ack);
}

/*************************************************************************//**
*****************************************************************************/
void NWK_SetAckControl(uint8_t control)
{
  nwkRxAckControl = control;
}

#ifdef NWK_ENABLE_SECURITY
/*************************************************************************//**
*****************************************************************************/
void nwkRxDecryptConf(NwkFrame_t *frame, bool status)
{
  if (status)
    frame->state = NWK_RX_STATE_INDICATE;
  else
    frame->state = NWK_RX_STATE_FINISH;
}
#endif

/*************************************************************************//**
*****************************************************************************/
static void nwkRxDuplicateRejectionTimerHandler(SYS_Timer_t *timer)
{
  bool restart = false;

  for (uint8_t i = 0; i < NWK_DUPLICATE_REJECTION_TABLE_SIZE; i++)
  {
    if (nwkRxDuplicateRejectionTable[i].ttl)
    {
      nwkRxDuplicateRejectionTable[i].ttl--;
      restart = true;
    }
  }

  if (restart)
    SYS_TimerStart(timer);
}

/*************************************************************************//**
*****************************************************************************/
static bool nwkRxRejectDuplicate(NwkFrameHeader_t *header)
{
  NwkDuplicateRejectionEntry_t *entry;
  NwkDuplicateRejectionEntry_t *freeEntry = NULL;

  for (uint8_t i = 0; i < NWK_DUPLICATE_REJECTION_TABLE_SIZE; i++)
  {
    entry = &nwkRxDuplicateRejectionTable[i];

    if (entry->ttl && header->nwkSrcAddr == entry->src)
    {
      uint8_t diff = (int8_t)entry->seq - header->nwkSeq;

      if (diff < 8)
      {
        if (entry->mask & (1 << diff))
        {
        #ifdef NWK_ENABLE_ROUTING
          if (nwkIb.addr == header->macDstAddr)
            nwkRouteRemove(header->nwkDstAddr, header->nwkFcf.multicast);
        #endif
          return true;
        }

        entry->mask |= (1 << diff);
        return false;
      }
      else
      {
        uint8_t shift = -(int8_t)diff;

        entry->seq = header->nwkSeq;
        entry->mask = (entry->mask << shift) | 1;
        entry->ttl = DUPLICATE_REJECTION_TTL;
        return false;
      }
    }

    if (0 == entry->ttl)
      freeEntry = entry;
  }

  if (NULL == freeEntry)
    return true;

  freeEntry->src = header->nwkSrcAddr;
  freeEntry->seq = header->nwkSeq;
  freeEntry->mask = 1;
  freeEntry->ttl = DUPLICATE_REJECTION_TTL;

  SYS_TimerStart(&nwkRxDuplicateRejectionTimer);

  return false;
}

/*************************************************************************//**
*****************************************************************************/
static bool nwkRxServiceDataInd(NWK_DataInd_t *ind)
{
#ifdef NWK_ENABLE_SECURE_COMMANDS
  if (0 == (ind->options & NWK_IND_OPT_SECURED))
    return false;
#endif

  if (ind->size < 1)
    return false;

  switch (ind->data[0])
  {
    case NWK_COMMAND_ACK:
      return nwkTxAckReceived(ind);

#ifdef NWK_ENABLE_ROUTING
    case NWK_COMMAND_ROUTE_ERROR:
      return nwkRouteErrorReceived(ind);
#endif

#ifdef NWK_ENABLE_ROUTE_DISCOVERY
    case NWK_COMMAND_ROUTE_REQUEST:
      return nwkRouteDiscoveryRequestReceived(ind);

    case NWK_COMMAND_ROUTE_REPLY:
      return nwkRouteDiscoveryReplyReceived(ind);
#endif

    default:
      return false;
  }
}

/*************************************************************************//**
*****************************************************************************/
static void nwkRxHandleReceivedFrame(NwkFrame_t *frame)
{
  NwkFrameHeader_t *header = &frame->header;

  frame->state = NWK_RX_STATE_FINISH;

#ifndef NWK_ENABLE_SECURITY
  if (header->nwkFcf.security)
    return;
#endif

#ifdef NWK_ENABLE_MULTICAST
  if (header->nwkFcf.multicast && header->nwkFcf.ackRequest)
    return;
#else
  if (header->nwkFcf.multicast)
    return;
#endif

  if (NWK_BROADCAST_PANID == header->macDstPanId)
  {
    if (nwkIb.addr == header->nwkDstAddr || NWK_BROADCAST_ADDR == header->nwkDstAddr)
    {
    #ifdef NWK_ENABLE_SECURITY
      if (header->nwkFcf.security)
        frame->state = NWK_RX_STATE_DECRYPT;
      else
    #endif
        frame->state = NWK_RX_STATE_INDICATE;
    }
    return;
  }

#ifdef NWK_ENABLE_ADDRESS_FILTER
  if (!NWK_FilterAddress(header->macSrcAddr, &frame->rx.lqi))
    return;
#endif

  if (NWK_BROADCAST_ADDR == header->nwkDstAddr && header->nwkFcf.ackRequest)
    return;

  if (nwkIb.addr == header->nwkSrcAddr)
    return;

#ifdef NWK_ENABLE_ROUTING
  nwkRouteFrameReceived(frame);
#endif

  if (nwkRxRejectDuplicate(header))
    return;

#ifdef NWK_ENABLE_MULTICAST
  if (header->nwkFcf.multicast)
  {
    NwkFrameMulticastHeader_t *mcHeader = (NwkFrameMulticastHeader_t *)frame->payload;
    bool member = NWK_GroupIsMember(header->nwkDstAddr);
    bool broadcast = false;

    if (NWK_BROADCAST_ADDR == header->macDstAddr)
    {
      if (member)
      {
        broadcast = mcHeader->memberRadius > 0;
        mcHeader->memberRadius--;
        mcHeader->nonMemberRadius = mcHeader->maxNonMemberRadius;
      }
      else
      {
        broadcast = mcHeader->nonMemberRadius > 0;
        mcHeader->nonMemberRadius--;
        mcHeader->memberRadius = mcHeader->maxMemberRadius;
      }
    }
    else
    {
      if (member)
      {
        broadcast = true;
        header->nwkFcf.linkLocal = 1;
      }
    #ifdef NWK_ENABLE_ROUTING
      else
      {
        frame->state = NWK_RX_STATE_ROUTE;
      }
    #endif
    }

    if (broadcast)
      nwkTxBroadcastFrame(frame);

    if (member)
    {
      frame->payload += sizeof(NwkFrameMulticastHeader_t);

    #ifdef NWK_ENABLE_SECURITY
      if (header->nwkFcf.security)
        frame->state = NWK_RX_STATE_DECRYPT;
      else
    #endif
        frame->state = NWK_RX_STATE_INDICATE;
    }
  }
  else
#endif // NWK_ENABLE_MULTICAST
  {
    if (NWK_BROADCAST_ADDR == header->macDstAddr && nwkIb.addr != header->nwkDstAddr &&
        0 == header->nwkFcf.linkLocal)
      nwkTxBroadcastFrame(frame);

    if (nwkIb.addr == header->nwkDstAddr || NWK_BROADCAST_ADDR == header->nwkDstAddr)
    {
    #ifdef NWK_ENABLE_SECURITY
      if (header->nwkFcf.security)
        frame->state = NWK_RX_STATE_DECRYPT;
      else
    #endif
        frame->state = NWK_RX_STATE_INDICATE;
    }

  #ifdef NWK_ENABLE_ROUTING
    else if (nwkIb.addr == header->macDstAddr)
    {
      frame->state = NWK_RX_STATE_ROUTE;
    }
  #endif
  }
}

/*************************************************************************//**
*****************************************************************************/
static bool nwkRxIndicateFrame(NwkFrame_t *frame)
{
  NwkFrameHeader_t *header = &frame->header;
  NWK_DataInd_t ind;

  if (NULL == nwkIb.endpoint[header->nwkDstEndpoint])
    return false;

  ind.srcAddr = header->nwkSrcAddr;
  ind.dstAddr = header->nwkDstAddr;
  ind.srcEndpoint = header->nwkSrcEndpoint;
  ind.dstEndpoint = header->nwkDstEndpoint;
  ind.data = frame->payload;
  ind.size = nwkFramePayloadSize(frame);
  ind.lqi = frame->rx.lqi;
  ind.rssi = frame->rx.rssi;

  ind.options  = (header->nwkFcf.ackRequest) ? NWK_IND_OPT_ACK_REQUESTED : 0;
  ind.options |= (header->nwkFcf.security) ? NWK_IND_OPT_SECURED : 0;
  ind.options |= (header->nwkFcf.linkLocal) ? NWK_IND_OPT_LINK_LOCAL : 0;
  ind.options |= (header->nwkFcf.multicast) ? NWK_IND_OPT_MULTICAST : 0;
  ind.options |= (NWK_BROADCAST_ADDR == header->nwkDstAddr) ? NWK_IND_OPT_BROADCAST : 0;
  ind.options |= (header->nwkSrcAddr == header->macSrcAddr) ? NWK_IND_OPT_LOCAL : 0;
  ind.options |= (NWK_BROADCAST_PANID == header->macDstPanId) ? NWK_IND_OPT_BROADCAST_PAN_ID : 0;

  return nwkIb.endpoint[header->nwkDstEndpoint](&ind);
}

/*************************************************************************//**
*****************************************************************************/
static void nwkRxHandleIndication(NwkFrame_t *frame)
{
  bool ack;

  nwkRxAckControl = 0;
  ack = nwkRxIndicateFrame(frame);
  
  if (0 == frame->header.nwkFcf.ackRequest)
    ack = false;

  if (NWK_BROADCAST_ADDR == frame->header.macDstAddr &&
      nwkIb.addr == frame->header.nwkDstAddr &&
      0 == frame->header.nwkFcf.multicast)
    ack = true;

  if (NWK_BROADCAST_PANID == frame->header.macDstPanId)
    ack = false;

  if (NWK_BROADCAST_ADDR == nwkIb.addr)
    ack = false;

  if (ack)
    nwkRxSendAck(frame);

  frame->state = NWK_RX_STATE_FINISH;
}

/*************************************************************************//**
  @brief Rx Module task handler
*****************************************************************************/
void nwkRxTaskHandler(void)
{
  NwkFrame_t *frame = NULL;

  while (NULL != (frame = nwkFrameNext(frame)))
  {
    switch (frame->state)
    {
      case NWK_RX_STATE_RECEIVED:
      {
        nwkRxHandleReceivedFrame(frame);
      } break;

#ifdef NWK_ENABLE_SECURITY
      case NWK_RX_STATE_DECRYPT:
      {
        nwkSecurityProcess(frame, false);
      } break;
#endif

      case NWK_RX_STATE_INDICATE:
      {
        nwkRxHandleIndication(frame);
      } break;

#ifdef NWK_ENABLE_ROUTING
      case NWK_RX_STATE_ROUTE:
      {
        nwkRouteFrame(frame);
      } break;
#endif

      case NWK_RX_STATE_FINISH:
      {
        nwkFrameFree(frame);
      } break;
    }
  }
}
