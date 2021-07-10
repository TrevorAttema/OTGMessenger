/**
  ******************************************************************************
  * @file    radio_conf.h
  * @author  MCD Application Team
  * @brief   Header of Radio configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RADIO_CONF_H__
#define __RADIO_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
//#include "mw_log_conf.h"     /* mw trace conf */
#include "radio_board_if.h"  /* low layer api (bsp) */
#include "utilities_conf.h"  /* low layer api (bsp) */
#include "sys_debug.h"
/* USER CODE BEGIN include */

/* USER CODE END include */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN Exported_Defines */
/* USER CODE END Exported_Defines */
#if( RADIO_DIOn > 4 )
#define  RADIO_DIO_4_ACTIVE
#endif
#if( RADIO_DIOn > 5 )
#define  RADIO_DIO_5_ACTIVE
#endif

/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
#ifndef CRITICAL_SECTION_BEGIN
/**
  * @brief macro used to enter the critical section
  */
#define CRITICAL_SECTION_BEGIN( )      UTILS_ENTER_CRITICAL_SECTION( )
#endif /* !CRITICAL_SECTION_BEGIN */
#ifndef CRITICAL_SECTION_END
/**
  * @brief macro used to exit the critical section
  */
#define CRITICAL_SECTION_END( )        UTILS_EXIT_CRITICAL_SECTION( )
#endif /* !CRITICAL_SECTION_END */

/* Function mapping */
/**
  * @brief Delay interface to radio Middleware
  */
#define RADIO_DELAY_MS                          osDelay

/**
  * @brief Memcpy utilities interface to radio Middleware
  */
#define RADIO_MEMCPY8( dest, value, size )      UTIL_MEM_cpy_8( (uint8_t *)dest, value, size )

/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __RADIO_CONF_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
