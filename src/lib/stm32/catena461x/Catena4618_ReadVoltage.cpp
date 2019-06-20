/* Catena4618_ReadAnalog.cpp	Tue Jun 18 2019 14:01:42 lakshmipriyan */

/*

Module:  Catena4618_ReadAnalog.cpp

Function:
	Catena4618::ReadVbat() and Catena4618::ReadVbus()

Version:
	V0.13.0	Tue Jun 18 2019 14:01:42 lakshmipriyan	Edit level 1

Copyright notice:
	This file copyright (C) 2019 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Lakshmi priya Natarajan, MCCI Corporation	June 2019

Revision history:
   0.13.0  Tue Jun 18 2019 14:01:42  lakshmipriyan
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena4618.h"
#include "Catena_Log.h"

#include <Arduino.h>
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

float
Catena4618::ReadVbat(void) const
	{
	float volt = this->ReadAnalog(Catena461x::ANALOG_CHANNEL_VBAT, 1, 1);
	return volt / 1000;
	}

float
Catena4618::ReadVbus(void) const
	{
	// on the Catena 4618, the Vbus divider impedance is very high,
	// so charge transfer from the previous channel causes problems.
	// Therefore, let's read one extra time; the prvious ADC will
	// have had some time to bleed off.
	float volt = this->ReadAnalog(Catena461x::ANALOG_CHANNEL_VBUS, 6, 3);
	return volt / 1000;
	}

#if defined(ARDUINO_MCCI_CATENA_4618) && defined(USBD_LL_ConnectionState_WEAK)

extern "C" {

uint32_t USBD_LL_ConnectionState(void)
	{
	uint32_t vBus;
	bool fStatus;

	fStatus = CatenaStm32L0_ReadAnalog(
			Catena461x::ANALOG_CHANNEL_VBUS, 6, 3, &vBus
			);
	return (fStatus && vBus < 3000) ? 0 : 1;
	}

}

#endif // ARDUINO_MCCI_CATENA_4618

#endif // ARDUINO_ARCH_STM32

/**** end of Catena4618_ReadAnalog.cpp ****/
