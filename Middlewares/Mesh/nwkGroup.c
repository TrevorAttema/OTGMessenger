/**
 * \file nwkGroup.c
 *
 * \brief Multicast group management implementation
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
 * $Id: nwkGroup.c 9267 2014-03-18 21:46:19Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sysConfig.h"

#ifdef NWK_ENABLE_MULTICAST

/*- Definitions ------------------------------------------------------------*/
#define NWK_GROUP_FREE      0xffff

/*- Prototypes -------------------------------------------------------------*/
static bool nwkGroupSwitch(uint16_t from, uint16_t to);

/*- Variables --------------------------------------------------------------*/
static uint16_t nwkGroups[NWK_GROUPS_AMOUNT];

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
  @brief Initializes the Group module
*****************************************************************************/
void nwkGroupInit(void)
{
  for (uint8_t i = 0; i < NWK_GROUPS_AMOUNT; i++)
    nwkGroups[i] = NWK_GROUP_FREE;
}

/*************************************************************************//**
  @brief Adds node to the @a group
  @param[in] group Group ID
  @return @c true in case of success and @c false otherwise
*****************************************************************************/
bool NWK_GroupAdd(uint16_t group)
{
  return nwkGroupSwitch(NWK_GROUP_FREE, group);
}

/*************************************************************************//**
  @brief Removes node from the @a group
  @param[in] group Group ID
  @return @c true in case of success and @c false otherwise
*****************************************************************************/
bool NWK_GroupRemove(uint16_t group)
{
  return nwkGroupSwitch(group, NWK_GROUP_FREE);
}

/*************************************************************************//**
  @brief Verifies if node is a member of the @a group
  @param[in] group Group ID
  @return @c true if node is a member of the group and @c false otherwise
*****************************************************************************/
bool NWK_GroupIsMember(uint16_t group)
{
  for (uint8_t i = 0; i < NWK_GROUPS_AMOUNT; i++)
    if (group == nwkGroups[i])
      return true;
  return false;
}

/*************************************************************************//**
  @brief Switches records with IDs @a from and @a to in the the group table
  @param[in] from Source group ID
  @param[in] to   Destination group ID
  @return @c true if @a from entry was found and @c false otherwise
*****************************************************************************/
static bool nwkGroupSwitch(uint16_t from, uint16_t to)
{
  for (uint8_t i = 0; i < NWK_GROUPS_AMOUNT; i++)
  {
    if (from == nwkGroups[i])
    {
      nwkGroups[i] = to;
      return true;
    }
  }
  return false;
}

#endif // NWK_ENABLE_MULTICAST
