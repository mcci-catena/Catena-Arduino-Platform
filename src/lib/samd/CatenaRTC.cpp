/* catenartc.cpp	Mon Nov 26 2018 15:02:35 chwon */

/*

Module:  catenartc.cpp

Function:
	CatenaRTC class

Version:
	V0.12.0	Mon Nov 26 2018 15:02:35 chwon	Edit level 3

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
   0.3.0  Tue Nov  1 2016 23:41:10  tmm
	Module created.

   0.3.0  Thu Nov 10 2016 23:41:10  tmm
	Add debugging code. Disable all other interrupt sources during sleep
	other than RTC (don't let Systick wake us up).

   0.12.0  Mon Nov 26 2018 15:02:36  chwon
	Remove RTCZero library.

*/

#ifdef ARDUINO_ARCH_SAMD

#include <CatenaRTC.h>
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

extern "C" {

static volatile uint32_t *gs_pAlarm;

void RTC_Handler(void)
	{
	if (gs_pAlarm)
		*gs_pAlarm = 1;

	// must clear flag at end
	RTC->MODE2.INTFLAG.reg = RTC_MODE2_INTFLAG_ALARM0;
	}

/* Wait for sync in write operations */
static inline void RtcWaitSynchronize(void)
	{
	while (RTC->MODE2.STATUS.bit.SYNCBUSY)
		;
	}

} /* extern "C" */

bool CatenaRTC::begin(bool fResetTime)
	{
	uint16_t tmp_reg = 0;

	/* turn on digital interface clock */
	PM->APBAMASK.reg |= PM_APBAMASK_RTC;

	/* Configure the 32768Hz Oscillator */
	SYSCTRL->XOSC32K.reg =	SYSCTRL_XOSC32K_ONDEMAND |
				SYSCTRL_XOSC32K_RUNSTDBY |
				SYSCTRL_XOSC32K_EN32K |
				SYSCTRL_XOSC32K_XTALEN |
				SYSCTRL_XOSC32K_STARTUP(6) |
				SYSCTRL_XOSC32K_ENABLE;

	// If the RTC is in clock mode and the reset was
	// not due to POR or BOD, preserve the clock time
	// POR causes a reset anyway, BOD behaviour is?
	bool validTime = false;
	RTC_MODE2_CLOCK_Type oldTime;

	if (! fResetTime &&
	    (PM->RCAUSE.reg & (PM_RCAUSE_SYST | PM_RCAUSE_WDT | PM_RCAUSE_EXT)))
		{
		if (RTC->MODE2.CTRL.reg & RTC_MODE2_CTRL_MODE_CLOCK)
			{
			validTime = true;
			oldTime.reg = RTC->MODE2.CLOCK.reg;
			}
		}

	/* Attach peripheral clock to 32k oscillator */
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(2)|GCLK_GENDIV_DIV(4);
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
		;
	GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN |
			    GCLK_GENCTRL_SRC_XOSC32K |
			    GCLK_GENCTRL_ID(2) |
			    GCLK_GENCTRL_DIVSEL;
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
		;
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |
			    GCLK_CLKCTRL_GEN_GCLK2 |
			    (RTC_GCLK_ID << GCLK_CLKCTRL_ID_Pos);
	while (GCLK->STATUS.bit.SYNCBUSY)
		;

	// disable RTC
	RTC->MODE2.CTRL.reg &= ~RTC_MODE2_CTRL_ENABLE;
	RtcWaitSynchronize();

	// RTC software reset
	RTC->MODE2.CTRL.reg |= RTC_MODE2_CTRL_SWRST;
	RtcWaitSynchronize();

	tmp_reg |= RTC_MODE2_CTRL_MODE_CLOCK; // set clock operating mode
	tmp_reg |= RTC_MODE2_CTRL_PRESCALER_DIV1024; // set prescaler to 1024 for MODE2
	tmp_reg &= ~RTC_MODE2_CTRL_MATCHCLR; // disable clear on match

	//According to the datasheet RTC_MODE2_CTRL_CLKREP = 0 for 24h
	tmp_reg &= ~RTC_MODE2_CTRL_CLKREP; // 24h time representation

	RTC->MODE2.READREQ.reg &= ~RTC_READREQ_RCONT; // disable continuously mode

	RTC->MODE2.CTRL.reg = tmp_reg;
	RtcWaitSynchronize();

	NVIC_EnableIRQ(RTC_IRQn); // enable RTC interrupt
	NVIC_SetPriority(RTC_IRQn, 0x00);

	RTC->MODE2.INTENSET.reg |= RTC_MODE2_INTENSET_ALARM0; // enable alarm interrupt
	RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = MATCH_OFF; // default alarm match is off (disabled)
	RtcWaitSynchronize();

	// enable RTC
	RTC->MODE2.CTRL.reg |= RTC_MODE2_CTRL_ENABLE;
	RtcWaitSynchronize();

	// software reset remove
	RTC->MODE2.CTRL.reg &= ~RTC_MODE2_CTRL_SWRST;
	RtcWaitSynchronize();

	// If desired and valid, restore the time value
	if (! fResetTime && validTime)
		{
		RTC->MODE2.CLOCK.reg = oldTime.reg;
		RtcWaitSynchronize();
		}

        return true;
	}

const uint16_t CatenaRTC::md[13] =
        {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
        };

bool
CatenaRTC::SetTime(const CatenaRTC::CalendarTime *pTime)
	{
        RTC_MODE2_CLOCK_Type Mode2Clock;

	Mode2Clock.bit.SECOND = pTime->Second;
	Mode2Clock.bit.MINUTE = pTime->Minute;
	Mode2Clock.bit.HOUR = pTime->Hour;
	Mode2Clock.bit.DAY = pTime->Day;
	Mode2Clock.bit.MONTH = pTime->Month;
	Mode2Clock.bit.YEAR = pTime->Year;

	RTC->MODE2.CLOCK.reg = Mode2Clock.reg;
	RtcWaitSynchronize();
	return true;
	}

CatenaRTC::CalendarTime
CatenaRTC::GetTime(void)
        {
        CalendarTime result;
        RTC_MODE2_CLOCK_Type Mode2Clock;

	RTC->MODE2.READREQ.reg = RTC_READREQ_RREQ;
	RtcWaitSynchronize();

	Mode2Clock.reg = RTC->MODE2.CLOCK.reg;

	result.Second = Mode2Clock.bit.SECOND;
	result.Minute = Mode2Clock.bit.MINUTE;
	result.Hour = Mode2Clock.bit.HOUR;
	result.Day = Mode2Clock.bit.DAY;
	result.Month = Mode2Clock.bit.MONTH;
	result.Year = Mode2Clock.bit.YEAR;

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
        RTC_MODE2_ALARM_Type Mode2Alarm;

	Mode2Alarm.reg = 0;
	Mode2Alarm.bit.SECOND = pNow->Second;
	Mode2Alarm.bit.MINUTE = pNow->Minute;
	Mode2Alarm.bit.HOUR = pNow->Hour;
	Mode2Alarm.bit.DAY = pNow->Day;
	Mode2Alarm.bit.MONTH = pNow->Month;
	Mode2Alarm.bit.YEAR = pNow->Year;

	RTC->MODE2.Mode2Alarm[0].ALARM.reg = Mode2Alarm.reg;
	RtcWaitSynchronize();
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
	RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = 0x00;
	RtcWaitSynchronize();

	this->m_Alarm = false;
	gs_pAlarm = &this->m_Alarm;

	RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = how;
	RtcWaitSynchronize();

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
                                // Entering standby mode when connected
                                // via the native USB port causes issues.
                                SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
                                __WFI();
                                }
                        break;
                }

	RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = 0x00;
	RtcWaitSynchronize();

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
	uint8_t lastDay = CatenaRTC::md[Month] - firstDay;

	if (leapYear && Month == 2)
		{
		++lastDay;
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
