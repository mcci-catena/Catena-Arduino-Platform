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
	The clock rate returned is the value set at compile time
	by CATENA_CFG_SYSCLK. The value we return is the correct
	nominal value, in Hz.

Returns:
	Analog value

*/

uint64_t CatenaStm32L0::GetSystemClockRate(
	void
	) const
	{
	switch (CATENA_CFG_SYSCLK)
		{
	case 2:
		return 2097 * 1000;
	case 4:
		return 4194 * 1000;
	case 16:
		return 16 * 1000 * 1000;
	case 24:
		return 24 * 1000 * 1000;
	default:
	case 32:
		return 32 * 1000 * 1000;
		}
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_GetSystemClockRate.cpp ****/
