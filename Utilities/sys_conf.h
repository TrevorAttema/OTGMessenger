/**
  ******************************************************************************
  * @file    sys_conf.h
  * @author  MCD Application Team
  * @brief   Applicative configuration, e.g. : debug, trace, low power, sensors
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
#ifndef __SYS_CONF_H__
#define __SYS_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/

/**
  * @brief  Verbose level for all trace logs
  */
#define VERBOSE_LEVEL     VLEVEL_M

/**
  * @brief Enable trace logs
  */
#define APP_LOG_ENABLED   1

/**
  * @brief Enable Debugger mode
  * @note  1:ON it enables the debbugger plus 4 dgb pins, 0:OFF the debugger is OFF (lower consumption)
  */
#define DEBUGGER_ON       0

/**
  * @brief Disable Low Power mode
  * @note  0: LowPowerMode enabled. MCU enters stop2 mode, 1: LowPowerMode disabled. MCU enters sleep mode only
  */
#define LOW_POWER_DISABLE 		0

/* USER CODE BEGIN EC */
#define USE_MODEM_LORA				1
/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __SYS_CONF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
