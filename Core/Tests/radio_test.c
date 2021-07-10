/*!
 * \file      subghz_phy_app.c
 *
 * \brief     Ping-Pong implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
/**
  ******************************************************************************
  *
  *          Portions COPYRIGHT 2020 STMicroelectronics
  *
  * @file    subghz_phy_app.c
  * @author  MCD Application Team
  * @brief   Application of the SubGHz_Phy Middleware
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "radio_test.h"

#include "platform.h"
#include "stm32_timer.h"
#include "sys_app.h"
#include "radio.h"
#include "radio_board_if.h"
//#include "utilities_def.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  LOWPOWER,
  RX,
  RX_TIMEOUT,
  RX_ERROR,
  TX,
  TX_TIMEOUT,
} States_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];

States_t State = LOWPOWER;

int8_t RssiValue = 0;
int8_t SnrValue = 0;

/* Led Timers objects*/
static  UTIL_TIMER_Object_t timerLed;

bool isMaster = true;

/* Radio events function pointer */
static RadioEvents_t RadioEvents;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/*!
 * @brief  Function executed on when led timer elapses
 * @param  LED context
 * @retval none
 */
static void OnledEvent(void *context);

/*!
 * @brief Function to be executed on Radio Tx Done event
 * @param  none
 * @retval none
 */
static void OnTxDone(void);

/*!
 * @brief Function to be executed on Radio Rx Done event
 * @param  payload sent
 * @param  payload size
 * @param  rssi
 * @param  snr
 * @retval none
 */
static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

/*!
 * @brief Function executed on Radio Tx Timeout event
 * @param  none
 * @retval none
 */
static void OnTxTimeout(void);

/*!
 * @brief Function executed on Radio Rx Timeout event
 * @param  none
 * @retval none
 */
static void OnRxTimeout(void);

/*!
 * @brief Function executed on Radio Rx Error event
 * @param  none
 * @retval none
 */
static void OnRxError(void);

/*!
 * @brief PingPong state machine implementation
 * @param  none
 * @retval none
 */

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ---------------------------------------------------------*/
void SubghzApp_Init(void)
{
  /* USER CODE BEGIN SubghzApp_Init_1 */

  /* USER CODE END SubghzApp_Init_1 */
  /* Print APP version*/
//  APP_LOG(TS_OFF, VLEVEL_M, "APP_VERSION= V%X.%X.%X\r\n", (uint8_t)(__APP_VERSION >> __APP_VERSION_MAIN_SHIFT),
//          (uint8_t)(__APP_VERSION >> __APP_VERSION_SUB1_SHIFT), (uint8_t)(__APP_VERSION >> __APP_VERSION_SUB2_SHIFT));

  /* Led Timers*/
  UTIL_TIMER_Create(&timerLed, 0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnledEvent, NULL);
  UTIL_TIMER_SetPeriod(&timerLed, LED_PERIOD_MS);

  UTIL_TIMER_Start(&timerLed);

  //return;

  /* Radio initialization */
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;

  Radio.Init(&RadioEvents);

#if (( USE_MODEM_LORA == 1 ) && ( USE_MODEM_FSK == 0 ))
  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);

  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

  Radio.SetMaxPayloadLength(MODEM_LORA, BUFFER_SIZE);

#elif (( USE_MODEM_LORA == 0 ) && ( USE_MODEM_FSK == 1 ))

  Radio.SetTxConfig(MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
                    FSK_DATARATE, 0,
                    FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, 0, TX_TIMEOUT_VALUE);

  Radio.SetRxConfig(MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                    0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                    0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
                    0, 0, false, true);

  Radio.SetMaxPayloadLength(MODEM_FSK, BUFFER_SIZE);

#else
#error "Please define a frequency band in the sys_conf.h file."
#endif /* USE_MODEM_LORA | USE_MODEM_FSK */

  Radio.SetChannel(RF_FREQUENCY);


  Radio.Rx(RX_TIMEOUT_VALUE);


  /* USER CODE BEGIN SubghzApp_Init_2 */

  /* USER CODE END SubghzApp_Init_2 */
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private functions ---------------------------------------------------------*/
void PingPong_Process(void)
{
  /* USER CODE BEGIN PingPong_Process_1 */

  /* USER CODE END PingPong_Process_1 */
  uint8_t i;

  switch (State)
  {
    case RX:

      if (isMaster == true)
      {
        if (BufferSize > 0)
        {
          if (strncmp((const char *)Buffer, (const char *)PongMsg, 4) == 0)
          {
            UTIL_TIMER_Stop(&timerLed);

            /* Send the next PING frame */
            Buffer[0] = 'P';
            Buffer[1] = 'I';
            Buffer[2] = 'N';
            Buffer[3] = 'G';
            /* We fill the buffer with numbers for the payload */
            for (i = 4; i < BufferSize; i++)
            {
              Buffer[i] = i - 4;
            }

            Radio.SetChannel(RF_FREQUENCY);

            /* Add delay between TX and RX =
            time Busy_signal is ON in RX opening window */
            HAL_Delay(Sx_Board_GetWakeUpTime() + TCXO_WORKAROUND_TIME_MARGIN);

            Radio.Send(Buffer, BufferSize);
          }
          else if (strncmp((const char *)Buffer, (const char *)PingMsg, 4) == 0)
          {
            /* A master already exists then become a slave */
            isMaster = false;

            Radio.SetChannel(RF_FREQUENCY);
            Radio.Rx(RX_TIMEOUT_VALUE);
          }
          else /* valid reception but neither a PING or a PONG message */
          {
            /* Set device as master and start again */
            isMaster = true;

            Radio.SetChannel(RF_FREQUENCY);
            Radio.Rx(RX_TIMEOUT_VALUE);
          }
        }
      }
      else
      {
        if (BufferSize > 0)
        {
          if (strncmp((const char *)Buffer, (const char *)PingMsg, 4) == 0)
          {
            /* Indicates on a LED that the received frame is a PING */
            UTIL_TIMER_Stop(&timerLed);


            /* Send the reply to the PONG string */
            Buffer[0] = 'P';
            Buffer[1] = 'O';
            Buffer[2] = 'N';
            Buffer[3] = 'G';
            /* We fill the buffer with numbers for the payload */
            for (i = 4; i < BufferSize; i++)
            {
              Buffer[i] = i - 4;
            }

            Radio.SetChannel(RF_FREQUENCY);

            /* Add delay between TX and RX =
            time Busy_signal is ON in RX opening window */
            HAL_Delay(Sx_Board_GetWakeUpTime() + TCXO_WORKAROUND_TIME_MARGIN);

            Radio.Send(Buffer, BufferSize);

          }
          else /* valid reception but not a PING as expected */
          {
            /* Set device as master and start again */
            isMaster = true;

            Radio.SetChannel(RF_FREQUENCY);
            Radio.Rx(RX_TIMEOUT_VALUE);
          }
        }
      }
      State = LOWPOWER;
      break;
    case TX:
      /* Indicates on a LED that we have sent a PING [Master] */
      /* Indicates on a LED that we have sent a PONG [Slave] */
      Radio.SetChannel(RF_FREQUENCY);

      Radio.Rx(RX_TIMEOUT_VALUE);
      State = LOWPOWER;
      break;
    case RX_TIMEOUT:
    case RX_ERROR:
      if (isMaster == true)
      {
        /* Send the next PING frame */
        Buffer[0] = 'P';
        Buffer[1] = 'I';
        Buffer[2] = 'N';
        Buffer[3] = 'G';
        for (i = 4; i < BufferSize; i++)
        {
          Buffer[i] = i - 4;
        }

        Radio.SetChannel(RF_FREQUENCY);
        /* Add delay between TX and RX =
        time Busy_signal is ON in RX opening window */
        HAL_Delay(Sx_Board_GetWakeUpTime() + TCXO_WORKAROUND_TIME_MARGIN);

        Radio.Send(Buffer, BufferSize);
      }
      else
      {

        Radio.SetChannel(RF_FREQUENCY);
        Radio.Rx(RX_TIMEOUT_VALUE);
      }
      State = LOWPOWER;
      break;
    case TX_TIMEOUT:
      Radio.SetChannel(RF_FREQUENCY);

      Radio.Rx(RX_TIMEOUT_VALUE);
      State = LOWPOWER;
      break;
    case LOWPOWER:
    default:
      /* Set low power */
      break;
  }
  /* USER CODE BEGIN PingPong_Process_2 */

  /* USER CODE END PingPong_Process_2 */
}

static void OnTxDone(void)
{
  /* USER CODE BEGIN OnTxDone_1 */

  /* USER CODE END OnTxDone_1 */

  Radio.Sleep();
  State = TX;

}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
  /* USER CODE BEGIN OnRxDone_1 */

  /* USER CODE END OnRxDone_1 */

  Radio.Sleep();
  BufferSize = size;
  memcpy(Buffer, payload, BufferSize);
  RssiValue = rssi;
  SnrValue = snr;
  State = RX;

  /* USER CODE BEGIN OnRxDone_2 */

  /* USER CODE END OnRxDone_2 */
}

static void OnTxTimeout(void)
{
  /* USER CODE BEGIN OnTxTimeout_1 */

  /* USER CODE END OnTxTimeout_1 */

  Radio.Sleep();
  State = TX_TIMEOUT;

  /* USER CODE BEGIN OnTxTimeout_2 */

  /* USER CODE END OnTxTimeout_2 */
}

static void OnRxTimeout(void)
{
  /* USER CODE BEGIN OnRxTimeout_1 */

  /* USER CODE END OnRxTimeout_1 */

  Radio.Sleep();
  State = RX_TIMEOUT;
  /* USER CODE BEGIN OnRxTimeout_2 */

  /* USER CODE END OnRxTimeout_2 */
}

static void OnRxError(void)
{
  /* USER CODE BEGIN OnRxError_1 */

  /* USER CODE END OnRxError_1 */

  Radio.Sleep();
  State = RX_ERROR;

  /* USER CODE BEGIN OnRxError_2 */

  /* USER CODE END OnRxError_2 */
}

static void OnledEvent(void *context)
{
  /* USER CODE BEGIN OnledEvent_1 */

  /* USER CODE END OnledEvent_1 */

  UTIL_TIMER_Start(&timerLed);
  /* USER CODE BEGIN OnledEvent_2 */

  /* USER CODE END OnledEvent_2 */
}

/* USER CODE BEGIN PrFD */

/* USER CODE END PrFD */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
