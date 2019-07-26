/*

Module:  CatenaStm32L0_GetSystemClockRate.cpp

Function:
	CatenaStm32L0::GetSystemClockRate()

Copyright notice:
	See accompanying license

Author:
	Terry Moore, MCCI Corporation	April 2019

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0.h"

#include <Arduino.h>

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

Name:	CatenaStm32L0::GetSystemCLockRate

Function:
	Return the system clock rate

Definition:
	virtual uint64_t CatenaStm32L0::GetSystemClockRate(
		void
		) const override;

Description:
	This function returns the system clock rate in Hz.

Returns:
	Analog value

*/

uint64_t CatenaStm32L0::GetSystemClockRate(
	void
	) const
	{
	return HAL_RCC_GetHCLKFreq();
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_GetSystemClockRate.cpp ****/
