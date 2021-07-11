/**
 * \file nwkSecurity.c
 *
 * \brief Security routines implementation
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
 * $Id: nwkSecurity.c 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sysConfig.h"
#include "sysEncrypt.h"
#include "nwk.h"
#include "nwkTx.h"
#include "nwkFrame.h"
#include "nwkSecurity.h"

#ifdef NWK_ENABLE_SECURITY

/*- Types ------------------------------------------------------------------*/
enum
{
  NWK_SECURITY_STATE_ENCRYPT_PENDING = 0x30,
  NWK_SECURITY_STATE_DECRYPT_PENDING = 0x31,
  NWK_SECURITY_STATE_PROCESS         = 0x32,
  NWK_SECURITY_STATE_WAIT            = 0x33,
  NWK_SECURITY_STATE_CONFIRM         = 0x34,
};

/*- Variables --------------------------------------------------------------*/
static uint8_t nwkSecurityActiveFrames;
static NwkFrame_t *nwkSecurityActiveFrame;
static uint8_t nwkSecuritySize;
static uint8_t nwkSecurityOffset;
static bool nwkSecurityEncrypt;
static uint32_t nwkSecurityVector[4];

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
  @brief Initializes the Security module
*****************************************************************************/
void nwkSecurityInit(void)
{
  nwkSecurityActiveFrames = 0;
  nwkSecurityActiveFrame = NULL;
}

/*************************************************************************//**
*****************************************************************************/
void NWK_SetSecurityKey(uint8_t *key)
{
  memcpy((uint8_t *)nwkIb.key, key, NWK_SECURITY_KEY_SIZE);
}

/*************************************************************************//**
*****************************************************************************/
void nwkSecurityProcess(NwkFrame_t *frame, bool encrypt)
{
  if (encrypt)
    frame->state = NWK_SECURITY_STATE_ENCRYPT_PENDING;
  else
    frame->state = NWK_SECURITY_STATE_DECRYPT_PENDING;
  ++nwkSecurityActiveFrames;
}

/*************************************************************************//**
*****************************************************************************/
static void nwkSecurityStart(void)
{
  NwkFrameHeader_t *header = &nwkSecurityActiveFrame->header;

  nwkSecurityVector[0] = header->nwkSeq;
  nwkSecurityVector[1] = ((uint32_t)header->nwkDstAddr << 16) | header->nwkDstEndpoint;
  nwkSecurityVector[2] = ((uint32_t)header->nwkSrcAddr << 16) | header->nwkSrcEndpoint;
  nwkSecurityVector[3] = ((uint32_t)header->macDstPanId << 16) | *(uint8_t *)&header->nwkFcf;

  if (NWK_SECURITY_STATE_DECRYPT_PENDING == nwkSecurityActiveFrame->state)
    nwkSecurityActiveFrame->size -= NWK_SECURITY_MIC_SIZE;

  nwkSecuritySize = nwkFramePayloadSize(nwkSecurityActiveFrame);
  nwkSecurityOffset = 0;
  nwkSecurityEncrypt = (NWK_SECURITY_STATE_ENCRYPT_PENDING == nwkSecurityActiveFrame->state);

  nwkSecurityActiveFrame->state = NWK_SECURITY_STATE_PROCESS;
}

/*************************************************************************//**
*****************************************************************************/
void SYS_EncryptConf(void)
{
  uint8_t *vector = (uint8_t *)nwkSecurityVector;
  uint8_t *text = &nwkSecurityActiveFrame->payload[nwkSecurityOffset];
  uint8_t block;

  block = (nwkSecuritySize < NWK_SECURITY_BLOCK_SIZE) ? nwkSecuritySize : NWK_SECURITY_BLOCK_SIZE;

  for (uint8_t i = 0; i < block; i++)
  {
    text[i] ^= vector[i];

    if (nwkSecurityEncrypt)
      vector[i] = text[i];
    else
      vector[i] ^= text[i];
  }

  nwkSecurityOffset += block;
  nwkSecuritySize -= block;

  if (nwkSecuritySize > 0)
    nwkSecurityActiveFrame->state = NWK_SECURITY_STATE_PROCESS;
  else
    nwkSecurityActiveFrame->state = NWK_SECURITY_STATE_CONFIRM;
}

/*************************************************************************//**
*****************************************************************************/
static bool nwkSecurityProcessMic(void)
{
  uint8_t *mic = &nwkSecurityActiveFrame->payload[nwkSecurityOffset];
  uint32_t vmic = nwkSecurityVector[0] ^ nwkSecurityVector[1] ^
                  nwkSecurityVector[2] ^ nwkSecurityVector[3];
  uint32_t tmic;

  if (nwkSecurityEncrypt)
  {
    memcpy(mic, (uint8_t *)&vmic, NWK_SECURITY_MIC_SIZE);
    nwkSecurityActiveFrame->size += NWK_SECURITY_MIC_SIZE;
    return true;
  }
  else
  {
    memcpy((uint8_t *)&tmic, mic, NWK_SECURITY_MIC_SIZE);
    return vmic == tmic;
  }
}

/*************************************************************************//**
  @brief Security Module task handler
*****************************************************************************/
void nwkSecurityTaskHandler(void)
{
  NwkFrame_t *frame = NULL;

  if (0 == nwkSecurityActiveFrames)
    return;

  if (nwkSecurityActiveFrame)
  {
    if (NWK_SECURITY_STATE_CONFIRM == nwkSecurityActiveFrame->state)
    {
      bool micStatus = nwkSecurityProcessMic();

      if (nwkSecurityEncrypt)
        nwkTxEncryptConf(nwkSecurityActiveFrame);
      else
        nwkRxDecryptConf(nwkSecurityActiveFrame, micStatus);

      nwkSecurityActiveFrame = NULL;
      --nwkSecurityActiveFrames;
    }
    else if (NWK_SECURITY_STATE_PROCESS == nwkSecurityActiveFrame->state)
    {
      nwkSecurityActiveFrame->state = NWK_SECURITY_STATE_WAIT;
      SYS_EncryptReq((uint8_t *)nwkSecurityVector, (uint8_t *)nwkIb.key);
    }

    return;
  }

  while (NULL != (frame = nwkFrameNext(frame)))
  {
    if (NWK_SECURITY_STATE_ENCRYPT_PENDING == frame->state ||
        NWK_SECURITY_STATE_DECRYPT_PENDING == frame->state)
    {
      nwkSecurityActiveFrame = frame;
      nwkSecurityStart();
      return;
    }
  }
}

#endif // NWK_ENABLE_SECURITY
