/* Includes ------------------------------------------------------------------*/

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "RFM95W.h"


#define RF_MID_BAND_THRESH                          525000000


/* Private variables ---------------------------------------------------------*/
static const uint32_t RADIO_DIO_EXTI_LINE[RADIO_DIOn] =
{
  RADIO_DIO_0_EXTI_LINE,
  RADIO_DIO_1_EXTI_LINE,
  RADIO_DIO_2_EXTI_LINE,
  RADIO_DIO_3_EXTI_LINE,
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4_EXTI_LINE,
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5_EXTI_LINE,
#endif
};

static const uint32_t RADIO_DIO_IT_PRIO [RADIO_DIOn] =
{
  RADIO_DIO_0_IT_PRIO,
  RADIO_DIO_1_IT_PRIO,
  RADIO_DIO_2_IT_PRIO,
  RADIO_DIO_3_IT_PRIO,
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4_IT_PRIO,
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5_IT_PRIO,
#endif
};

static const IRQn_Type RADIO_DIO_IRQn [RADIO_DIOn]   =
{
  RADIO_DIO_0_IRQn,
  RADIO_DIO_1_IRQn,
  RADIO_DIO_2_IRQn,
  RADIO_DIO_3_IRQn,
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4_IRQn,
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5_IRQn,
#endif
};

static GPIO_TypeDef *RADIO_DIO_PORT[RADIO_DIOn] =
{
  RADIO_DIO_0_PORT,
  RADIO_DIO_1_PORT,
  RADIO_DIO_2_PORT,
  RADIO_DIO_3_PORT,
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4_PORT,
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5_PORT,
#endif
};

static const uint16_t RADIO_DIO_PIN[RADIO_DIOn] =
{
	RADIO_DIO_0_PIN,
  RADIO_DIO_1_PIN,
  RADIO_DIO_2_PIN,
  RADIO_DIO_3_PIN,
#if( RADIO_DIOn > 4 )
  RADIO_DIO_4_PIN,
#endif
#if( RADIO_DIOn > 5 )
  RADIO_DIO_5_PIN,
#endif
};

static const uint32_t RADIO_DIO_MODE[RADIO_DIOn] =
{
  GPIO_MODE_IT_RISING,
  GPIO_MODE_IT_RISING_FALLING,
  GPIO_MODE_IT_RISING,
  GPIO_MODE_IT_RISING,
#if( RADIO_DIOn > 4 )
  GPIO_MODE_IT_RISING,
#endif
#if( RADIO_DIOn > 5 )
  GPIO_MODE_IT_RISING,
#endif
};

/* static */ EXTI_HandleTypeDef hRADIO_DIO_exti[RADIO_DIOn];

/* Private function prototypes -----------------------------------------------*/

static void RFM95W_RADIO_SPI_IoInit(SPI_HandleTypeDef *spiHandle);
static void RFM95W_RADIO_SPI_IoDeInit(void);

/* Exported functions ---------------------------------------------------------*/
uint32_t RFM95W_RADIO_GetWakeUpTime(void)
{
  return  BOARD_WAKEUP_TIME;
}

void RFM95W_RADIO_SetXO(uint8_t state)
{
}

void RFM95W_RADIO_IoInit(void)
{
//  GPIO_InitTypeDef initStruct = {0};
//  SPI_HandleTypeDef dummy_hspi;
//
//  /* Enable DIO GPIO clock */
//  RADIO_DIO_0_GPIO_CLK_ENABLE();
//  RADIO_DIO_1_GPIO_CLK_ENABLE();
//  RADIO_DIO_2_GPIO_CLK_ENABLE();
//  RADIO_DIO_3_GPIO_CLK_ENABLE();
//#if( RADIO_DIOn > 4 )
//  RADIO_DIO_4_GPIO_CLK_ENABLE();
//#endif
//#if( RADIO_DIOn > 5 )
//  RADIO_DIO_5_GPIO_CLK_ENABLE();
//#endif
//
//  /* DIO IO Init */
//  initStruct.Pull = GPIO_PULLDOWN;
//  initStruct.Speed = GPIO_SPEED_HIGH;
//
//  for (uint32_t i = 0; i < RADIO_DIOn ; i++)
//  {
//    initStruct.Mode = RADIO_DIO_MODE[i];
//    initStruct.Pin = RADIO_DIO_PIN[i];
//    HAL_GPIO_Init(RADIO_DIO_PORT[i], &initStruct);
//  }

  //* Antenna IO Init */
//  RADIO_ANT_CLK_ENABLE();

//  initStruct.Pin = RADIO_ANT_SWITCH_PIN;
//  initStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  initStruct.Pull = GPIO_NOPULL;
//  initStruct.Speed = GPIO_SPEED_HIGH;
//
//  HAL_GPIO_Init(RADIO_ANT_SWITCH_PORT, &initStruct);

  /* SPI IO Init */
  /* Normally done by the HAL_MSP callback but not for this applic */
  // RFM95W_RADIO_SPI_IoInit(&dummy_hspi);

  /* NSS initialization */
//  initStruct.Pin = RADIO_NSS_PIN;
//  initStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  initStruct.Pull = GPIO_PULLUP;
//
//  /* Enable NSS */
//  RADIO_NSS_CLK_ENABLE();
//  HAL_GPIO_Init(RADIO_NSS_PORT, &initStruct);
//  HAL_GPIO_WritePin(RADIO_NSS_PORT, RADIO_NSS_PIN, GPIO_PIN_SET);

	HAL_GPIO_WritePin(RADIO_EN_GPIO_Port, RADIO_EN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RADIO_NSS_PORT, RADIO_NSS_PIN, GPIO_PIN_SET);


}

void RFM95W_RADIO_IoDeInit(void)
{
//  GPIO_InitTypeDef initStruct = {0};
//
//  /* DIO IO DeInit */
//
//  initStruct.Pull = GPIO_PULLDOWN;
//
//  for (uint32_t i = 0; i < RADIO_DIOn ; i++)
//  {
//    initStruct.Mode = RADIO_DIO_MODE[i];
//    initStruct.Pin = RADIO_DIO_PIN[i];
//    HAL_GPIO_Init(RADIO_DIO_PORT[i], &initStruct);
//  }

  /* SPI IO DeInit */
//  RFM95W_RADIO_SPI_IoDeInit();

  /* NSS IO DeInit is not done */

  /* Antenna IO DeInit is not done */

  /* Reset IO DeInit is not done */
	HAL_GPIO_WritePin(RADIO_EN_GPIO_Port, RADIO_EN_Pin, GPIO_PIN_RESET);

}

void RFM95W_RADIO_IoIrqInit(DioIrqHandler **irqHandlers)
{
  CRITICAL_SECTION_BEGIN();
  for (uint32_t i = 0; i < RADIO_DIOn ; i++)
  {
    HAL_EXTI_GetHandle(&hRADIO_DIO_exti[i], RADIO_DIO_EXTI_LINE[i]);
    HAL_EXTI_RegisterCallback(&hRADIO_DIO_exti[i], HAL_EXTI_COMMON_CB_ID, irqHandlers[i]);
    //HAL_NVIC_SetPriority(RADIO_DIO_IRQn[i], RADIO_DIO_IT_PRIO[i], 0x00);
    //HAL_NVIC_EnableIRQ(RADIO_DIO_IRQn[i]);
  }
  CRITICAL_SECTION_END();
}

/*!
 * @brief it points the demanded line in the EXTI_HandleTypeDef TABLE before calling HAL_EXTI_IRQHandler()
 * @param [IN]: DIO extiLine demanded
 * @retval none
 * @note "stm32h7xx_it.c" code generated by MX do not call this function but directly the HAL_EXTI_IRQHandler()
 */
void RFM95W_RADIO_IRQHandler(RADIO_DIO_TypeDef DIO)
{
  HAL_EXTI_IRQHandler(&hRADIO_DIO_exti[DIO]);
}

TxConfig_TypeDef RFM95W_RADIO_GetPaSelect(uint32_t channel)
{
  if (channel > RF_MID_BAND_THRESH)
  {
    return CONF_RFO_HP;
  }
  else
  {
    return CONF_RFO_LF;
  }
}

void RFM95W_RADIO_SetAntSw(RfSw_TypeDef state)
{

}

bool RFM95W_RADIO_CheckRfFrequency(uint32_t frequency)
{
  // Implement check. Currently all frequencies are supported
  return true;
}

void RFM95W_RADIO_Reset(void)
{
  GPIO_InitTypeDef initStruct = { 0 };

  initStruct.Mode = GPIO_MODE_OUTPUT_PP;
  initStruct.Pull = GPIO_NOPULL;
  initStruct.Speed = GPIO_SPEED_HIGH;
  initStruct.Pin = RADIO_RESET_PIN;

  // Set RESET pin to 0
  HAL_GPIO_Init(RADIO_RESET_PORT, &initStruct);
  HAL_GPIO_WritePin(RADIO_RESET_PORT, RADIO_RESET_PIN, GPIO_PIN_RESET);

  // Wait 1 ms
  osDelay(1);

  // Configure RESET as input
  initStruct.Mode = GPIO_NOPULL;
  HAL_GPIO_Init(RADIO_RESET_PORT, &initStruct);

  // Wait 6 ms
  osDelay(6);
}

/* Bus mapping to SPI */

void RFM95W_RADIO_Bus_Init(void)
{
  RADIO_SPI_Init();
}

void RFM95W_RADIO_Bus_deInit(void)
{
  RADIO_SPI_DeInit();
}

/**
  * @brief  Send Receive data via SPI
  * @param  Data to send
  * @retval Received data
  */
uint16_t RFM95W_RADIO_SendRecv(uint16_t txData)
{
  uint16_t rxData ;

  /*RADIO_SPI_SendRecv(( uint8_t * ) &txData, ( uint8_t* ) &rxData, 1);*/
  RADIO_SPI_SendRecv((uint8_t *) &txData, (uint8_t *) &rxData, 1);

  return rxData;
}

void RFM95W_RADIO_ChipSelect(int32_t state)
{
  if (state == 0)
  {
    HAL_GPIO_WritePin(RADIO_NSS_PORT, RADIO_NSS_PIN, GPIO_PIN_RESET);
  }
  else
  {
    HAL_GPIO_WritePin(RADIO_NSS_PORT, RADIO_NSS_PIN, GPIO_PIN_SET);
  }
}

uint32_t RFM95W_RADIO_GetDio1PinState(void)
{
  return HAL_GPIO_ReadPin(RADIO_DIO_1_PORT, RADIO_DIO_1_PIN);
}
/* Private functions --------------------------------------------------------*/

/**
  * @brief  Initializes SPI MSP.
  * @param  hspi  SPI handler
  * @retval None
*/
static void RFM95W_RADIO_SPI_IoInit(SPI_HandleTypeDef *spiHandle)
{
//  GPIO_InitTypeDef GPIO_InitStruct;
//  /* USER CODE BEGIN SPI1_MspInit 0 */
//
//  /* USER CODE END SPI1_MspInit 0 */
//  /* Enable Peripheral clock */
//  RADIO_SPI_SCK_GPIO_CLK_ENABLE();
//  RADIO_SPI_MOSI_GPIO_CLK_ENABLE();
//  RADIO_SPI_MISO_GPIO_CLK_ENABLE();
//
//  /**SPI1 GPIO Configuration
//  PA5     ------> SPI1_SCK
//  PA6     ------> SPI1_MISO
//  PA7     ------> SPI1_MOSI
//    */
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//
//  GPIO_InitStruct.Alternate = 0;
//  GPIO_InitStruct.Pin = RADIO_SPI_MOSI_GPIO_PIN;
//  HAL_GPIO_Init(RADIO_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
//  GPIO_InitStruct.Alternate = 0;
//  GPIO_InitStruct.Pin = RADIO_SPI_MISO_GPIO_PIN;
//  HAL_GPIO_Init(RADIO_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);
//  GPIO_InitStruct.Alternate = 0;
//  GPIO_InitStruct.Pin = RADIO_SPI_SCK_GPIO_PIN;
//  HAL_GPIO_Init(RADIO_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  DeInitializes SPI MSP.
  * @param  hspi  SPI handler
  * @retval None
  */
static void RFM95W_RADIO_SPI_IoDeInit(void)
{
//  GPIO_InitTypeDef GPIO_InitStruct;
//
//  /* Peripheral clock disable */
//  /* no need to call SPI1_CLK_DISABLE() because going in LowPower it gets disabled automatically */
//
//  /* DeInitialize Peripheral GPIOs */
//  /* Instead of using HAL_GPIO_DeInit() which set ANALOG mode
//     it's preferred to set in OUTPUT_PP mode, with the pins set to 0 */
//
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//  GPIO_InitStruct.Pin = RADIO_SPI_MOSI_GPIO_PIN;
//  HAL_GPIO_Init(RADIO_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
//  GPIO_InitStruct.Pin = RADIO_SPI_MISO_GPIO_PIN;
//  HAL_GPIO_Init(RADIO_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);
//  GPIO_InitStruct.Pin = RADIO_SPI_SCK_GPIO_PIN;
//  HAL_GPIO_Init(RADIO_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);
//
//
  HAL_GPIO_WritePin(RADIO_SPI_MOSI_GPIO_PORT, RADIO_SPI_MOSI_GPIO_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RADIO_SPI_MISO_GPIO_PORT, RADIO_SPI_MISO_GPIO_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(RADIO_SPI_SCK_GPIO_PORT, RADIO_SPI_SCK_GPIO_PIN, GPIO_PIN_RESET);

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
