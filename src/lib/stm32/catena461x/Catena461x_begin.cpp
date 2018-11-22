/* Catena461x_begin.cpp	Thu Nov 15 2018 14:52:31 chwon */

/*

Module:  Catena461x_begin.cpp

Function:
	Catena461x::begin()

Version:
	V0.11.0	Thu Nov 15 2018 14:52:31 chwon	Edit level 1

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
   0.11.0  Thu Nov 15 2018 14:52:31  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena461x.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	Catena461x::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Catena461x::begin();

Description:
	Issues begin() for all the Catena461x things.

Returns:
	true for success, false for failure.

*/

bool Catena461x::begin()
	{
	Serial.begin(115200);
	Wire.begin();
	delay(1000);
	gLog.begin(cLog::DebugFlags(gLog.kError | gLog.kBug));

	gLog.printf(
		gLog.kAlways,
		"\n+Catena461x::begin() for %s\n",
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

/**** end of Catena461x_begin.cpp ****/
