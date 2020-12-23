/* begin.cpp	Wed Dec 05 2018 14:30:26 chwon */

/*

Module:  begin.cpp

Function:
	Catena4410::LoRaWAN::begin()

Version:
	V0.12.0	Wed Dec 05 2018 14:30:26 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2016, 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Tue Oct 25 2016 03:42:18  tmm
	Module created. Note that since this lives in a library with
	lib/begin.cpp, we must have a different file name.

   0.12.0  Wed Dec 05 2018 14:30:26  chwon
	Add debug message.

*/

#ifdef ARDUINO_ARCH_SAMD

#include <CatenaFeatherM0.h>

#include <Catena_Log.h>

#include <mcciadk_baselib.h>
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

bool CatenaFeatherM0::LoRaWAN::begin(
	CatenaFeatherM0 *pParent
	)
	{
	gLog.printf(gLog.kTrace, "+CatenaFeatherM0::LoRaWAN::begin()\n");

	this->m_pCatena = pParent;
	this->m_ulDebugMask |= LOG_VERBOSE | LOG_ERRORS | LOG_BASIC;

	/* first call the base begin */
	if (! this->Arduino_LoRaWAN::begin())
                {
                gLog.printf(
                        gLog.kBug,
                        "?CatenaFeatherM0::LoRaWAN::begin:"
                        " Arduino_LoRaWAN::begin() failed\n"
                        );
		return false;
                }

        /* here's where we do any required post-processing */
        /* (none at the moment) */

        /* indicate success to the client */
        return true;
	}

#endif // ARDUINO_ARCH_SAMD
