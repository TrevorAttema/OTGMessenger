/**
  ******************************************************************************
  * @file    timer.h
  * @author  MCD Application Team
  * @brief   Wrapper to timer server
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
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_timer.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/**
  * @brief Max timer mask
  */
#define TIMERTIME_T_MAX ( ( uint32_t )~0 )

/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macro ------------------------------------------------------------*/
/**
  * @brief Timer value on 32 bits
  */
#define TimerTime_t UTIL_TIMER_Time_t

/**
  * @brief Timer object description
  */
#define TimerEvent_t UTIL_TIMER_Object_t

/**
  * @brief Create the timer object
  */
#define TimerInit(HANDLE, CB) do {\
                                   UTIL_TIMER_Create( HANDLE, TIMERTIME_T_MAX, UTIL_TIMER_ONESHOT, CB, NULL);\
                                 } while(0)

/**
  * @brief update the period and start the timer
  */
#define TimerSetValue(HANDLE, TIMEOUT) do{ \
                                           UTIL_TIMER_SetPeriod(HANDLE, TIMEOUT);\
                                         } while(0)

/**
  * @brief Start and adds the timer object to the list of timer events
  */
#define TimerStart(HANDLE)   do {\
                                  UTIL_TIMER_Start(HANDLE);\
                                } while(0)

/**
  * @brief Stop and removes the timer object from the list of timer events
  */
#define TimerStop(HANDLE)   do {\
                                 UTIL_TIMER_Stop(HANDLE);\
                               } while(0)

/**
  * @brief return the current time
  */
#define TimerGetCurrentTime  UTIL_TIMER_GetCurrentTime

/**
  * @brief return the elapsed time
  */
#define TimerGetElapsedTime UTIL_TIMER_GetElapsedTime

/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
