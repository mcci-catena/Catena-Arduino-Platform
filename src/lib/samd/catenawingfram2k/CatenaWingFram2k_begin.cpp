/* CatenaWingFram2k_begin.cpp	Wed Dec 05 2018 14:26:50 chwon */

/*

Module:  CatenaWingFram2k_begin.cpp

Function:
	CatenaWingFram2k::begin()

Version:
	V0.12.0	Wed Dec 05 2018 14:26:50 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2017-2018 by

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

   0.12.0  Wed Dec 05 2018 14:26:51  chwon
	Move common initialization code to CatenaBase class.

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
        gLog.printf(gLog.kTrace, "+CatenaWingFram2k::begin()\n");

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
		{
		gLog.printf(
			gLog.kError,
			"?CatenaWingFram2k::begin:"
			" Super::begin() failed\n"
			);
		return false;
		}

        // register all commands in this stack
        this->registerCommands();

        // and register the FRAM commands
        if (!this->m_Fram.addCommands())
        	{
		gLog.printf(
			gLog.kError,
			"?CatenaWingFram2k::begin:"
			" m_Fram.addCommands() failed\n"
			);
		return false;
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

#endif // ARDUINO_ARCH_SAMD
