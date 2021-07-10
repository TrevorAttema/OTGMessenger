/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "b_l072z_lrwan1_bus.h"

#define TIMEOUT_DURATION 1000

extern SPI_HandleTypeDef hspi1;

void		BSP_SPI1_Init() {

}

void 		BSP_SPI1_DeInit() {

}


/**
  * @brief  Write Data through SPI BUS.
  * @param  pData: Data
  * @param  len: Length of data in byte
  * @retval BSP status
  */
int32_t BSP_SPI1_Send(uint8_t *pData, uint16_t len)
{
  int32_t ret = -1;

  if (HAL_SPI_Transmit(&hspi1, pData, len, TIMEOUT_DURATION) == HAL_OK)
  {
    ret = len;
  }
  return ret;
}

/**
  * @brief  Receive Data from SPI BUS
  * @param  pData: Data
  * @param  len: Length of data in byte
  * @retval BSP status
  */
int32_t  BSP_SPI1_Recv(uint8_t *pData, uint16_t len)
{
  int32_t ret = -1;

  if (HAL_SPI_Receive(&hspi1, pData, len, TIMEOUT_DURATION) == HAL_OK)
  {
    ret = len;
  }
  return ret;
}

/**
  * @brief  Send and Receive data to/from SPI BUS (Full duplex)
  * @param  pData: Data
  * @param  len: Length of data in byte
  * @retval BSP status
  */
int32_t BSP_SPI1_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t len)
{
  int32_t ret = -1;

  if (HAL_SPI_TransmitReceive(&hspi1, pTxData, pRxData, len, TIMEOUT_DURATION) == HAL_OK)
  {
    ret = len;
  }
  return ret;
}
