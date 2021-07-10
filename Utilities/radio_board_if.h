/**
  ******************************************************************************
  * @file    radio_board_if.h
  * @author  MCD Application Team
  * @brief   Header for Radio interface configuration
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
#ifndef RADIO_BOARD_IF_H
#define RADIO_BOARD_IF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifdef RFM95W

#include "RFM95W.h"

#define Sx_Board_IoInit            RFM95W_RADIO_IoInit
#define Sx_Board_IoDeInit          RFM95W_RADIO_IoDeInit
#define Sx_Board_IoIrqInit         RFM95W_RADIO_IoIrqInit
#define Sx_Board_SendRecv          RFM95W_RADIO_SendRecv
#define Sx_Board_ChipSelect        RFM95W_RADIO_ChipSelect
#define Sx_Board_CheckRfFrequency  RFM95W_RADIO_CheckRfFrequency
#define Sx_Board_Reset             RFM95W_RADIO_Reset
#define Sx_Board_SetXO             RFM95W_RADIO_SetXO
#define Sx_Board_GetWakeUpTime     RFM95W_RADIO_GetWakeUpTime
#define Sx_Board_GetPaSelect       RFM95W_RADIO_GetPaSelect
#define Sx_Board_SetAntSw          RFM95W_RADIO_SetAntSw
#define Sx_Board_Bus_Init          RFM95W_RADIO_Bus_Init
#define Sx_Board_Bus_deInit        RFM95W_RADIO_Bus_deInit
#define Sx_Board_GetDio1PinState   RFM95W_RADIO_GetDio1PinState

#else
#error "include your board here"
#endif

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* RADIO_BOARD_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

