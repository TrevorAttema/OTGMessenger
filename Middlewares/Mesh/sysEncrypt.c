/**
 * \file sysEncrypt.c
 *
 * \brief System encryption routines implementation
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
 * $Id: sysEncrypt.c 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include "sysEncrypt.h"
#include "sysConfig.h"
#include "phy.h"

#ifdef NWK_ENABLE_SECURITY

/*- Implementations --------------------------------------------------------*/

#if SYS_SECURITY_MODE == 1
/*************************************************************************//**
*****************************************************************************/
static void xtea(uint32_t text[2], uint32_t const key[4])
{
  uint32_t t0 = text[0];
  uint32_t t1 = text[1];
  uint32_t sum = 0;
  uint32_t delta = 0x9e3779b9;

  for (uint8_t i = 0; i < 32; i++)
  {
    t0 += (((t1 << 4) ^ (t1 >> 5)) + t1) ^ (sum + key[sum & 3]);
    sum += delta;
    t1 += (((t0 << 4) ^ (t0 >> 5)) + t0) ^ (sum + key[(sum >> 11) & 3]);
  }
  text[0] = t0;
  text[1] = t1;
}
#endif

/*************************************************************************//**
*****************************************************************************/
void SYS_EncryptReq(uint8_t *text, uint8_t *key)
{
#if SYS_SECURITY_MODE == 0
  PHY_EncryptReq(text, key);

#elif SYS_SECURITY_MODE == 1
  xtea(&text[0], key);
  text[2] ^= text[0];
  text[3] ^= text[1];
  xtea(&text[2], key);

#endif

  SYS_EncryptConf();
}

#endif // NWK_ENABLE_SECURITY
