/* CatenaStm32L0Rtc.h	Fri Dec 01 2017 13:59:17 chwon */

/*

Module:  CatenaStm32L0Rtc.h

Function:
	Class CatenaStm32L0Rtc

Version:
	V0.6.0	Fri Dec 01 2017 13:59:17 chwon	Edit level 2

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

   0.6.0  Fri Dec 01 2017 13:59:17  chwon
	Include Arduino.h instead of variant.h.

*/

#ifndef _CATENASTM32L0RTC_H_		/* prevent multiple includes */
#define _CATENASTM32L0RTC_H_

#ifdef STM32L0xx	/* This is valid only STM32L0xx CPU */

#pragma once

#include <Arduino.h>

/* TODO: make this an enum const in the McciCatena namespace */
#define	CATENA_STM32L0_RTC_BASE_YEAR	2017

namespace McciCatena {

class CatenaStm32L0Rtc
	{
public:
	struct CalendarTime;
	enum class SleepMode
		{
		SleepWithRegulator = 0,
		SleepWithLowPowerRegulator,
		StopWithRegulator,
		StopWithLowPowerRegulator,
		Standby
		};

	struct CalendarTime
		{
		uint8_t Year, Month, Day;
		uint8_t Hour, Minute, Second;

		bool Advance(uint32_t delta);
		};

	CatenaStm32L0Rtc() {};
	bool begin(bool fResetTime = false);

	CalendarTime GetTime();

	bool SetTime(const CalendarTime *pNow);

	void SetAlarm(uint32_t delta);

	void SetAlarm(const CalendarTime *pNow);

	bool PollAlarmState(void);

	void SleepForAlarm(SleepMode howSleep = SleepMode::SleepWithRegulator);

	uint32_t AdjustMillisForward(uint32_t delta);

private:
	RTC_HandleTypeDef	m_hRtc;
	volatile uint32_t	m_Alarm;
	bool			m_fAlarmEnabled;
	static const uint16_t	md[13];
	};

} // namespace McciCatena

#endif /* STM32L0xx */

/**** end of CatenaStm32L0Rtc.h ****/
#endif /* _CATENASTM32L0RTC_H_ */
