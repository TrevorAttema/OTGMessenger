/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MP2667.h"
#include "SX1509B.h"
#include "MC2PA8201.h"
#include "MC2PA8201/colors.h"

#include "main2.h"
#include "queue.h"

#include "stm32_timer.h"
#include "radio_test.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for lcdTask */
osThreadId_t lcdTaskHandle;
const osThreadAttr_t lcdTask_attributes = {
  .name = "lcdTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for keypadTask */
osThreadId_t keypadTaskHandle;
const osThreadAttr_t keypadTask_attributes = {
  .name = "keypadTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for radioDispatcher */
osThreadId_t radioDispatcherHandle;
const osThreadAttr_t radioDispatcher_attributes = {
  .name = "radioDispatcher",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for radioIntTask */
osThreadId_t radioIntTaskHandle;
const osThreadAttr_t radioIntTask_attributes = {
  .name = "radioIntTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for keypadQueue */
osMessageQueueId_t keypadQueueHandle;
const osMessageQueueAttr_t keypadQueue_attributes = {
  .name = "keypadQueue"
};
/* Definitions for keypadEvent */
osEventFlagsId_t keypadEventHandle;
const osEventFlagsAttr_t keypadEvent_attributes = {
  .name = "keypadEvent"
};
/* Definitions for radioInterruptEvent */
osEventFlagsId_t radioInterruptEventHandle;
const osEventFlagsAttr_t radioInterruptEvent_attributes = {
  .name = "radioInterruptEvent"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern void SX1276OnDio0Irq( void );
extern void SX1276OnDio1Irq( void );
extern void SX1276OnDio2Irq( void );
extern void SX1276OnDio3Irq( void );
extern void SX1276OnDio4Irq( void );
extern void RFM95W_RADIO_IoInit( void );
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void lcdTaskEntry(void *argument);
void keypadTaskEntry(void *argument);
void radioTaskDispatcher(void *argument);
void radioIntTaskEntry(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of keypadQueue */
  keypadQueueHandle = osMessageQueueNew (1, sizeof(uint8_t), &keypadQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of lcdTask */
  lcdTaskHandle = osThreadNew(lcdTaskEntry, NULL, &lcdTask_attributes);

  /* creation of keypadTask */
  keypadTaskHandle = osThreadNew(keypadTaskEntry, NULL, &keypadTask_attributes);

  /* creation of radioDispatcher */
  radioDispatcherHandle = osThreadNew(radioTaskDispatcher, NULL, &radioDispatcher_attributes);

  /* creation of radioIntTask */
  radioIntTaskHandle = osThreadNew(radioIntTaskEntry, NULL, &radioIntTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of keypadEvent */
  keypadEventHandle = osEventFlagsNew(&keypadEvent_attributes);

  /* creation of radioInterruptEvent */
  radioInterruptEventHandle = osEventFlagsNew(&radioInterruptEvent_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	//SX1509B_KeyboardLedSet(GPIO_PIN_RESET);
	for (;;)
	{
		osDelay (1);
		//show_3d_cube();
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_lcdTaskEntry */
/**
 * @brief Function implementing the lcdTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_lcdTaskEntry */
void lcdTaskEntry(void *argument)
{
  /* USER CODE BEGIN lcdTaskEntry */
	/* Infinite loop */
	for (;;)
	{
		gfx_thread ();
		//osDelay(1);
	}
  /* USER CODE END lcdTaskEntry */
}

/* USER CODE BEGIN Header_keypadTaskEntry */
/**
 * @brief Function implementing the keypadTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_keypadTaskEntry */
void keypadTaskEntry(void *argument)
{
  /* USER CODE BEGIN keypadTaskEntry */
	/* Infinite loop */

	static uint16_t code = 0;
	static uint8_t key = 0;
	HAL_StatusTypeDef status = HAL_OK;

	for (;;)
	{
		osEventFlagsWait (keypadEventHandle, EVENT_KEYBOARD_INT, osFlagsWaitAny,
											osWaitForever);
		status = SX1509B_ReadScanCode (&code);
		if (status == HAL_OK && code > 0)
		{
			key = SX1509B_TranslateScanCode (code);
			osMessageQueuePut (keypadQueueHandle, &key, 0, osWaitForever);
			//osDelay(100);
		}
	}
  /* USER CODE END keypadTaskEntry */
}

/* USER CODE BEGIN Header_radioTaskDispatcher */
/**
* @brief Function implementing the radioDispatcher thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_radioTaskDispatcher */
void radioTaskDispatcher(void *argument)
{
  /* USER CODE BEGIN radioTaskDispatcher */
  /* Infinite loop */
	RFM95W_RADIO_IoInit ();
	SubghzApp_Init ();
	/* Infinite loop */
	for (;;)
	{
		PingPong_Process();
		osDelay (1);
	}
  /* USER CODE END radioTaskDispatcher */
}

/* USER CODE BEGIN Header_radioIntTaskEntry */
/**
* @brief Function implementing the radioIntTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_radioIntTaskEntry */
void radioIntTaskEntry(void *argument)
{
  /* USER CODE BEGIN radioIntTaskEntry */
	uint32_t flags;
	/* Infinite loop */
	for (;;)
	{
		flags = osEventFlagsWait (	radioInterruptEventHandle,
																RADIO_IRQ_ALL_EVENTS,
																osFlagsWaitAny, osWaitForever  );

		if ((flags & RADIO_IRQ_EVENT_0) == RADIO_IRQ_EVENT_0)
		{
			SX1276OnDio0Irq();
		}

		if ((flags & RADIO_IRQ_EVENT_1) == RADIO_IRQ_EVENT_1)
		{
			SX1276OnDio1Irq();
		}

		if ((flags & RADIO_IRQ_EVENT_2) == RADIO_IRQ_EVENT_2)
		{
			SX1276OnDio2Irq();
		}

		if ((flags & RADIO_IRQ_EVENT_3) == RADIO_IRQ_EVENT_3)
		{
			SX1276OnDio3Irq();
		}

		if ((flags & RADIO_IRQ_EVENT_4) == RADIO_IRQ_EVENT_4)
		{
			SX1276OnDio4Irq();
		}
	}
  /* USER CODE END radioIntTaskEntry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
