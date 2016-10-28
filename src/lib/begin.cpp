/* begin.cpp	Tue Oct 25 2016 01:22:51 tmm */

/*

Module:  begin.cpp

Function:
	Catena4410::begin().

Version:
	V0.1.0	Tue Oct 25 2016 01:22:51 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

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

*/

#include <Catena4410.h>

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

bool CatenaSamd21::begin()
    {
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
    UniqueID_buffer_t CpuID;
    uint32_t OperatingFlags;

    /* do the platform begin */
    /* NOTYET */

    /* get the CPU ID */
    this->GetUniqueID(&CpuID);
    this->m_pPlatform = this->GetPlatformForID(
                                &CpuID,
                                &this->m_SysEUI,
                                &OperatingFlags
                                );
    this->m_OperatingFlags =
        (OperatingFlags & ~uClearMask) | uSetMask;

    /* do the serial port raindance */
    if (this->m_OperatingFlags & 
         static_cast<uint32_t>(OPERATING_FLAGS::fUnattended))
        {
        /* do unattended startup */
        delay(2000);
        /* big difference: don't spin */
        if (Serial)
                Serial.begin(115200);
        }
    else
        {
        /* wait a while to make it easier to break in */
        delay(2000);
        while (!Serial)
           /* NOTHING */; // wait for Serial to be initialized

        Serial.begin(115200);
        }

    return true;
    }
