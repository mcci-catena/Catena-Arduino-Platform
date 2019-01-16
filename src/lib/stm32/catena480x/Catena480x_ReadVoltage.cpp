/* Catena480x_ReadVoltage.cpp	Wed Jan 16 2019 10:40:07 chwon */

/*

Module:  Catena480x_ReadVoltage.cpp

Function:
	Catena480x::ReadVbat() and Catena480x::ReadVbus()

Version:
	V0.13.0	Wed Jan 16 2019 10:40:07 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2018-2019 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	November 2018

Revision history:
   0.12.0  Mon Nov 26 2018 16:18:29  chwon
	Module created.

   0.13.0  Wed Jan 16 2019 10:40:07  chwon
	Use CatenaStm32L0::ReadAnalog() method.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena480x.h"

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
Catena480x::ReadVbat(void) const
	{
	float volt = this->ReadAnalog(Catena480x::ANALOG_CHANNEL_VBAT, 1, 1);
	return volt / 1000;
	}

float
Catena480x::ReadVbus(void) const
	{
	return 0.0;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena480x_ReadVoltage.cpp ****/
