/*!
 * \file      timer.h
 *
 * \brief     Timer objects and scheduling management implementation
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

/******************************************************************************
 * @file    stm32_timer.h
 * @author  MCD Application Team
 * @brief   This is the header of the timer server driver
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#ifndef UTIL_TIME_SERVER_H__
#define UTIL_TIME_SERVER_H__

#ifdef __cplusplus

 extern "C" {
#endif

 /** @defgroup TIMER_SERVER timer server
   * @{
   */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>   
#include <cmsis_compiler.h>
#include "utilities_conf.h"
   
/* Exported types ------------------------------------------------------------*/
/** @defgroup TIMER_SERVER_exported_TypeDef TIMER_SERVER exported Typedef
  *  @{
  */

/**
  * @brief Timer mode
  */
typedef enum {
  UTIL_TIMER_ONESHOT  = 0, /*!<One-shot timer. */
  UTIL_TIMER_PERIODIC = 1  /*!<Periodic timer. */
} UTIL_TIMER_Mode_t;

  
/**
  * @brief Timer status
  */
typedef enum {
  UTIL_TIMER_OK            = 0,  /*!<Operation terminated successfully.*/
  UTIL_TIMER_INVALID_PARAM = 1,  /*!<Invalid Parameter.                */
  UTIL_TIMER_HW_ERROR      = 2,  /*!<Hardware Error.                   */
  UTIL_TIMER_UNKNOWN_ERROR = 3   /*!<Unknown Error.                    */
} UTIL_TIMER_Status_t;

/**
  * @brief Timer object description
  */
typedef struct TimerEvent_s
{
    uint32_t Timestamp;           /*!<Expiring timer value in ticks from TimerContext */
    uint32_t ReloadValue;         /*!<Reload Value when Timer is restarted            */
    uint8_t IsPending;            /*!<Is the timer waiting for an event               */
    uint8_t IsRunning;            /*!<Is the timer running                            */
    uint8_t IsReloadStopped;      /*!<Is the reload stopped                           */
    UTIL_TIMER_Mode_t Mode;       /*!<Timer type : one-shot/continuous                */
    void ( *Callback )( void *);  /*!<callback function                               */
    void *argument;               /*!<callback argument                               */
	struct TimerEvent_s *Next;    /*!<Pointer to the next Timer object.               */
} UTIL_TIMER_Object_t;

/**
  * @brief Timer driver definition
  */
typedef struct
{
    UTIL_TIMER_Status_t   (* InitTimer )( void );                  /*!< Initialisation of the low layer timer    */
    UTIL_TIMER_Status_t   (* DeInitTimer )( void );                /*!< Un-Initialisation of the low layer timer */
      
    UTIL_TIMER_Status_t   (* StartTimerEvt )( uint32_t timeout );  /*!< Start the low layer timer */
    UTIL_TIMER_Status_t   (* StopTimerEvt )( void);                /*!< Stop the low layer timer */
    
    uint32_t              (* SetTimerContext)( void );             /*!< Set the timer context */
    uint32_t              (* GetTimerContext)( void );             /*!< Get the timer context */
    
    uint32_t              (* GetTimerElapsedTime)( void );         /*!< Get elapsed time */
    uint32_t              (* GetTimerValue)( void );               /*!< Get timer value */
    uint32_t              (* GetMinimumTimeout)( void );           /*!< Get Minimum timeout */
    
    uint32_t              (* ms2Tick)( uint32_t timeMicroSec );    /*!< convert ms to tick */
    uint32_t              (* Tick2ms)( uint32_t tick );            /*!< convert tick into ms */
} UTIL_TIMER_Driver_s;

/**
  * @brief Timer value on 32 bits
  */
typedef uint32_t UTIL_TIMER_Time_t;
/**
  *  @}
  */

/* Exported variables ------------------------------------------------------------*/
/** @defgroup TIMER_SERVER_exported_Variable TIMER_SERVER exported Variable
  *  @{
  */
/**
 * @brief low layer interface to handle timing execution
 *
 * @remark This structure is defined and initialized in the specific platform
 *         timer implementation
 */
extern const UTIL_TIMER_Driver_s UTIL_TimerDriver;

/**
  *  @}
  */

/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 

/** @defgroup TIMER_SERVER_exported_function TIMER_SERVER exported function
  *  @{
  */

/**
  * @brief Initialize the timer server
  *
  * @retval Status based on @ref UTIL_TIMER_Status_t
  */
UTIL_TIMER_Status_t UTIL_TIMER_Init(void);

/**
  * @brief Un-Initialize the timer server
  *
  * @retval Status based on @ref UTIL_TIMER_Status_t
  */
UTIL_TIMER_Status_t UTIL_TIMER_DeInit(void);

/**
  * @brief Create the timer object
  *
  * @remark TimerSetValue function must be called before starting the timer.
  *         this function initializes timestamp and reload value at 0.
  *
  * @param TimerObject Structure containing the timer object parameters
  * @param PeriodValue Period value of the timer in ms
  * @param Mode @ref UTIL_TIMER_Mode_t
  * @param Callback Function callback called at the end of the timeout
  * @param Argument argument for the callback function
  * @retval Status based on @ref UTIL_TIMER_Status_t
  */
UTIL_TIMER_Status_t UTIL_TIMER_Create( UTIL_TIMER_Object_t *TimerObject, uint32_t PeriodValue, UTIL_TIMER_Mode_t Mode, void ( *Callback )( void *) , void *Argument);

/**
  * @brief Start and adds the timer object to the list of timer events
  *
  * @param TimerObject Structure containing the timer object parameters
  * @retval Status based on @ref UTIL_TIMER_Status_t
  */
UTIL_TIMER_Status_t UTIL_TIMER_Start( UTIL_TIMER_Object_t *TimerObject );

/**
  * @brief Start and adds the timer object to the list of timer events
  *
  * @param TimerObject Structure containing the timer object parameters
  * @param PeriodValue period value of the timer
  * @retval Status based on @ref UTIL_TIMER_Status_t
  */
UTIL_TIMER_Status_t UTIL_TIMER_StartWithPeriod( UTIL_TIMER_Object_t *TimerObject, uint32_t PeriodValue);

/**
  * @brief Stop and removes the timer object from the list of timer events
  *
  * @param TimerObject Structure containing the timer object parameters
  * @retval Status based on @ref UTIL_TIMER_Status_t
  */
UTIL_TIMER_Status_t UTIL_TIMER_Stop( UTIL_TIMER_Object_t *TimerObject );


/**
  * @brief update the period and start the timer
  *
  * @param TimerObject Structure containing the timer object parameters
  * @param NewPeriodValue new period value of the timer
  * @retval Status based on @ref UTIL_TIMER_Status_t
  */
UTIL_TIMER_Status_t UTIL_TIMER_SetPeriod(UTIL_TIMER_Object_t *TimerObject, uint32_t NewPeriodValue);

/**
 * @brief update the period and start the timer
 *
 * @param TimerObject Structure containing the timer object parameters
 * @param ReloadMode new reload mode @ref UTIL_TIMER_Mode_t
 * @retval Status based on @ref UTIL_TIMER_Status_t
 */
UTIL_TIMER_Status_t UTIL_TIMER_SetReloadMode(UTIL_TIMER_Object_t *TimerObject, UTIL_TIMER_Mode_t ReloadMode);

/**
 * @brief get the remaining time before timer expiration
 *  *
 * @param TimerObject Structure containing the timer object parameters
 * @param Time time before expiration in ms
 * @retval Status based on @ref UTIL_TIMER_Status_t
 */
UTIL_TIMER_Status_t UTIL_TIMER_GetRemainingTime(UTIL_TIMER_Object_t *TimerObject, uint32_t *Time);

/**
 * @brief return timer state
 *
 * @param TimerObject Structure containing the timer object parameters
 * @retval boolean value is returned 0 = false and 1 = true
 */
uint32_t UTIL_TIMER_IsRunning( UTIL_TIMER_Object_t *TimerObject );


/**
  * @brief return the remaining time of the first timer in the chain list
  *
  * @retval return the time in ms, the value 0xFFFFFFFF means no timer running
  */
uint32_t UTIL_TIMER_GetFirstRemainingTime(void);

/**
  * @brief return the current time
  *
  * @retval time value
  */
UTIL_TIMER_Time_t UTIL_TIMER_GetCurrentTime(void);


/**
  * @brief return the elapsed time
  *
  * @param past a value returned by the function UTIL_TIMER_GetCurrentTime
  * @retval elasped time value
  */
UTIL_TIMER_Time_t UTIL_TIMER_GetElapsedTime(UTIL_TIMER_Time_t past );

/**
 * @brief Timer IRQ event handler
 *
 * @note Head Timer Object is automatically removed from the List
 *
 * @note e.g. it is not needed to stop it
 */
void UTIL_TIMER_IRQ_Handler( void );

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* UTIL_TIME_SERVER_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
