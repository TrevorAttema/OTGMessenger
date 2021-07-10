/******************************************************************************
  * @file    subghz_phy_version.h
  * @author  MCD Application Team
  * @brief   defines the radio driver version
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
 ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __SUBGHZ_PHY_VERSION_H__
#define __SUBGHZ_PHY_VERSION_H__

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* __SUBGHZ_PHY_TYPE: 0x01  STM32WL
                      0x61  SX126X
                      0x72  SX1272
                      0x76  SX1276 */

#define __SUBGHZ_PHY_TYPE           (0x76U) /*!< [31:24] main version */
#define __SUBGHZ_PHY_VERSION_MAIN   (0x00U) /*!< [23:16] main version */
#define __SUBGHZ_PHY_VERSION_SUB1   (0x00U) /*!< [15:8]  sub1 version */
#define __SUBGHZ_PHY_VERSION_SUB2   (0x01U) /*!< [7:0]   sub2 version */ 
#define __SUBGHZ_PHY_VERSION        ((__SUBGHZ_PHY_VERSION_MAIN << 24)  \
                                     |(__SUBGHZ_PHY_VERSION_SUB1 << 16) \
                                     |(__SUBGHZ_PHY_VERSION_SUB2 << 8 ) \
                                     |(__SUBGHZ_PHY_TYPE))
   
/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 

#ifdef __cplusplus
}
#endif

#endif /*__SUBGHZ_PHY_VERSION_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
