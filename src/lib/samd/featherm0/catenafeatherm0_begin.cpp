/* catenafeatherm0_begin.cpp	Tue Dec 04 2018 13:33:34 chwon */

/*

Module:  catenafeatherm0_begin.cpp

Function:
	CatenaFeatherM0::begin()

Version:
	V0.12.0	Tue Dec 04 2018 13:33:34 chwon	Edit level 1

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
   0.12.0  Tue Dec 04 2018 13:33:34  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaFeatherM0.h"

#include "Catena_Log.h"

using namespace McciCatena;

/*

Name:	CatenaFeatherM0::begin()

Function:
	Set up all the well-known board peripherals.

Definition:
	bool CatenaFeatherM0::begin();

Description:
	Issues begin() for all the CatenaFeatherM0 things.

Returns:
	true for success, false for failure.

*/


bool CatenaFeatherM0::begin()
	{
        gLog.printf(gLog.kTrace, "+CatenaFeatherM0::begin()\n");

        // do the platform selection.
	if (! this->Super::begin())
		{
		gLog.printf(
			gLog.kError,
			"?CatenaFeatherM0::begin:"
			" Super::begin() failed\n"
			);
		return false;
		}

	return true;
	}

#endif // ARDUINO_ARCH_SAMD
