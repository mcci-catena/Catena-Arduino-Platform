/*

Module:  CatenaStm32L0_Sleep.cpp

Function:
	Home for CatenaStm32L0::Sleep()

Copyright notice:
	See accompanying LICENSE file.

Author:
	ChaeHee Won, MCCI Corporation	October 2018

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0.h"

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants & typedefs.
|
\****************************************************************************/



/****************************************************************************\
|
|	Read-only data.
|
\****************************************************************************/



/****************************************************************************\
|
|	Variables.
|
\****************************************************************************/


/*

Name:	CatenaStm32L0::Sleep()

Function:
	Put system in sleep mode

Definition:
	void CatenaStm32L0::Sleep(uint32_t howLongInSeconds);

Description:
	This function sets the alarm with given howLongInSeconds and puts the
	system in low power sleep mode. The system will be wake up when alarm
	happen in howLongInSeconds.

Returns:
	No explicit result.

*/

void CatenaStm32L0::Sleep(uint32_t howLongInSeconds)
	{

	this->m_Rtc.SetAlarm(howLongInSeconds);
	this->m_Rtc.SleepForAlarm(CatenaStm32L0Rtc::SleepMode::StopWithLowPowerRegulator);

	// add the number of ms that we were asleep to the millisecond timer.
	// we don't need extreme accuracy.
	this->m_Rtc.AdjustMillisForward(howLongInSeconds * 1000);
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_Sleep.cpp ****/
