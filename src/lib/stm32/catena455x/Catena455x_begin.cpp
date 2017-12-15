/* Catena455x_begin.cpp	Wed Dec 06 2017 15:31:37 chwon */

/*

Module:  Catena455x_begin.cpp

Function:
	Catena455x::begin()

Version:
	V0.6.0	Wed Dec 06 2017 15:31:37 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2017 by

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
	gLog.printf(gLog.kAlways, "\n+Catena455x::begin()\n");

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

	// set up the command line collector
	this->m_Collector.begin(&Serial, &this->m_SerialReady);
	this->registerObject(&this->m_Collector);

	// set up the command line processor
	this->m_CommandStream.begin(
		&this->m_Collector,
		this
		);

	// register all commands in this stack
	this->registerCommands();

	// and register the FRAM commands
	if (!this->m_Fram.addCommands())
		return false;

	this->m_Fram.getField(
		cFramStorage::StandardKeys::kBootCount,
		this->m_BootCount
		);
	++this->m_BootCount;
	this->m_Fram.saveField(
		cFramStorage::StandardKeys::kBootCount,
		this->m_BootCount
		);
	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena455x_begin.cpp ****/
