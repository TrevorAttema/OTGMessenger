/**
 * \file sysConfig.h
 *
 * \brief Main system configyration file
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
 * $Id: sysConfig.h 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*- Includes ---------------------------------------------------------------*/
//#include "config.h"

/*- Definitions ------------------------------------------------------------*/
#ifndef NWK_BUFFERS_AMOUNT
#define NWK_BUFFERS_AMOUNT                       5
#endif

#ifndef NWK_DUPLICATE_REJECTION_TABLE_SIZE
#define NWK_DUPLICATE_REJECTION_TABLE_SIZE       10
#endif

#ifndef NWK_DUPLICATE_REJECTION_TTL
#define NWK_DUPLICATE_REJECTION_TTL              1000 // ms
#endif

#ifndef NWK_ROUTE_TABLE_SIZE
#define NWK_ROUTE_TABLE_SIZE                     10
#endif

#ifndef NWK_ROUTE_DEFAULT_SCORE
#define NWK_ROUTE_DEFAULT_SCORE                  3
#endif

#ifndef NWK_ACK_WAIT_TIME
#define NWK_ACK_WAIT_TIME                        1000 // ms
#endif

#ifndef NWK_GROUPS_AMOUNT
#define NWK_GROUPS_AMOUNT                        10
#endif

#ifndef NWK_ROUTE_DISCOVERY_TABLE_SIZE
#define NWK_ROUTE_DISCOVERY_TABLE_SIZE           5
#endif

#ifndef NWK_ROUTE_DISCOVERY_TIMEOUT
#define NWK_ROUTE_DISCOVERY_TIMEOUT              1000 // ms
#endif

//#define NWK_ENABLE_ROUTING
//#define NWK_ENABLE_SECURITY
//#define NWK_ENABLE_MULTICAST
//#define NWK_ENABLE_ROUTE_DISCOVERY
//#define NWK_ENABLE_SECURE_COMMANDS

#ifndef SYS_SECURITY_MODE
#define SYS_SECURITY_MODE                        0
#endif

/*- Sanity checks ----------------------------------------------------------*/
#if defined(NWK_ENABLE_SECURITY) && (SYS_SECURITY_MODE == 0)
  #define PHY_ENABLE_AES_MODULE
#endif

#endif // _SYS_CONFIG_H_
