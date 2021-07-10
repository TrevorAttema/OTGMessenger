/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc_if.c
  * @author  MCD Application Team
  * @brief   Configure RTC Alarm, Tick and Calendar manager
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
/* USER CODE END Header */

#define USE_FULL_LL_DRIVER

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "main.h" /*for Mx generated RTC_N_PREDIV_S and RTC_N_PREDIV_A*/
#include "rtc.h"
#include "rtc_if.h"
#include "stm32_lpm.h"
//#include "utilities_def.h"
#include "stm32h7xx_ll_rtc.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint32_t Rtc_Time; /* Reference time */

  RTC_TimeTypeDef RTC_Calndr_Time; /* Reference time in calendar format */

  RTC_DateTypeDef RTC_Calndr_Date; /* Reference date in calendar format */

} RtcTimerContext_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
#define MIN_ALARM_DELAY               3 /* in ticks */
/* RTC Ticks/ms conversion */

#define RTC_BKP_SECONDS    RTC_BKP_DR0
#define RTC_BKP_SUBSECONDS RTC_BKP_DR1
#define RTC_BKP_MSBTICKS   RTC_BKP_DR2

/* Sub-second mask definition  */
#define RTC_ALARMSUBSECONDMASK    RTC_N_PREDIV_S<<RTC_ALRMASSR_MASKSS_Pos


/* RTC Time base in us */
#define USEC_NUMBER               1000000
#define MSEC_NUMBER               (USEC_NUMBER/1000)

#define COMMON_FACTOR        3
#define CONV_NUMER                (MSEC_NUMBER>>COMMON_FACTOR)
#define CONV_DENOM                (1<<(RTC_N_PREDIV_S-COMMON_FACTOR))

#define  DAYS_IN_LEAP_YEAR (uint32_t) 366

#define  DAYS_IN_YEAR      (uint32_t) 365

#define  SECONDS_IN_1DAY   (uint32_t) 86400

#define  SECONDS_IN_1HOUR   (uint32_t) 3600

#define  SECONDS_IN_1MINUTE   (uint32_t) 60

#define  MINUTES_IN_1HOUR    (uint32_t) 60

#define  HOURS_IN_1DAY      (uint32_t) 24

#define  DAYS_IN_MONTH_CORRECTION_NORM     ((uint32_t) 0x99AAA0 )
#define  DAYS_IN_MONTH_CORRECTION_LEAP     ((uint32_t) 0x445550 )


/* Calculates ceiling(X/N) */
#define DIVC(X,N)   ( ( (X) + (N) -1 ) / (N) )

/* #define RTIF_DEBUG */

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/

#ifdef RTIF_DEBUG
#include "sys_app.h" /*for app_log*/
#define RTC_IF_DBG_PRINTF(...) do{ {UTIL_ADV_TRACE_COND_FSend(VLEVEL_ALWAYS, T_REG_OFF, TS_OFF, __VA_ARGS__);} }while(0);
#else
#define RTC_IF_DBG_PRINTF(...)
#endif /* RTIF_DEBUG */

/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/*!
 * \brief Indicates if the RTC is already Initialized or not
 */
static bool RTC_Initalized = false;

/*!
 * \brief compensates MCU wakeup time
 */

static int16_t McuWakeUpTimeCal = 0;

/*!
 * Number of days in each month on a normal year
 */
static const uint8_t DaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*!
 * Number of days in each month on a leap year
 */
static const uint8_t DaysInMonthLeapYear[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static RTC_AlarmTypeDef RTC_AlarmStructure;

/*!
 * Keep the value of the RTC timer when the RTC alarm is set
 * Set with the RTC_SetTimerContext function
 * Value is kept as a Reference to calculate alarm
 */
static RtcTimerContext_t RtcTimerContext;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/*!
 * @brief start wake up alarm
 * @note  alarm in RtcTimerContext.Rtc_Time + timeoutValue
 * @param timeoutValue in ticks
 * @retval none
 */
static void RTC_StartWakeUpAlarm(uint32_t timeoutValue);

/*!
 * @brief get current time from calendar in ticks
 * @param pointer to RTC_DateStruct
 * @param pointer to RTC_TimeStruct
 * @retval time in ticks
 */
static uint32_t RTC_GetCalendarValue(RTC_DateTypeDef *RTC_DateStruct, RTC_TimeTypeDef *RTC_TimeStruct);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported variables ---------------------------------------------------------*/

/*Timer driver*/
const UTIL_TIMER_Driver_s UTIL_TimerDriver =
{
  RTC_IF_Init,
  NULL,

  RTC_IF_StartTimer,
  RTC_IF_StopTimer,

  RTC_IF_SetTimerContext,
  RTC_IF_GetTimerContext,

  RTC_IF_GetTimerElapsedTime,
  RTC_IF_GetTimerValue,
  RTC_IF_GetMinimumTimeout,

  RTC_IF_Convert_ms2Tick,
  RTC_IF_Convert_Tick2ms,
};

/*System Time driver*/
const UTIL_SYSTIM_Driver_s UTIL_SYSTIMDriver =
{
  RTC_IF_BkUp_Write_Seconds,
  RTC_IF_BkUp_Read_Seconds,
  RTC_IF_BkUp_Write_SubSeconds,
  RTC_IF_BkUp_Read_SubSeconds,
  RTC_IF_GetTime,
};

/* USER CODE BEGIN Exported_Variables */

/* USER CODE END Exported_Variables */

/* Exported functions ---------------------------------------------------------*/
/* USER CODE BEGIN Exported_Functions */

/* USER CODE END Exported_Functions */

UTIL_TIMER_Status_t RTC_IF_Init(void)
{
  //RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  if (RTC_Initalized == false)
  {
//    RTC_IF_DBG_PRINTF("RTC_init\n\r");

    /* ##-1- Configure the RTC clock source ###################################### */

    /* Select LSE as RTC clock source */
//    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
//    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
//    {
//      /* Initialization Error */
//      while (1);
//    }

    MX_RTC_Init();

    /** Configure the Alarm A */
    HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
    /*Enable Direct Read of the calendar registers (not through Shadow) */
    HAL_RTCEx_EnableBypassShadow(&hrtc);

    RTC_IF_SetTimerContext();

    RTC_Initalized = true;
  }
  return UTIL_TIMER_OK;
}

UTIL_TIMER_Status_t RTC_IF_StartTimer(uint32_t timeout)
{
  /* we don't go in Low Power mode for timeout below MIN_ALARM_DELAY */
//  if ((MIN_ALARM_DELAY + McuWakeUpTimeCal) < ((timeout - RTC_IF_GetTimerElapsedTime())))
//  {
//    //UTIL_LPM_SetStopMode((1 << CFG_LPM_RTC_Id), UTIL_LPM_ENABLE);
//  }
//  else
//  {
//    //UTIL_LPM_SetStopMode((1 << CFG_LPM_RTC_Id), UTIL_LPM_DISABLE);
//  }

  /*In case stop mode is required */
  if (UTIL_LPM_GetMode() == UTIL_LPM_STOPMODE)
  {
    timeout = timeout -  McuWakeUpTimeCal;
  }

  RTC_StartWakeUpAlarm(timeout);
  return UTIL_TIMER_OK;
}

UTIL_TIMER_Status_t RTC_IF_StopTimer(void)
{
  /* Clear RTC Alarm Flag */
  __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
  /* Disable the Alarm A interrupt */
  HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
  return UTIL_TIMER_OK;
}

uint32_t RTC_IF_SetTimerContext(void)
{
  /*store time context*/
  RtcTimerContext.Rtc_Time = RTC_GetCalendarValue(&RtcTimerContext.RTC_Calndr_Date, &RtcTimerContext.RTC_Calndr_Time);
  RTC_IF_DBG_PRINTF("RTC_IF_SetTimerContext=%d\n\r", RtcTimerContext.Rtc_Time);
  /*return time context*/
  return (uint32_t) RtcTimerContext.Rtc_Time;
}

uint32_t RTC_IF_GetTimerContext(void)
{
  RTC_IF_DBG_PRINTF("RTC_IF_GetTimerContext=%d\n\r", RtcTimerContext.Rtc_Time);
  /*return time context*/
  return (uint32_t) RtcTimerContext.Rtc_Time;
}

uint32_t RTC_IF_GetTimerElapsedTime(void)
{
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;

  uint32_t CalendarValue = RTC_GetCalendarValue(&RTC_DateStruct, &RTC_TimeStruct);

  return ((uint32_t)(CalendarValue - RtcTimerContext.Rtc_Time));
}

uint32_t RTC_IF_GetTimerValue(void)
{
  uint32_t CalendarValue = 0;
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;

  if (RTC_Initalized == true)
  {
    CalendarValue = (uint32_t) RTC_GetCalendarValue(&RTC_DateStruct, &RTC_TimeStruct);
  }
  return CalendarValue;
}

uint32_t RTC_IF_GetMinimumTimeout(void)
{
  return (MIN_ALARM_DELAY);
}

uint32_t RTC_IF_Convert_ms2Tick(uint32_t timeMilliSec)
{
  return (uint32_t)((((uint64_t)timeMilliSec) * CONV_DENOM) / CONV_NUMER);
}

uint32_t RTC_IF_Convert_Tick2ms(uint32_t tick)
{
  return (((uint64_t)(tick) * CONV_NUMER) / CONV_DENOM);
}

void RTC_IF_DelayMs(uint32_t delay)
{
  uint32_t delayValue = 0;
  uint32_t timeout = 0;

  delayValue = RTC_IF_Convert_ms2Tick(delay);

  /* Wait delay ms */
  timeout = RTC_IF_GetTimerValue();
  while (((RTC_IF_GetTimerValue() - timeout)) < delayValue)
  {
    __NOP();
  }
}

/**
  * @brief  Alarm A callback.
  * @param  RtcHandle: RTC handle
  * @retval None
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *RtcHandle)
{
  UTIL_TIMER_IRQ_Handler();
}

uint32_t RTC_IF_GetTime(uint16_t *mSeconds)
{
  RTC_TimeTypeDef RTC_TimeStruct ;
  RTC_DateTypeDef RTC_DateStruct;
  uint32_t ticks;

  uint64_t calendarValue = RTC_GetCalendarValue(&RTC_DateStruct, &RTC_TimeStruct);

  uint32_t seconds = (uint32_t)(calendarValue >> RTC_N_PREDIV_S);

  ticks = (uint32_t) calendarValue & RTC_PREDIV_S;

  *mSeconds = RTC_IF_Convert_Tick2ms(ticks);

  return seconds;
}

void RTC_IF_BkUp_Write_Seconds(uint32_t Seconds)
{
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_SECONDS, Seconds);
}

void RTC_IF_BkUp_Write_SubSeconds(uint32_t SubSeconds)
{
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_SUBSECONDS, SubSeconds);
}

uint32_t RTC_IF_BkUp_Read_Seconds(void)
{
  return HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_SECONDS);
}

uint32_t RTC_IF_BkUp_Read_SubSeconds(void)
{
  return HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_SUBSECONDS);
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private functions ---------------------------------------------------------*/

/*!
 * @brief start wake up alarm
 * @note  alarm in RtcTimerContext.Rtc_Time + timeoutValue
 * @param timeoutValue in ticks
 * @retval none
 */
static void RTC_StartWakeUpAlarm(uint32_t timeoutValue)
{
  uint16_t rtcAlarmSubSeconds = 0;
  uint16_t rtcAlarmSeconds = 0;
  uint16_t rtcAlarmMinutes = 0;
  uint16_t rtcAlarmHours = 0;
  uint16_t rtcAlarmDays = 0;
  RTC_TimeTypeDef RTC_TimeStruct = RtcTimerContext.RTC_Calndr_Time;
  RTC_DateTypeDef RTC_DateStruct = RtcTimerContext.RTC_Calndr_Date;

  RTC_IF_StopTimer();


  /*reverse counter */
  rtcAlarmSubSeconds =  RTC_PREDIV_S - RTC_TimeStruct.SubSeconds;
  rtcAlarmSubSeconds += (timeoutValue & RTC_PREDIV_S);
  /* convert timeout  to seconds */
  timeoutValue >>= RTC_N_PREDIV_S;  /* convert timeout  in seconds */

  /*convert microsecs to RTC format and add to 'Now' */
  rtcAlarmDays =  RTC_DateStruct.Date;
  while (timeoutValue >= SECONDS_IN_1DAY)
  {
    timeoutValue -= SECONDS_IN_1DAY;
    rtcAlarmDays++;
  }

  /* calc hours */
  rtcAlarmHours = RTC_TimeStruct.Hours;
  while (timeoutValue >= SECONDS_IN_1HOUR)
  {
    timeoutValue -= SECONDS_IN_1HOUR;
    rtcAlarmHours++;
  }

  /* calc minutes */
  rtcAlarmMinutes = RTC_TimeStruct.Minutes;
  while (timeoutValue >= SECONDS_IN_1MINUTE)
  {
    timeoutValue -= SECONDS_IN_1MINUTE;
    rtcAlarmMinutes++;
  }

  /* calc seconds */
  rtcAlarmSeconds =  RTC_TimeStruct.Seconds + timeoutValue;

  /***** correct for modulo********/
  while (rtcAlarmSubSeconds >= (RTC_PREDIV_S + 1))
  {
    rtcAlarmSubSeconds -= (RTC_PREDIV_S + 1);
    rtcAlarmSeconds++;
  }

  while (rtcAlarmSeconds >= SECONDS_IN_1MINUTE)
  {
    rtcAlarmSeconds -= SECONDS_IN_1MINUTE;
    rtcAlarmMinutes++;
  }

  while (rtcAlarmMinutes >= MINUTES_IN_1HOUR)
  {
    rtcAlarmMinutes -= MINUTES_IN_1HOUR;
    rtcAlarmHours++;
  }

  while (rtcAlarmHours >= HOURS_IN_1DAY)
  {
    rtcAlarmHours -= HOURS_IN_1DAY;
    rtcAlarmDays++;
  }

  if (RTC_DateStruct.Year % 4 == 0)
  {
    if (rtcAlarmDays > DaysInMonthLeapYear[ RTC_DateStruct.Month - 1 ])
    {
      rtcAlarmDays = rtcAlarmDays % DaysInMonthLeapYear[ RTC_DateStruct.Month - 1 ];
    }
  }
  else
  {
    if (rtcAlarmDays > DaysInMonth[ RTC_DateStruct.Month - 1 ])
    {
      rtcAlarmDays = rtcAlarmDays % DaysInMonth[ RTC_DateStruct.Month - 1 ];
    }
  }

  /* Set RTC_AlarmStructure with calculated values*/
  RTC_AlarmStructure.AlarmTime.SubSeconds = RTC_PREDIV_S - rtcAlarmSubSeconds;
  RTC_AlarmStructure.AlarmSubSecondMask  = RTC_ALARMSUBSECONDMASK;
  RTC_AlarmStructure.AlarmTime.Seconds = rtcAlarmSeconds;
  RTC_AlarmStructure.AlarmTime.Minutes = rtcAlarmMinutes;
  RTC_AlarmStructure.AlarmTime.Hours   = rtcAlarmHours;
  RTC_AlarmStructure.AlarmDateWeekDay    = (uint8_t)rtcAlarmDays;
  RTC_AlarmStructure.AlarmTime.TimeFormat   = RTC_TimeStruct.TimeFormat;
  RTC_AlarmStructure.AlarmDateWeekDaySel   = RTC_ALARMDATEWEEKDAYSEL_DATE;
  RTC_AlarmStructure.AlarmMask       = RTC_ALARMMASK_NONE;
  RTC_AlarmStructure.Alarm = RTC_ALARM_A;
  RTC_AlarmStructure.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  RTC_AlarmStructure.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;

  /* Set RTC_Alarm */
  HAL_RTC_SetAlarm_IT(&hrtc, &RTC_AlarmStructure, RTC_FORMAT_BIN);
}


/*!
  * @brief get current time from calendar in ticks
  * @param pointer to RTC_DateStruct
  * @param pointer to RTC_TimeStruct
  * @retval time in ticks
  */
static uint32_t RTC_GetCalendarValue(RTC_DateTypeDef *RTC_DateStruct, RTC_TimeTypeDef *RTC_TimeStruct)
{
  uint32_t calendarValue = 0;
  uint32_t first_read;
  uint32_t correction;

  /* Get Time and Date*/
  HAL_RTC_GetTime(&hrtc, RTC_TimeStruct, RTC_FORMAT_BIN);

  /* make sure it is correct due to asynchronus nature of RTC*/
  do
  {
    first_read = LL_RTC_TIME_GetSubSecond(RTC);
    HAL_RTC_GetDate(&hrtc, RTC_DateStruct, RTC_FORMAT_BIN);
    HAL_RTC_GetTime(&hrtc, RTC_TimeStruct, RTC_FORMAT_BIN);

  } while (first_read != LL_RTC_TIME_GetSubSecond(RTC));

  /* calculte amount of elapsed days since 01/01/2000 */
  calendarValue = DIVC((DAYS_IN_YEAR * 3 + DAYS_IN_LEAP_YEAR) * RTC_DateStruct->Year, 4);

  correction = ((RTC_DateStruct->Year % 4) == 0) ? DAYS_IN_MONTH_CORRECTION_LEAP : DAYS_IN_MONTH_CORRECTION_NORM ;

  calendarValue += (DIVC((RTC_DateStruct->Month - 1) * (30 + 31),
                         2) - (((correction >> ((RTC_DateStruct->Month - 1) * 2)) & 0x3)));

  calendarValue += (RTC_DateStruct->Date - 1);

  /* convert from days to seconds */
  calendarValue *= SECONDS_IN_1DAY;

  calendarValue += ((uint32_t)RTC_TimeStruct->Seconds +
                    ((uint32_t)RTC_TimeStruct->Minutes * SECONDS_IN_1MINUTE) +
                    ((uint32_t)RTC_TimeStruct->Hours * SECONDS_IN_1HOUR)) ;

  calendarValue = (calendarValue << RTC_N_PREDIV_S) + (RTC_PREDIV_S - RTC_TimeStruct->SubSeconds);

  return (calendarValue);
}

/* USER CODE BEGIN PrFD */

/* USER CODE END PrFD */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
