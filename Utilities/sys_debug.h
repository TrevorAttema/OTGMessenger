/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sys_debug.h
  * @author  MCD Application Team
  * @brief   Configuration of the debug.c instances
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
#ifndef __SYS_DEBUG_H__
#define __SYS_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_conf.h"
#include "platform.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* Pin defines */

/**  Definition for Debug Line 1   **/
/**
  * @brief Pin of Debug Line 1
  */
#define DGB_LINE1_PIN                           GPIO_PIN_12

/**
  * @brief Port of Debug Line 1
  */
#define DGB_LINE1_PORT                          GPIOB

/**
  * @brief Enable GPIOs clock of Debug Line 1
  */
#define DGB_LINE1_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()

/**
  * @brief Disable GPIOs clock of Debug Line 1
  */
#define DGB_LINE1_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

/**  Definition for Debug Line 2   **/
/**
  * @brief Pin of Debug Line 2
  */
#define DGB_LINE2_PIN                           GPIO_PIN_13

/**
  * @brief Port of Debug Line 2
  */
#define DGB_LINE2_PORT                          GPIOB

/**
  * @brief Enable GPIOs clock of Debug Line 2
  */
#define DGB_LINE2_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()

/**
  * @brief Disable GPIOs clock of Debug Line 2
  */
#define DGB_LINE2_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

/**  Definition for Debug Line 3   **/
/**
  * @brief Pin of Debug Line 3
  */
#define DGB_LINE3_PIN                           GPIO_PIN_14

/**
  * @brief Port of Debug Line 3
  */
#define DGB_LINE3_PORT                          GPIOB

/**
  * @brief Enable GPIOs clock of Debug Line 3
  */
#define DGB_LINE3_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()

/**
  * @brief Disable GPIOs clock of Debug Line 3
  */
#define DGB_LINE3_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

/**  Definition for Debug Line 4   **/
/**
  * @brief Pin of Debug Line 4
  */
#define DGB_LINE4_PIN                           GPIO_PIN_15

/**
  * @brief Port of Debug Line 4
  */
#define DGB_LINE4_PORT                          GPIOB

/**
  * @brief Enable GPIOs clock of Debug Line 4
  */
#define DGB_LINE4_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()

/**
  * @brief Disable GPIOs clock of Debug Line 4
  */
#define DGB_LINE4_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macro ------------------------------------------------------------*/
#if defined (DEBUGGER_ON) && (DEBUGGER_ON == 1)

#define DBG_GPIO_WRITE( gpio, n, x )  HAL_GPIO_WritePin( gpio, n, (GPIO_PinState)(x) )

/**
  * @brief Set pin to high level
  */
#define DBG_GPIO_SET_LINE( gpio, n )       LL_GPIO_SetOutputPin( gpio, n )

/**
  * @brief Set pin to low level
  */
#define DBG_GPIO_RST_LINE( gpio, n )       LL_GPIO_ResetOutputPin ( gpio, n )

#define DBG( x )  do{ x } while(0)

/*enabling RTC_OUTPUT should be set via MX GUI because MX_RTC_Init is now entirely generated */

#elif defined (DEBUGGER_ON) && (DEBUGGER_ON == 0) /* DEBUGGER_OFF */

/**
  * @brief not usable
  */
#define DBG_GPIO_SET_LINE( gpio, n )

/**
  * @brief not usable
  */
#define DBG_GPIO_RST_LINE( gpio, n )

#define DBG( x ) do{  } while(0)

/*disabling RTC_OUTPUT should be set via MX GUI because MX_RTC_Init is now entirely generated */

#else
#error "DEBUGGER_ON not defined or out of range <0,1>"
#endif /* DEBUGGER_OFF */

/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/**
  * @brief Initializes the debug
  */
void DBG_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __SYS_DEBUG_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
