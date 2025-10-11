/* Catena5230_ReadAnalog.cpp	Fri Dec 28 2018 14:01:42 chwon */

/*

Module:  Catena5230_ReadAnalog.cpp

Function:
	Catena5230::ReadVbat() and Catena5230::ReadVbus()

Version:
	V0.13.0	Fri Dec 28 2018 14:01:42 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	December 2018

Revision history:
   0.13.0  Fri Dec 28 2018 14:01:42  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena5230.h"
#include "Catena_Log.h"

#include <Arduino.h>
#include <MCCI_Catena_nPM1300.h>

using namespace McciCatena;
using namespace McciCatenaNpm1300;

/****************************************************************************\
|
|		Manifest constants & typedefs.
|
|	This is strictly for private types and constants which will not
|	be exported.
|
\****************************************************************************/

extern McciCatenaNpm1300::cNPM1300 npm1300;

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
Catena5230::ReadVbat(void) const
	{
	float volt = npm1300.measureVbat();
	return volt;
	}

float
Catena5230::ReadVbus(void) const
	{
	float volt = npm1300.measureVbus();
	return volt;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena5230_ReadAnalog.cpp ****/
