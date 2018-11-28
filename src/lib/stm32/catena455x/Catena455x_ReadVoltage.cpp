/* Catena455x_ReadVoltage.cpp	Mon Nov 26 2018 16:18:29 chwon */

/*

Module:  Catena455x_ReadVoltage.cpp

Function:
	Catena455x::ReadVbat() and Catena455x::ReadVbus()

Version:
	V0.12.0	Mon Nov 26 2018 16:18:29 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

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

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena455x.h"

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
Catena455x::ReadVbat(void) const
	{
	float rawVoltage = analogRead(Catena455x::APIN_VBAT_SENSE);
	return rawVoltage * 3.3 / 1024;
	}

float
Catena455x::ReadVbus(void) const
	{
	float rawVoltage = analogRead(Catena455x::APIN_VBUS_SENSE);
	return rawVoltage * 3.3 / 1024;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena455x_ReadVoltage.cpp ****/
