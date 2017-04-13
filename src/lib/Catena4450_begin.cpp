/* Catena4450_begin.cpp	Sun Mar 12 2017 19:19:28 tmm */

/*

Module:  Catena4450_begin.cpp

Function:
	Catena4450::begin()

Version:
	V0.5.0	Sun Mar 12 2017 19:19:28 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2017

Revision history:
   0.5.0  Sun Mar 12 2017 19:19:28  tmm
	Module created.

*/

#include "Catena4450.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	Catena4450::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool Catena4450::begin();

Description:
	Issues begin() for all the Catena4450 things.

Returns:
	true for success, false for failure.

*/


bool Catena4450::begin()
	{
        delay(2000);
        gLog.printf(gLog.kAlways, "+Catena4450::begin()\n");

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

        // start the FRAM
        if (!this->m_Fram.begin())
                return false;

        if (!this->m_Fram.addCommands())
                return false;

	// check whether the FRAM is valid
	if (! this->m_Fram.isValid())
		{
		gLog.printf(
			gLog.kError, 
			"FRAM contents are not valid, resetting\n"
			);
		this->m_Fram.initialize();
		}

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

