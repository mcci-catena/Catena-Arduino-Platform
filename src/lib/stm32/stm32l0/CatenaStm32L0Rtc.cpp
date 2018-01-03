/* CatenaStm32L0Rtc.cpp	Wed Jan 03 2018 11:04:33 chwon */

/*

Module:  CatenaStm32L0Rtc.cpp

Function:
	Class CatenaStm32L0Rtc

Version:
	V0.7.0	Wed Jan 03 2018 11:04:33 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	ChaeHee Won, MCCI Corporation	October 2017

Revision history:
   0.6.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

   0.6.0  Fri Dec 01 2017 13:55:23  chwon
	Remove debug code.

   0.6.0  Wed Dec 06 2017 15:32:22  chwon
	Change clock division setting.

   0.7.0  Wed Jan 03 2018 11:04:33  chwon
	Need to call HAL_RTCEx_EnableBypassShadow() to support USB.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0Rtc.h"
using namespace McciCatena;

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/


/****************************************************************************\
|
|	Read-only data.
|
|	If program is to be ROM-able, these must all be tagged read-only
|	using the ROM storage class; they may be global.
|
\****************************************************************************/


/****************************************************************************\
|
|	VARIABLES:
|
|	If program is to be ROM-able, these must be initialized
|	using the BSS keyword.  (This allows for compilers that require
|	every variable to have an initializer.)  Note that only those
|	variables owned by this module should be declared here, using the BSS
|	keyword; this allows for linkers that dislike multiple declarations
|	of objects.
|
\****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

static volatile uint32_t *gs_pAlarm;
static RTC_HandleTypeDef *gs_phRtc;

void RTC_IRQHandler(void)
	{
	HAL_RTC_AlarmIRQHandler(gs_phRtc);
	}

void HAL_RTC_AlarmAEventCallback(
	RTC_HandleTypeDef *	hRtc
	)
	{
	if (gs_pAlarm)
		*gs_pAlarm = 1;
	}

void HAL_RTC_MspInit(
	RTC_HandleTypeDef *	hRtc
	)
	{
	if (hRtc->Instance == RTC)
		{
		/* USER CODE BEGIN RTC_MspInit 0 */

		/* USER CODE END RTC_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_RTC_ENABLE();
		/* USER CODE BEGIN RTC_MspInit 1 */
		HAL_NVIC_SetPriority(RTC_IRQn, TICK_INT_PRIORITY, 0U);
		HAL_NVIC_EnableIRQ(RTC_IRQn); 
		/* USER CODE END RTC_MspInit 1 */
		}
	}

void HAL_RTC_MspDeInit(
	RTC_HandleTypeDef *	hRtc
	)
	{
	if (hRtc->Instance == RTC)
		{
		/* USER CODE BEGIN RTC_MspDeInit 0 */
		HAL_NVIC_DisableIRQ(RTC_IRQn); 
		/* USER CODE END RTC_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_RTC_DISABLE();
		/* USER CODE BEGIN RTC_MspDeInit 1 */

		/* USER CODE END RTC_MspDeInit 1 */
		}
	}

uint32_t HAL_AddTick(
	uint32_t delta
	)
	{
	extern __IO uint32_t uwTick;
	// copy old interrupt-enable state to flags.
	uint32_t const flags = __get_PRIMASK();

	// disable interrupts
	__set_PRIMASK(1);

	// observe uwTick, and advance it.
	uint32_t const tickCount = uwTick + delta;

	// save uwTick
	uwTick = tickCount;

	// restore interrupts (does nothing if ints were disabled on entry)
	__set_PRIMASK(flags);

	// return the new value of uwTick.
	return tickCount;
	}


#ifdef __cplusplus
}
#endif

bool CatenaStm32L0Rtc::begin(bool fResetTime)
	{
	RTC_TimeTypeDef	Time;
	RTC_DateTypeDef	Date;

//	memset(&this->m_hRtc, 0, sizeof(this->m_hRtc));

	this->m_hRtc.Instance = RTC;
	this->m_hRtc.Init.HourFormat = RTC_HOURFORMAT_24;
//	this->m_hRtc.Init.AsynchPrediv = 37 - 1; /* 37kHz / 37 = 1000Hz */
//	this->m_hRtc.Init.SynchPrediv = 1000 - 1; /* 1000Hz / 1000 = 1Hz */

//	this->m_hRtc.Init.AsynchPrediv = 64 - 1; /* 40kHz / 64 = 625Hz */
//	this->m_hRtc.Init.SynchPrediv = 625 - 1; /* 625Hz / 625 = 1Hz */

	this->m_hRtc.Init.AsynchPrediv = 128 - 1; /* 32768Hz / 128 = 256Hz */
//	this->m_hRtc.Init.SynchPrediv = 256 - 1; /* 256Hz / 256 = 1Hz */
	this->m_hRtc.Init.SynchPrediv = 256; /* 256Hz / 256 = 1Hz */

	this->m_hRtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	this->m_hRtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	this->m_hRtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	this->m_hRtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	gs_phRtc = &this->m_hRtc;

	if (HAL_RTC_Init(&this->m_hRtc) != HAL_OK)
		{
		Serial.println("HAL_RTC_Init() failed");
		return false;
		}

	/* Initialize RTC and set the Time and Date */
	if (fResetTime ||
	    HAL_RTCEx_BKUPRead(&this->m_hRtc, RTC_BKP_DR0) != 0x32F2)
		{
		Time.Hours = 0x0;
		Time.Minutes = 0x0;
		Time.Seconds = 0x0;
		Time.SubSeconds = 0x0;
		Time.TimeFormat = RTC_HOURFORMAT12_AM;
		Time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		Time.StoreOperation = RTC_STOREOPERATION_RESET;

		if (HAL_RTC_SetTime(
			&this->m_hRtc,
			&Time,
			RTC_FORMAT_BIN
			) != HAL_OK)
			{
			Serial.println("HAL_RTC_SetTime() failed");
			return false;
			}
	
		/* Sunday 1st January 2017 */
		Date.WeekDay = RTC_WEEKDAY_SUNDAY;
		Date.Month = RTC_MONTH_JANUARY;
		Date.Date = 0x1;
		Date.Year = 0x0;
	
		if (HAL_RTC_SetDate(
			&this->m_hRtc,
			&Date,
			RTC_FORMAT_BIN
			) != HAL_OK)
			{
			Serial.println("HAL_RTC_SetDate() failed");
			return false;
			}

		HAL_RTCEx_BKUPWrite(&this->m_hRtc, RTC_BKP_DR0, 0x32F2);
		}

	/* Enable Direct Read of the calendar registers (not through Shadow) */
	HAL_RTCEx_EnableBypassShadow(&this->m_hRtc);

	HAL_RTC_DeactivateAlarm(&this->m_hRtc, RTC_ALARM_A);
	return true;
	}

const uint16_t CatenaStm32L0Rtc::md[13] =
	{
	0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
	};


CatenaStm32L0Rtc::CalendarTime
CatenaStm32L0Rtc::GetTime(void)
	{
	CalendarTime	Result;
	RTC_TimeTypeDef	Time;
	RTC_DateTypeDef	Date;
	uint32_t	SubSeconds;

	do	{
		HAL_RTC_GetTime(&this->m_hRtc, &Time, RTC_FORMAT_BIN);
		SubSeconds = Time.SubSeconds;
		HAL_RTC_GetDate(&this->m_hRtc, &Date, RTC_FORMAT_BIN);
		HAL_RTC_GetTime(&this->m_hRtc, &Time, RTC_FORMAT_BIN);
		} while (SubSeconds != Time.SubSeconds);

	Result.Second = Time.Seconds;
	Result.Minute = Time.Minutes;
	Result.Hour = Time.Hours;
	Result.Day = Date.Date;
	Result.Month = Date.Month;
	Result.Year = Date.Year;
	return Result;
	}

bool CatenaStm32L0Rtc::SetTime(const CalendarTime *pNow)
	{
	RTC_TimeTypeDef	Time;
	RTC_DateTypeDef	Date;

	Time.Hours = pNow->Hour;
	Time.Minutes = pNow->Minute;
	Time.Seconds = pNow->Second;
	Time.SubSeconds = 0x0;
	Time.TimeFormat = RTC_HOURFORMAT12_AM;
	Time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	Time.StoreOperation = RTC_STOREOPERATION_RESET;

	if (HAL_RTC_SetTime(&this->m_hRtc, &Time, RTC_FORMAT_BIN) != HAL_OK)
		{
		Serial.println("HAL_RTC_SetTime() failed");
		return false;
		}

	Date.WeekDay = RTC_WEEKDAY_MONDAY;	/* don't care */
	Date.Month = pNow->Month;
	Date.Date = pNow->Day;
	Date.Year = pNow->Year;

	if (HAL_RTC_SetDate(&this->m_hRtc, &Date, RTC_FORMAT_BIN) != HAL_OK)
		{
		Serial.println("HAL_RTC_SetDate() failed");
		return false;
		}

	return true;
	}

void CatenaStm32L0Rtc::SetAlarm(uint32_t delta)
	{
	CalendarTime now = GetTime();

	now.Advance(delta);
	SetAlarm(&now);
	}

void CatenaStm32L0Rtc::SetAlarm(const CalendarTime *pNow)
	{
	RTC_AlarmTypeDef Alarm;

	this->m_fAlarmEnabled = true;
	this->m_Alarm = 0;
	gs_pAlarm = &this->m_Alarm;

//	Serial.print("SetAlarm: ");
//	Serial.print(pNow->Year + CATENA_STM32L0_RTC_BASE_YEAR);
//	Serial.print("-");
//	Serial.print(pNow->Month);
//	Serial.print("-");
//	Serial.print(pNow->Day);
//	Serial.print(" ");
//	Serial.print(pNow->Hour);
//	Serial.print(":");
//	Serial.print(pNow->Minute);
//	Serial.print(":");
//	Serial.println(pNow->Second);

	/* Set Alarm with calculated values*/
	Alarm.AlarmTime.Hours = pNow->Hour;
	Alarm.AlarmTime.Minutes = pNow->Minute;
	Alarm.AlarmTime.Seconds = pNow->Second;
	Alarm.AlarmTime.SubSeconds = 0;
	Alarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	Alarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	Alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
	Alarm.AlarmDateWeekDay = pNow->Day;
	Alarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	Alarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	Alarm.AlarmMask = RTC_ALARMMASK_NONE;
	Alarm.Alarm = RTC_ALARM_A;

	/* Set RTC_Alarm */
	HAL_RTC_SetAlarm_IT(&this->m_hRtc, &Alarm, RTC_FORMAT_BIN);
	}

bool CatenaStm32L0Rtc::PollAlarmState(void)
	{
	if (this->m_Alarm)
		{
		this->m_fAlarmEnabled = false;
		return true;
		}
	return false;
	}

void CatenaStm32L0Rtc::SleepForAlarm(
	CatenaStm32L0Rtc::SleepMode howSleep
	)
	{
	uint32_t nWakes;

	if (! this->m_fAlarmEnabled || this->m_Alarm)
		return;

	HAL_SuspendTick();

	/* we may want to try deep sleep, maybe not */
	nWakes = 0;
	switch (howSleep)
		{
	default:
	case SleepMode::SleepWithRegulator:
		while (! m_Alarm)
			{
			++nWakes;
			HAL_PWR_EnterSLEEPMode(
				PWR_MAINREGULATOR_ON,
				PWR_SLEEPENTRY_WFI
				);
			}
		break;

	case SleepMode::SleepWithLowPowerRegulator:
		while (! m_Alarm)
			{
			++nWakes;
			HAL_PWR_EnterSLEEPMode(
				PWR_LOWPOWERREGULATOR_ON,
				PWR_SLEEPENTRY_WFI
				);
			}
		break;

	case SleepMode::StopWithRegulator:
		while (! m_Alarm)
			{
			++nWakes;
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
			HAL_PWR_EnterSTOPMode(
				PWR_MAINREGULATOR_ON,
				PWR_SLEEPENTRY_WFI
				);
			}
		break;

	case SleepMode::StopWithLowPowerRegulator:
		while (! m_Alarm)
			{
			++nWakes;
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
			HAL_PWR_EnterSTOPMode(
				PWR_LOWPOWERREGULATOR_ON,
				PWR_SLEEPENTRY_WFI
				);
			}
		break;

	case SleepMode::Standby:
		while (! m_Alarm)
			{
			++nWakes;
			HAL_PWR_EnterSTANDBYMode();
			}
		break;
		}

	HAL_IncTick();
	HAL_ResumeTick();

//	Serial.print("SleepForAlarm: ");
//	Serial.println(nWakes);
	}

uint32_t CatenaStm32L0Rtc::AdjustMillisForward(uint32_t delta)
	{
	return HAL_AddTick(delta);
	}

/*
|| assumes that we're only advancing by a day or so -- anything more
|| than a day is treated as a day.
*/
bool
CatenaStm32L0Rtc::CalendarTime::Advance(
	uint32_t delta
	)
	{
	uint32_t d;
	bool result;

	result = true;

	if (delta > 86400)	// everyone knows that one day is 86400 secs,
		{		// right?
		delta = 86400;
		result = false;
		}


	d = delta % 60;
	delta /= 60;
	Second += d;
	if (Second > 59)
		{
		delta += Second / 60;
		Second %= 60;
		}
	if (delta == 0)
		return result;

	d = delta % 60;
	delta /= 60;
	Minute += d;
	if (Minute > 59)
		{
		delta += Minute / 60;
		Minute %= 60;
		}
	if (delta == 0)
		return result;

	d = delta % 24;
	delta /= 24;
	Hour += d;
	if (Hour > 23)
		{
		delta += Hour / 24;
		Hour %= 24;
		}
	if (delta == 0)
		return result;

	uint8_t leapYear = (Year & 0x3) ? 0 : 1;
	uint8_t firstDay = CatenaStm32L0Rtc::md[Month - 1];
	uint8_t lastDay = CatenaStm32L0Rtc::md[Month];

	if (leapYear)
		{
		if (Month >= 2)
			{
			++lastDay;
			if (Month >= 3)
				++firstDay;
			}
		}

	// now we just propagate carry
	++Day;
	if (Day <= lastDay)
		return result;

	Day = 1;
	++Month;
	if (Month <= 12)
		return result;
	Month = 1;
	++Year;
	return result;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0Rtc.cpp ****/
