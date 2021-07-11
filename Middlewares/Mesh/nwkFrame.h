/**
 * \file nwkFrame.h
 *
 * \brief Frame buffers management interface
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
 * $Id: nwkFrame.h 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

#ifndef _NWK_FRAME_H_
#define _NWK_FRAME_H_

/*- Includes ---------------------------------------------------------------*/
#include <stdint.h>
#include "sysTypes.h"

/*- Definitions ------------------------------------------------------------*/
#define NWK_FRAME_MAX_PAYLOAD_SIZE   127

/*- Types ------------------------------------------------------------------*/
typedef struct PACK NwkFrameHeader_t
{
  uint16_t    macFcf;
  uint8_t     macSeq;
  uint16_t    macDstPanId;
  uint16_t    macDstAddr;
  uint16_t    macSrcAddr;

  struct PACK
  {
    uint8_t   ackRequest : 1;
    uint8_t   security   : 1;
    uint8_t   linkLocal  : 1;
    uint8_t   multicast  : 1;
    uint8_t   reserved   : 4;
  }           nwkFcf;
  uint8_t     nwkSeq;
  uint16_t    nwkSrcAddr;
  uint16_t    nwkDstAddr;
  struct PACK
  {
    uint8_t   nwkSrcEndpoint : 4;
    uint8_t   nwkDstEndpoint : 4;
  };
} NwkFrameHeader_t;

typedef struct PACK NwkFrameMulticastHeader_t
{
  uint16_t    nonMemberRadius    : 4;
  uint16_t    maxNonMemberRadius : 4;
  uint16_t    memberRadius       : 4;
  uint16_t    maxMemberRadius    : 4;
} NwkFrameMulticastHeader_t;

typedef struct NwkFrame_t
{
  uint8_t      state;
  uint8_t      size;

  union
  {
    NwkFrameHeader_t header;
    uint8_t          data[NWK_FRAME_MAX_PAYLOAD_SIZE];
  };

  uint8_t      *payload;

  union
  {
    struct
    {
      uint8_t  lqi;
      int8_t   rssi;
    } rx;

    struct
    {
      uint8_t  status;
      uint16_t timeout;
      uint8_t  control;
      void     (*confirm)(struct NwkFrame_t *frame);
    } tx;
  };
} NwkFrame_t;

/*- Prototypes -------------------------------------------------------------*/
void nwkFrameInit(void);
NwkFrame_t *nwkFrameAlloc(void);
void nwkFrameFree(NwkFrame_t *frame);
NwkFrame_t *nwkFrameNext(NwkFrame_t *frame);
void nwkFrameCommandInit(NwkFrame_t *frame);

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*****************************************************************************/
static inline uint8_t nwkFramePayloadSize(NwkFrame_t *frame)
{
  return frame->size - (frame->payload - frame->data);
}

#endif // _NWK_FRAME_H_
