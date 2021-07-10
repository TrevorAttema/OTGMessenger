/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: driver specific target board functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
  */
/**
  ******************************************************************************
  * @file    sx1276mb1las.h
  * @author  MCD Application Team
  * @brief   driver for sx1276mb1las board
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
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
#ifndef __RFM95W__
#define __RFM95W__

#ifdef __cplusplus
extern "C"
{
#endif



/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "RFM95W_conf.h"


/**
  * @brief Define for SX1276MB1XAS board
  */
#if !defined (USE_RFM95W)
#define USE_RFM95W
#endif
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  RFSW_OFF = 0,
  RFSW_RX,
  RFSW_RFO_LP,
  RFSW_RFO_HP,
  RFSW_RFO_LF,
} RfSw_TypeDef;

typedef enum
{
  CONF_RFO_LP_HP = 0,
  CONF_RFO_LP = 1,
  CONF_RFO_HP = 2,
  CONF_RFO_LF = 3,
} TxConfig_TypeDef;

/* RADIO_DIO_TypeDef is defined in case RFM95W_RADIO_IRQHandler() API is used */
typedef enum
{
  RADIO_DIO_0 = 0U,
  RADIO_DIO_1 = 1U,
  RADIO_DIO_2 = 2U,
  RADIO_DIO_3 = 3U,
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4 = 4U,
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5 = 5U,
#endif
} RADIO_DIO_TypeDef;

/* Hardware IO IRQ callback function definition */
typedef void (DioIrqHandler)(void);

/* Exported constants --------------------------------------------------------*/

#define BOARD_WAKEUP_TIME  0 // no TCXO

/* Exported variables------------------------------------------------------- */
extern EXTI_HandleTypeDef hRADIO_DIO_exti[RADIO_DIOn];

/* Exported functions ------------------------------------------------------- */

/*!
 * \brief Initializes the radio I/Os pins interface
 */
void RFM95W_RADIO_IoInit(void);

/*!
 * \brief De-initializes the radio I/Os pins interface.
 *
 * \remark Useful when going in MCU low power modes
 */
void RFM95W_RADIO_IoDeInit(void);

/*!
 * \brief Register Radio Irq Handler.
 *
 * \param array of callbacks wrt to DIO numbers
 */
void RFM95W_RADIO_IoIrqInit(DioIrqHandler **irqHandlers);

/*!
 * \brief Send a command that write data to the radio
 *
 * \param  [in]   txData        data to be sent
 * \return [out]  rxdata        data to be read
 */
uint16_t RFM95W_RADIO_SendRecv(uint16_t txData);

/*!
 * \brief Chip select of the board
 *
 * \param  [in]   state       0 state is 0, 1 otherwise
 */
void RFM95W_RADIO_ChipSelect(int32_t state);

/*!
 * \brief Gets current state of DIO1 pin state (FifoLevel).
 *
 * \retval state DIO1 pin current state.
 */
uint32_t RFM95W_RADIO_GetDio1PinState(void);
/*!
 * \brief Checks if the given RF frequency is supported by the hardware
 *
 * \param [IN] frequency RF frequency to be checked
 * \retval isSupported [true: supported, false: unsupported]
 */
bool RFM95W_RADIO_CheckRfFrequency(uint32_t frequency);

/*!
 * \brief Resets the Radio
 */
void RFM95W_RADIO_Reset(void);

/*!
 * \brief Set the XO on/off
 *
 * \param [IN] 0 off, 1 On
 * \remark not applicable for this board
 */
void RFM95W_RADIO_SetXO(uint8_t state);

/*!
 * \brief Get the wake up time of the board
 *
 * \return value time in ms
 */
uint32_t RFM95W_RADIO_GetWakeUpTime(void);

/*!
 * \brief Get the PA configuration of the board
 * \param  channel frequency at which the the transmission will occur
 * \return PA configuration of the board
 */
TxConfig_TypeDef RFM95W_RADIO_GetPaSelect(uint32_t channel);

/*!
 * \brief Set the antenna switch on the requested input
 * \param  requested input
 */
void RFM95W_RADIO_SetAntSw(RfSw_TypeDef state);

/*!
 * \brief SPI Bus init
 */
void RFM95W_RADIO_Bus_Init(void);

/*!
 * \brief SPI Bus Deinit
 */
void RFM95W_RADIO_Bus_deInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __RFM95W__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
