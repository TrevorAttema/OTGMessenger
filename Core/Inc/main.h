/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EVENT_KEYBOARD_INT 0x00000001U
#define RADIO_INT1_Pin GPIO_PIN_3
#define RADIO_INT1_GPIO_Port GPIOE
#define RADIO_INT2_Pin GPIO_PIN_4
#define RADIO_INT2_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_0
#define LED0_GPIO_Port GPIOC
#define KB_IRQ_Pin GPIO_PIN_0
#define KB_IRQ_GPIO_Port GPIOA
#define KB_IRQ_EXTI_IRQn EXTI0_IRQn
#define AUDIO_EN_Pin GPIO_PIN_2
#define AUDIO_EN_GPIO_Port GPIOA
#define AMP_EN_Pin GPIO_PIN_3
#define AMP_EN_GPIO_Port GPIOA
#define BATT_VSENSE_Pin GPIO_PIN_4
#define BATT_VSENSE_GPIO_Port GPIOC
#define KB_LED_EN_Pin GPIO_PIN_5
#define KB_LED_EN_GPIO_Port GPIOC
#define KB_POWER_END_KEY_Pin GPIO_PIN_1
#define KB_POWER_END_KEY_GPIO_Port GPIOB
#define RADIO_INT3_Pin GPIO_PIN_11
#define RADIO_INT3_GPIO_Port GPIOE
#define RADIO_INT3_EXTI_IRQn EXTI15_10_IRQn
#define RADIO_INT4_Pin GPIO_PIN_12
#define RADIO_INT4_GPIO_Port GPIOE
#define RADIO_INT4_EXTI_IRQn EXTI15_10_IRQn
#define RADIO_INT5_Pin GPIO_PIN_13
#define RADIO_INT5_GPIO_Port GPIOE
#define RADIO_INT5_EXTI_IRQn EXTI15_10_IRQn
#define RADIO_RESET_Pin GPIO_PIN_14
#define RADIO_RESET_GPIO_Port GPIOE
#define RADIO_EN_Pin GPIO_PIN_15
#define RADIO_EN_GPIO_Port GPIOE
#define LCD_RESX_Pin GPIO_PIN_8
#define LCD_RESX_GPIO_Port GPIOD
#define PERIFERALS_EN_Pin GPIO_PIN_9
#define PERIFERALS_EN_GPIO_Port GPIOD
#define FLASH_EN_Pin GPIO_PIN_10
#define FLASH_EN_GPIO_Port GPIOD
#define LCD_BACKLIGHT_PWM_Pin GPIO_PIN_8
#define LCD_BACKLIGHT_PWM_GPIO_Port GPIOA
#define LCD_BACKLIGHT_EN_Pin GPIO_PIN_9
#define LCD_BACKLIGHT_EN_GPIO_Port GPIOA
#define SDCARD_EN_Pin GPIO_PIN_10
#define SDCARD_EN_GPIO_Port GPIOA
#define LCD_TE_Pin GPIO_PIN_15
#define LCD_TE_GPIO_Port GPIOA
#define LCD_TE_EXTI_IRQn EXTI15_10_IRQn
#define uSD_DETECT_EXTI_Pin GPIO_PIN_6
#define uSD_DETECT_EXTI_GPIO_Port GPIOD
#define KB_POWER_END_KEY_EXTI_Pin GPIO_PIN_7
#define KB_POWER_END_KEY_EXTI_GPIO_Port GPIOD
#define BATT_VSENSE_EN_Pin GPIO_PIN_4
#define BATT_VSENSE_EN_GPIO_Port GPIOB
#define BATT_STATUS_INT_Pin GPIO_PIN_5
#define BATT_STATUS_INT_GPIO_Port GPIOB
#define TCXO_ENABLE_Pin GPIO_PIN_0
#define TCXO_ENABLE_GPIO_Port GPIOE
#define RADIO_INT0_Pin GPIO_PIN_1
#define RADIO_INT0_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
