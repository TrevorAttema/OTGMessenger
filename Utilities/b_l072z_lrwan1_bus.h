/**
  ******************************************************************************
  * @file           : b_l072z_lrwan1_bus.h
  * @brief          : header file for the BSP BUS IO driver
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
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
#ifndef __SPI_XMIT_IF__
#define __SPI_XMIT_IF__

#ifdef __cplusplus
extern "C" {
#endif

/* Needed for SPI_GetPrescaler */
#define RADIO_SPI_BAUDRATE                  10000000U /* 16M Sigfox, 10M Lora */

#define BUS_SPI1_SCK_GPIO_PIN            		SPI1_SCK_Pin
#define BUS_SPI1_MISO_GPIO_PIN           		SPI1_MISO_Pin
#define BUS_SPI1_MOSI_GPIO_PIN           		SPI1_MOSI_Pin
#define BUS_SPI1_SCK_GPIO_PORT           		SPI1_SCK_GPIO_Port
#define BUS_SPI1_MISO_GPIO_PORT          		SPI1_MISO_GPIO_Port
#define BUS_SPI1_MOSI_GPIO_PORT          		SPI1_MOSI_GPIO_Port
#define BUS_SPI1_SCK_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_MOSI_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_SPI1_MISO_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* BUS IO driver over SPI Peripheral */

void		BSP_SPI1_Init();
void 		BSP_SPI1_DeInit();
int32_t BSP_SPI1_Send(uint8_t *pData, uint16_t len);
int32_t BSP_SPI1_Recv(uint8_t *pData, uint16_t len);
int32_t BSP_SPI1_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t len);


#ifdef __cplusplus
}
#endif

#endif /* __SPI_XMIT_IF__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
