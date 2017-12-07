/* catenartc.cpp	Thu Nov 10 1 2017 15:32:10 tmm */

/*

Module:  catenartc.cpp

Function:
	CatenaRTC class

Version:
	V0.3.0	Thu Nov 10 2016 23:41:10 tmm	Edit level 2

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

   0.3.0  Thu Nov 10 2016 23:41:10  tmm
	Add debugging code. Disable all other interrupt sources during sleep
	other than RTC (don't let Systick wake us up).

*/

#ifdef ARDUINO_ARCH_SAMD

#include <CatenaRTC.h>
#include <Arduino.h>

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
        uint32_t nWakes;
        uint32_t nvic_iser_save;
        uint32_t systick_ctrl_save;

        // fetch the bits for other than the RC,
        // and then disable all other sources.
        nvic_iser_save = NVIC->ISER[0] & ~(1 << RTC_IRQn);
        NVIC->ICER[0] = nvic_iser_save;

        // fetch the current state of SysTick, then
        // disable interrupts.
        systick_ctrl_save = SysTick->CTRL;
        SysTick->CTRL = systick_ctrl_save & ~SysTick_CTRL_ENABLE_Msk;

        // turn off alarms, just in case, so we can safely init.
        this->disableAlarm();
        this->m_Alarm = false;
        s_pAlarm = &this->m_Alarm;
        this->attachInterrupt(AlarmCb);
        this->enableAlarm(how);

        /* we may want to try deep sleep, maybe not */
        nWakes = 0;
        switch (howSleep)
                {
                default:
                case SleepMode::IdleCpu:
                        while (! m_Alarm)
                                {
                                ++nWakes;
                                PM->SLEEP.bit.IDLE = PM_SLEEP_IDLE_CPU_Val;
                                SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
                                __WFI();
                                }
                        break;

                case SleepMode::IdleCpuAhb:
                        while (! m_Alarm)
                                {
                                ++nWakes;
                                PM->SLEEP.bit.IDLE = PM_SLEEP_IDLE_AHB_Val;
                                SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
                                __WFI();
                                }
                        break;

                case SleepMode::IdleCpuAhbApb:
                        while (! m_Alarm)
                                {
                                ++nWakes;
                                PM->SLEEP.bit.IDLE = PM_SLEEP_IDLE_APB_Val;
                                SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
                                __WFI();
                                }
                        break;

                case SleepMode::DeepSleep:
                        while (! m_Alarm)
                                {
                                ++nWakes;
                                this->standbyMode();
                                }
                        break;
                }
        this->disableAlarm();

        uint32_t systick_ctrl_post = SysTick->CTRL;
        uint32_t nvic_iser_post = NVIC->ISER[0];

        // restore the ISER bits.
        SysTick->CTRL = systick_ctrl_save;
        NVIC->ISER[0] = nvic_iser_save;

#if 0
        if (/* nWakes > 1 && */ Serial)
                {
                Serial.print("Alarm! "); Serial.print(nWakes-1);
                Serial.print(" extra wakeups. NVIC_ISER: saved="); Serial.print(nvic_iser_save, HEX);
                Serial.print(" current="); Serial.print(nvic_iser_post, HEX);
                Serial.print(" SysTick.CTRL: saved ="); Serial.print(systick_ctrl_save, HEX);
                Serial.print(" current="); Serial.println(systick_ctrl_post, HEX);

                Serial.print("PORT_DIR="); Serial.print(PORT->Group[0].DIR.reg, HEX);
                Serial.print(" PORT_IN="); Serial.print(PORT->Group[0].IN.reg, HEX);
                uint32_t uIoEnables, uIoPullupEn, uIoPMuxEn;
                uIoEnables = uIoPullupEn = uIoPMuxEn = 0;
                for (uint32_t i = 0; i < 32; ++i)
                        {
                        uIoEnables |= (PORT->Group[0].PINCFG[i].bit.INEN << i);
                        uIoPullupEn |= (PORT->Group[0].PINCFG[i].bit.PULLEN << i);
                        uIoPMuxEn |= (PORT->Group[0].PINCFG[i].bit.PMUXEN << i);
                        }
                Serial.print(" PORT_INEN="); Serial.print(uIoEnables, HEX);
                Serial.print(" PORT_PULLEN="); Serial.print(uIoPullupEn, HEX);
                Serial.print(" PORT_PMUXEN="); Serial.print(uIoPMuxEn, HEX);
                Serial.println("");
                }
#endif
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

#endif // ARDUINO_ARCH_SAMD
