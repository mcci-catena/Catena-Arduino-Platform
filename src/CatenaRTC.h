/* CatenaRTC.h	Tue Nov  1 2016 22:56:04 tmm */

/*

Module:  CatenaRTC.h

Function:
	RTC wrapper for Catena

Version:
	V0.3.0	Tue Nov  1 2016 22:56:04 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

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

*/

#ifndef _CATENARTC_H_		/* prevent multiple includes */
#define _CATENARTC_H_

#include <CatenaSamd21.h>

#include <RTCZero.h>

/* the class for real-time calculations */

class CatenaRTC : public RTCZero
        {
public:
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

        void SetAlarm(uint32_t delta);

	void SetAlarm(const CalendarTime *pNow);

	void SleepForAlarm(Alarm_Match how = MATCH_HHMMSS, 
			   SleepMode howSleep = SleepMode::IdleCpu
			   );

private:
        volatile uint32_t m_Alarm;
	static const uint16_t md[13];
        };



/**** end of CatenaRTC.h ****/
#endif /* _CATENARTC_H_ */
