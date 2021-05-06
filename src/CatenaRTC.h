/* CatenaRTC.h	Mon Nov 26 2018 15:01:32 chwon */

/*

Module:  CatenaRTC.h

Function:
	RTC wrapper for Catena

Version:
	V0.12.0	Mon Nov 26 2018 15:01:32 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2016, 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	November 2016

Revision history:
   0.3.0  Tue Nov  1 2016 22:56:04  tmm
	Module created.

   0.11.0  Mon Nov 19 2018 12:08:48  chwon
	Remove include CatenaSamd21.h.

   0.12.0  Mon Nov 26 2018 15:01:32  chwon
	Remove RTCZero library.

*/

#ifndef _CATENARTC_H_		/* prevent multiple includes */
#define _CATENARTC_H_

#include <Arduino.h>

/* the class for real-time calculations */
namespace McciCatena {

class CatenaRTC
        {
public:
	enum Alarm_Match: uint8_t
		{
		MATCH_OFF          = RTC_MODE2_MASK_SEL_OFF_Val,          // Never
		MATCH_SS           = RTC_MODE2_MASK_SEL_SS_Val,           // Every Minute
		MATCH_MMSS         = RTC_MODE2_MASK_SEL_MMSS_Val,         // Every Hour
		MATCH_HHMMSS       = RTC_MODE2_MASK_SEL_HHMMSS_Val,       // Every Day
		MATCH_DHHMMSS      = RTC_MODE2_MASK_SEL_DDHHMMSS_Val,     // Every Month
		MATCH_MMDDHHMMSS   = RTC_MODE2_MASK_SEL_MMDDHHMMSS_Val,   // Every Year
		MATCH_YYMMDDHHMMSS = RTC_MODE2_MASK_SEL_YYMMDDHHMMSS_Val  // Once, on a specific date and a specific time
		};

        struct JulianTime;
        struct CalendarTime;
	enum class SleepMode
		{
		IdleCpu = 0,
		IdleCpuAhb,
		IdleCpuAhbApb,
		SleepWalk,
		DeepSleep,
		};

        struct CalendarTime
                {
                uint8_t Year, Month, Day;
                uint8_t Hour, Minute, Second;

		bool Advance(uint32_t delta);
                };

        CatenaRTC() {};
        bool begin(bool fResetTime = false);

        CalendarTime GetTime();
	bool SetTime(const CalendarTime *pTime);
        void SetAlarm(uint32_t delta);

	void SetAlarm(const CalendarTime *pNow);

	void SleepForAlarm(Alarm_Match how = MATCH_HHMMSS,
			   SleepMode howSleep = SleepMode::IdleCpu
			   );

private:
        volatile uint32_t m_Alarm;
	static const uint16_t md[13];
        };

} // namespace McciCatena


/**** end of CatenaRTC.h ****/
#endif /* _CATENARTC_H_ */
