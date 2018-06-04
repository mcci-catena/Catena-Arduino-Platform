/* CatenaWingFram2k_begin.cpp	Sun Mar 12 2017 19:19:28 tmm */

/*

Module:  CatenaWingFram2k_begin.cpp

Function:
	CatenaWingFram2k::begin()

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

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaWingFram2k.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	CatenaWingFram2k::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool CatenaWingFram2k::begin();

Description:
	Issues begin() for all the CatenaWingFram2k things.

Returns:
	true for success, false for failure.

*/


bool CatenaWingFram2k::begin()
	{
        delay(2000);
        // gLog.printf(gLog.kAlways, "+CatenaWingFram2k::begin()\n");

        // we must initialize the FRAM before we call our parent, 
        // because FRAM is used for stable storage of platform info.

        // start the FRAM
        if (!this->m_Fram.begin())
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

#endif // ARDUINO_ARCH_SAMD
