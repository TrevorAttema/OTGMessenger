/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc_if.h
  * @author  MCD Application Team
  * @brief  configuration of the rtc_if.c instances
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_IF_H__
#define __RTC_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_timer.h"
#include "stm32_systime.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/*!
 * @brief Init RTC hardware
 * @param none
 * @retval none
 */
UTIL_TIMER_Status_t RTC_IF_Init(void);

/*!
 * @brief Set the alarm
 * @note The alarm is set at timeout from timer Reference (TimerContext)
 * @param timeout Duration of the Timer in ticks
 */
UTIL_TIMER_Status_t RTC_IF_StartTimer(uint32_t timeout);

/*!
 * @brief Stop the Alarm
 * @param none
 * @retval none
 */
UTIL_TIMER_Status_t RTC_IF_StopTimer(void);

/*!
 * @brief set timer Reference (TimerContext)
 * @param none
 * @retval  Timer Reference Value in  Ticks
 */
uint32_t RTC_IF_SetTimerContext(void);

/*!
 * @brief Get the RTC timer Reference
 * @param none
 * @retval Timer Value in  Ticks
 */
uint32_t RTC_IF_GetTimerContext(void);

/*!
 * @brief Get the timer elapsed time since timer Reference (TimerContext) was set
 * @param none
 * @retval RTC Elapsed time in ticks
 */
uint32_t RTC_IF_GetTimerElapsedTime(void);

/*!
 * @brief Get the timer value
 * @param none
 * @retval RTC Timer value in ticks
 */
uint32_t RTC_IF_GetTimerValue(void);

/*!
 * @brief Return the minimum timeout in ticks the RTC is able to handle
 * @param none
 * @retval minimum value for a timeout in ticks
 */
uint32_t RTC_IF_GetMinimumTimeout(void);

/*!
 * @brief a delay of delay ms by polling RTC
 * @param delay in ms
 * @retval none
 */
void RTC_IF_DelayMs(uint32_t delay);

/*!
 * @brief converts time in ms to time in ticks
 * @param [IN] time in milliseconds
 * @retval returns time in timer ticks
 */
uint32_t RTC_IF_Convert_ms2Tick(uint32_t timeMilliSec);

/*!
 * @brief converts time in ticks to time in ms
 * @param [IN] time in timer ticks
 * @retval returns time in timer milliseconds
 */
uint32_t RTC_IF_Convert_Tick2ms(uint32_t tick);

/*!
 * @brief Get rtc time
 * @param [OUT] subSeconds in ticks
 * @retval returns time seconds
 */
uint32_t RTC_IF_GetTime(uint16_t *subSeconds);

/*!
 * @brief write seconds in backUp register
 * @Note Used to store seconds difference between RTC time and Unix time
 * @param [IN] time in seconds
 * @retval None
 */
void RTC_IF_BkUp_Write_Seconds(uint32_t Seconds);

/*!
 * @brief reads seconds from backUp register
 * @Note Used to store seconds difference between RTC time and Unix time
 * @param [IN] None
 * @retval Time in seconds
 */
uint32_t RTC_IF_BkUp_Read_Seconds(void);

/*!
 * @brief writes SubSeconds in backUp register
 * @Note Used to store SubSeconds difference between RTC time and Unix time
 * @param [IN] time in SubSeconds
 * @retval None
 */
void RTC_IF_BkUp_Write_SubSeconds(uint32_t SubSeconds);

/*!
 * @brief reads SubSeconds from backUp register
 * @Note Used to store SubSeconds difference between RTC time and Unix time
 * @param [IN] None
 * @retval Time in SubSeconds
 */
uint32_t RTC_IF_BkUp_Read_SubSeconds(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
