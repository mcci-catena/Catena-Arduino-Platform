/* Catena455x_begin.cpp	Tue Nov 20 2018 13:53:31 chwon */

/*

Module:  Catena455x_begin.cpp

Function:
	Catena455x::begin()

Version:
	V0.11.0	Tue Nov 20 2018 13:53:31 chwon	Edit level 3

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

   0.6.0  Wed Dec 06 2017 15:31:37  chwon
	Set debug flag.

   0.11.0  Tue Nov 20 2018 13:53:31  chwon
	Move common code to CatenaStm32L0::begin().

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena455x.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	Catena455x::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Catena455x::begin();

Description:
	Issues begin() for all the Catena455x things.

Returns:
	true for success, false for failure.

*/

bool Catena455x::begin()
	{
	Serial.begin(115200);
	Wire.begin();
	delay(1000);
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));

	gLog.printf(
		gLog.kInfo,
		"\n+Catena455x::begin() for %s\n",
		CatenaName()
		);

	// we must initialize the FRAM before we call our parent,
	// because FRAM is used for stable storage of platform info.

	// start the FRAM
	if (!this->m_Fram.begin())
		{
		gLog.printf(
			gLog.kError,
			"FRAM begin() failed\n"
			);
		return false;
		}

	// check whether the FRAM is valid
	if (! this->m_Fram.isValid())
		{
		gLog.printf(
			gLog.kError,
			"FRAM contents are not valid, resetting\n"
			);
		this->m_Fram.initialize();
		}

	// do the platform selection.
	if (! this->Super::begin())
		return false;

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena455x_begin.cpp ****/
