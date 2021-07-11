/**
 * \file nwkFrame.c
 *
 * \brief Frame buffers management implementation
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
 * $Id: nwkFrame.c 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sysConfig.h"
#include "nwk.h"
#include "nwkFrame.h"

/*- Types ------------------------------------------------------------------*/
enum
{
  NWK_FRAME_STATE_FREE = 0x00,
};

/*- Variables --------------------------------------------------------------*/
static NwkFrame_t nwkFrameFrames[NWK_BUFFERS_AMOUNT];

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
  @brief Initializes the Frame module
*****************************************************************************/
void nwkFrameInit(void)
{
  for (uint8_t i = 0; i < NWK_BUFFERS_AMOUNT; i++)
    nwkFrameFrames[i].state = NWK_FRAME_STATE_FREE;
}

/*************************************************************************//**
  @brief Allocates an empty frame from the buffer pool
  @return Pointer to the frame or @c NULL if there are no free frames
*****************************************************************************/
NwkFrame_t *nwkFrameAlloc(void)
{
  for (uint8_t i = 0; i < NWK_BUFFERS_AMOUNT; i++)
  {
    if (NWK_FRAME_STATE_FREE == nwkFrameFrames[i].state)
    {
      memset(&nwkFrameFrames[i], 0, sizeof(NwkFrame_t));
      nwkFrameFrames[i].size = sizeof(NwkFrameHeader_t);
      nwkFrameFrames[i].payload = nwkFrameFrames[i].data + sizeof(NwkFrameHeader_t);
      nwkIb.lock++;
      return &nwkFrameFrames[i];
    }
  }
  return NULL;
}

/*************************************************************************//**
  @brief Frees a @a frame and returns it to the buffer pool
  @param[in] frame Pointer to the frame to be freed
*****************************************************************************/
void nwkFrameFree(NwkFrame_t *frame)
{
  frame->state = NWK_FRAME_STATE_FREE;
  nwkIb.lock--;
}

/*************************************************************************//**
  @brief Cycles through the allocated frames starting from the specified @a frame
  @param[in] frame Pointer to the current frame or @c NULL for the first frame
  @return Next allocated frame or @c NULL if there are no more frames
*****************************************************************************/
NwkFrame_t *nwkFrameNext(NwkFrame_t *frame)
{
  if (NULL == frame)
    frame = nwkFrameFrames;
  else
    frame++;

  for (; frame < &nwkFrameFrames[NWK_BUFFERS_AMOUNT]; frame++)
  {
    if (NWK_FRAME_STATE_FREE != frame->state)
      return frame;
  }

  return NULL;
}

/*************************************************************************//**
  @brief Sets default parameters for the the command @a frame
  @param[in] frame Pointer to the command frame
*****************************************************************************/
void nwkFrameCommandInit(NwkFrame_t *frame)
{
  frame->tx.status = NWK_SUCCESS_STATUS;
  frame->header.nwkSeq = ++nwkIb.nwkSeqNum;
  frame->header.nwkSrcAddr = nwkIb.addr;
#ifdef NWK_ENABLE_SECURE_COMMANDS
  frame->header.nwkFcf.security = 1;
#endif
}
