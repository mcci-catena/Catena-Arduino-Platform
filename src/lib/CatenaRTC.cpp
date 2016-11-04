/* catenartc.cpp	Tue Nov  1 2016 23:41:10 tmm */

/*

Module:  catenartc.cpp

Function:
	CatenaRTC class

Version:
	V0.3.0	Tue Nov  1 2016 23:41:10 tmm	Edit level 1

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
   0.3.0  Tue Nov  1 2016 23:41:10  tmm
	Module created.

*/

#include <CatenaRTC.H>

bool CatenaRTC::begin(bool fResetTime)
	{
	RTCZero::begin(fResetTime);
        return true;
	}

const uint16_t CatenaRTC::md[13] =
        {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
        };


CatenaRTC::CalendarTime 
CatenaRTC::GetTime(void)
        {
        CalendarTime result;
        uint8_t s_again;

        do  {
            result.Second = getSeconds();
            result.Minute = getMinutes();
            result.Hour = getHours();
            result.Day = getDay();
            result.Month = getMonth();
            result.Year = getYear();
            s_again = getSeconds();
            } while (s_again != result.Second);
        return result;
        }

void CatenaRTC::SetAlarm(uint32_t delta)
        {
        CalendarTime now = GetTime();

        now.Advance(delta);
        SetAlarm(&now);
        }

void CatenaRTC::SetAlarm(const CalendarTime *pNow)
        {
        setAlarmSeconds(pNow->Second);
        setAlarmMinutes(pNow->Minute);
        setAlarmHours(pNow->Hour);
        setAlarmDay(pNow->Day);
        setAlarmMonth(pNow->Month);
        setAlarmYear(pNow->Year);
        }

static volatile uint32_t *s_pAlarm;

static void AlarmCb(void)
        {
        if (s_pAlarm)
                *s_pAlarm = 1;
        }

void CatenaRTC::SleepForAlarm(
        CatenaRTC::Alarm_Match how,
        CatenaRTC::SleepMode howSleep
        )
        {
        this->disableAlarm();
        this->m_Alarm = false;
        s_pAlarm = &this->m_Alarm;
        this->attachInterrupt(AlarmCb);
        this->enableAlarm(how);

        /* we may want to try deep sleep, maybe not */
        switch (howSleep)
                {
                default:
                case SleepMode::IdleCpu:
                        while (! m_Alarm)
                                {
                                PM->SLEEP.bit.IDLE = PM_SLEEP_IDLE_CPU_Val;
                                SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
                                __WFI();
                                }
                        break;

                case SleepMode::IdleCpuAhb:
                        while (! m_Alarm)
                                {
                                PM->SLEEP.bit.IDLE = PM_SLEEP_IDLE_AHB_Val;
                                SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
                                __WFI();
                                }
                        break;

                case SleepMode::IdleCpuAhbApb:
                        while (! m_Alarm)
                                {
                                PM->SLEEP.bit.IDLE = PM_SLEEP_IDLE_APB_Val;
                                SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
                                __WFI();
                                }
                        break;

                case SleepMode::DeepSleep:
                        while (! m_Alarm)
                                this->standbyMode();
                        break;
                }
        this->disableAlarm();
        }

/* 
|| assumes that we're only advancing by a day or so -- anything more
|| than a day is treated as a day.
*/
bool
CatenaRTC::CalendarTime::Advance(
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
	uint8_t firstDay = CatenaRTC::md[Month - 1];
	uint8_t lastDay = CatenaRTC::md[Month];

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
