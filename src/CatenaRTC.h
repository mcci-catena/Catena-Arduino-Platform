/* CatenaRTC.h	Mon Nov 19 2018 12:08:48 chwon */

/*

Module:  CatenaRTC.h

Function:
	RTC wrapper for Catena

Version:
	V0.11.0	Mon Nov 19 2018 12:08:48 chwon	Edit level 2

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

*/

#ifndef _CATENARTC_H_		/* prevent multiple includes */
#define _CATENARTC_H_

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
