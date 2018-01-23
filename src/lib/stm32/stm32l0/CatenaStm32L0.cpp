/* CatenaStm32L0.cpp	Tue Jan 23 2018 09:38:23 chwon */

/*

Module:  CatenaStm32L0.cpp

Function:
	CatenaStm32L0::ReadVbat()

Version:
	V0.7.0	Tue Jan 23 2018 09:38:23 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2017-2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	ChaeHee Won, MCCI Corporation	October 2017

Revision history:
   0.6.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

   0.7.0  Wed Jan 03 2018 11:03:39  chwon
	Remove multiply 2.

   0.7.0  Tue Jan 23 2018 09:38:23  chwon
	Move yield() and HAL_GetTick() to variant.cpp.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0.h"

#include "Arduino.h"
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
CatenaStm32L0::ReadVbat(void) const
	{
	float rawVoltage = analogRead(CatenaStm32L0::APIN_VBAT_SENSE);
	return rawVoltage * 3.3 / 1024;
	}

float
CatenaStm32L0::ReadVbus(void) const
	{
	float rawVoltage = analogRead(CatenaStm32L0::APIN_VBUS_SENSE);
	return rawVoltage * 3.3 / 1024;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0.cpp ****/
