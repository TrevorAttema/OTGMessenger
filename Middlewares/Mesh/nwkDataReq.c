/**
 * \file nwkDataReq.c
 *
 * \brief NWK_DataReq() implementation
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
 * $Id: nwkDataReq.c 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sysConfig.h"
#include "nwk.h"
#include "nwkTx.h"
#include "nwkFrame.h"
#include "nwkGroup.h"
#include "nwkDataReq.h"

/*- Types ------------------------------------------------------------------*/
enum
{
  NWK_DATA_REQ_STATE_INITIAL,
  NWK_DATA_REQ_STATE_WAIT_CONF,
  NWK_DATA_REQ_STATE_CONFIRM,
};

/*- Prototypes -------------------------------------------------------------*/
static void nwkDataReqTxConf(NwkFrame_t *frame);

/*- Variables --------------------------------------------------------------*/
static NWK_DataReq_t *nwkDataReqQueue;

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
  @brief Initializes the Data Request module
*****************************************************************************/
void nwkDataReqInit(void)
{
  nwkDataReqQueue = NULL;
}

/*************************************************************************//**
  @brief Adds request @a req to the queue of outgoing requests
  @param[in] req Pointer to the request parameters
*****************************************************************************/
void NWK_DataReq(NWK_DataReq_t *req)
{
  req->state = NWK_DATA_REQ_STATE_INITIAL;
  req->status = NWK_SUCCESS_STATUS;
  req->frame = NULL;

  nwkIb.lock++;

  if (NULL == nwkDataReqQueue)
  {
    req->next = NULL;
    nwkDataReqQueue = req;
  }
  else
  {
    req->next = nwkDataReqQueue;
    nwkDataReqQueue = req;
  }
}

/*************************************************************************//**
  @brief Prepares and send outgoing frame based on the request @a req parameters
  @param[in] req Pointer to the request parameters
*****************************************************************************/
static void nwkDataReqSendFrame(NWK_DataReq_t *req)
{
  NwkFrame_t *frame;

  if (NULL == (frame = nwkFrameAlloc()))
  {
    req->state = NWK_DATA_REQ_STATE_CONFIRM;
    req->status = NWK_OUT_OF_MEMORY_STATUS;
    return;
  }

  req->frame = frame;
  req->state = NWK_DATA_REQ_STATE_WAIT_CONF;

  frame->tx.confirm = nwkDataReqTxConf;
  frame->tx.control = req->options & NWK_OPT_BROADCAST_PAN_ID ? NWK_TX_CONTROL_BROADCAST_PAN_ID : 0;

  frame->header.nwkFcf.ackRequest = req->options & NWK_OPT_ACK_REQUEST ? 1 : 0;
  frame->header.nwkFcf.linkLocal = req->options & NWK_OPT_LINK_LOCAL ? 1 : 0;

#ifdef NWK_ENABLE_SECURITY
  frame->header.nwkFcf.security = req->options & NWK_OPT_ENABLE_SECURITY ? 1 : 0;
#endif

#ifdef NWK_ENABLE_MULTICAST
  frame->header.nwkFcf.multicast = req->options & NWK_OPT_MULTICAST ? 1 : 0;

  if (frame->header.nwkFcf.multicast)
  {
    NwkFrameMulticastHeader_t *mcHeader = (NwkFrameMulticastHeader_t *)frame->payload;

    mcHeader->memberRadius = req->memberRadius;
    mcHeader->maxMemberRadius = req->memberRadius;
    mcHeader->nonMemberRadius = req->nonMemberRadius;
    mcHeader->maxNonMemberRadius = req->nonMemberRadius;

    frame->payload += sizeof(NwkFrameMulticastHeader_t);
    frame->size += sizeof(NwkFrameMulticastHeader_t);
  }
#endif

  frame->header.nwkSeq = ++nwkIb.nwkSeqNum;
  frame->header.nwkSrcAddr = nwkIb.addr;
  frame->header.nwkDstAddr = req->dstAddr;
  frame->header.nwkSrcEndpoint = req->srcEndpoint;
  frame->header.nwkDstEndpoint = req->dstEndpoint;

  memcpy(frame->payload, req->data, req->size);
  frame->size += req->size;

  nwkTxFrame(frame);
}

/*************************************************************************//**
  @brief Frame transmission confirmation handler
  @param[in] frame Pointer to the sent frame
*****************************************************************************/
static void nwkDataReqTxConf(NwkFrame_t *frame)
{
  for (NWK_DataReq_t *req = nwkDataReqQueue; req; req = req->next)
  {
    if (req->frame == frame)
    {
      req->status = frame->tx.status;
      req->control = frame->tx.control;
      req->state = NWK_DATA_REQ_STATE_CONFIRM;
      break;
    }
  }

  nwkFrameFree(frame);
}

/*************************************************************************//**
  @brief Confirms request @req to the application and remove it from the queue
  @param[in] req Pointer to the request parameters
*****************************************************************************/
static void nwkDataReqConfirm(NWK_DataReq_t *req)
{
  if (nwkDataReqQueue == req)
  {
    nwkDataReqQueue = nwkDataReqQueue->next;
  }
  else
  {
    NWK_DataReq_t *prev = nwkDataReqQueue;
    while (prev->next != req)
      prev = prev->next;
    prev->next = ((NWK_DataReq_t *)prev->next)->next;
  }

  nwkIb.lock--;
  req->confirm(req);
}

/*************************************************************************//**
  @brief Data Request module task handler
*****************************************************************************/
void nwkDataReqTaskHandler(void)
{
  for (NWK_DataReq_t *req = nwkDataReqQueue; req; req = req->next)
  {
    switch (req->state)
    {
      case NWK_DATA_REQ_STATE_INITIAL:
      {
        nwkDataReqSendFrame(req);
        return;
      } break;

      case NWK_DATA_REQ_STATE_WAIT_CONF:
        break;

      case NWK_DATA_REQ_STATE_CONFIRM:
      {
        nwkDataReqConfirm(req);
        return;
      } break;

      default:
        break;
    };
  }
}
