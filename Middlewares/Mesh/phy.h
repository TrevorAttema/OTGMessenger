/**
 * \file phy.h
 *
 * \brief AT86RF212 PHY interface
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
 * $Id: phy.h 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

#ifndef _PHY_H_
#define _PHY_H_

/*- Includes ---------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "sysConfig.h"

/*- Definitions ------------------------------------------------------------*/
#define PHY_RSSI_BASE_VAL_BPSK_20             (-100)
#define PHY_RSSI_BASE_VAL_BPSK_40             (-99)
#define PHY_RSSI_BASE_VAL_OQPSK_SIN_RC_100    (-98)
#define PHY_RSSI_BASE_VAL_OQPSK_SIN_250       (-97)
#define PHY_RSSI_BASE_VAL_OQPSK_RC_250        (-97)

#define PHY_HAS_RANDOM_NUMBER_GENERATOR
#define PHY_HAS_AES_MODULE

/*- Types ------------------------------------------------------------------*/
typedef struct PHY_DataInd_t
{
  uint8_t    *data;
  uint8_t    size;
  uint8_t    lqi;
  int8_t     rssi;
} PHY_DataInd_t;

enum
{
  PHY_STATUS_SUCCESS                = 0,
  PHY_STATUS_CHANNEL_ACCESS_FAILURE = 1,
  PHY_STATUS_NO_ACK                 = 2,
  PHY_STATUS_ERROR                  = 3,
};

/*- Prototypes -------------------------------------------------------------*/
void PHY_Init(void);
void PHY_SetRxState(bool rx);
void PHY_SetChannel(uint8_t channel);
void PHY_SetBand(uint8_t band);
void PHY_SetModulation(uint8_t modulation);
void PHY_SetPanId(uint16_t panId);
void PHY_SetShortAddr(uint16_t addr);
void PHY_SetTxPower(uint8_t txPower);
void PHY_Sleep(void);
void PHY_Wakeup(void);
void PHY_DataReq(uint8_t *data, uint8_t size);
void PHY_DataConf(uint8_t status);
void PHY_DataInd(PHY_DataInd_t *ind);
void PHY_TaskHandler(void);

#ifdef PHY_ENABLE_RANDOM_NUMBER_GENERATOR
uint16_t PHY_RandomReq(void);
#endif

#ifdef PHY_ENABLE_AES_MODULE
void PHY_EncryptReq(uint8_t *text, uint8_t *key);
#endif

#ifdef PHY_ENABLE_ENERGY_DETECTION
int8_t PHY_EdReq(void);
#endif

#endif // _PHY_H_
