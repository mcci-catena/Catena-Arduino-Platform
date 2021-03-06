/* CatenaSamd21_begin.cpp	Wed Dec 05 2018 14:25:52 chwon */

/*

Module:  CatenaSamd21_begin.cpp

Function:
	CatenaSamd21::begin().

Version:
	V0.12.0	Wed Dec 05 2018 14:25:52 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2016-2018 by

		MCCI Corporation
		3520 Krums Corners Road
                Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Tue Oct 25 2016 01:22:51  tmm
	Module created.

   0.5.0  Sat Mar 18 2017 23:14:48  tmm
	Changed module name to match standards.

   0.11.0  Mon Nov 19 2018 12:15:05  chwon
	Add RTC class begin.

   0.12.0  Wed Dec 05 2018 14:25:52  chwon
	Move common initialization code to CatenaBase class.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaSamd21.h"
#include "Catena_Log.h"

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

/*

Name:	CatenaSamd21::begin()

Function:
	Start up the framework on Samd21-based Catenas.

Definition:
	bool CatenaSamd21::begin(void);

	bool CatenaSamd21::begin(
			uint32_t uOverride);

	bool CatenaSamd21::begin(
			uint32_t uClearMask,
			uint32_t uSetMask
			);

Description:
	These three forms of the SAMD21 startup code all set the operating
	flags according to the masks, and initialize the Samd21-specific
	fields of the Catena object according to platform data.

	The super-class begin() is called before ours is called.

Returns:
	true if initialied correctly, false otherwise.

*/

bool CatenaSamd21::begin()
    {
    gLog.printf(gLog.kTrace, "+CatenaSamd21::begin()\n");
    return this->begin(/* clear */ 0, /* set */ 0);
    }

bool CatenaSamd21::begin(uint32_t uOverride)
    {
    return this->begin(/* clear */ ~0, /* set */ uOverride);
    }

bool CatenaSamd21::begin(
    uint32_t uClearMask,
    uint32_t uSetMask
    )
    {
    uint32_t OperatingFlags;

    /* do the platform begin */
    gLog.printf(gLog.kTrace, "+CatenaSamd21::begin(%x, %x)\n", uClearMask, uSetMask);
    if (! this->Super::begin())
        {
        gLog.printf(gLog.kError, "CatenaSamd21::Super::begin failed\n");
        return false;
        }

    OperatingFlags = this->GetOperatingFlags();
    OperatingFlags = (OperatingFlags & ~uClearMask) | uSetMask;
    this->SetOperatingFlags(OperatingFlags);

    /* do the serial port raindance */
    if (OperatingFlags & static_cast<uint32_t>(OPERATING_FLAGS::fUnattended))
        {
        /* do unattended startup */
        delay(2000);
        /* big difference: don't spin */
        if (Serial.dtr())
                Serial.begin(115200);
        }
    else
        {
        /* wait a while to make it easier to break in */
        delay(2000);
        while (! Serial.dtr())
           /* NOTHING */; // wait for Serial to be initialized

        Serial.begin(115200);
        }

    this->m_Rtc.begin();

    return true;
    }

#endif // ARDUINO_ARCH_SAMD
