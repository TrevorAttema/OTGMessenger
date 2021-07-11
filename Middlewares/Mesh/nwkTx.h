/**
 * \file nwkTx.h
 *
 * \brief Transmit routines interface
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
 * $Id: nwkTx.h 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

#ifndef _NWK_TX_H_
#define _NWK_TX_H_

/*- Includes ---------------------------------------------------------------*/
#include <stdint.h>
#include "sysTypes.h"
#include "nwkRx.h"
#include "nwkFrame.h"

/*- Types ------------------------------------------------------------------*/
enum
{
  NWK_TX_CONTROL_BROADCAST_PAN_ID = 1 << 0,
  NWK_TX_CONTROL_ROUTING          = 1 << 1,
  NWK_TX_CONTROL_DIRECT_LINK      = 1 << 2,
};

/*- Prototypes -------------------------------------------------------------*/
void nwkTxInit(void);
void nwkTxFrame(NwkFrame_t *frame);
void nwkTxBroadcastFrame(NwkFrame_t *frame);
bool nwkTxAckReceived(NWK_DataInd_t *ind);
void nwkTxConfirm(NwkFrame_t *frame, uint8_t status);
void nwkTxEncryptConf(NwkFrame_t *frame);
void nwkTxTaskHandler(void);

#endif // _NWK_TX_H_
