/* CatenaStm32_begin.cpp	Wed Jan 17 2018 15:35:49 chwon */

/*

Module:  CatenaStm32_begin.cpp

Function:
	CatenaStm32::begin().

Version:
	V0.7.0	Wed Jan 17 2018 15:35:49 chwon	Edit level 2

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

   0.7.0  Wed Jan 17 2018 15:35:49  chwon
	Remove serial port raindance code because Serial.begin() will
	reinitialize USB module.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32.h"
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

Name:	CatenaStm32::begin()

Function:
	Start up the framework on Stm32-based Catenas.

Definition:
	bool CatenaStm32::begin(void);

	bool CatenaStm32::begin(
			uint32_t uOverride);

	bool CatenaStm32::begin(
			uint32_t uClearMask,
			uint32_t uSetMask
			);

Description:
	These three forms of the STM32 startup code all set the operating
	flags according to the masks, and initialize the Stm32-specific
	fields of the Catena object according to platform data.

	The super-class begin() is called before ours is called.

Returns:
	true if initialied correctly, false otherwise.

*/

bool CatenaStm32::begin()
	{
	return this->begin(/* clear */ 0, /* set */ 0);
	}

bool CatenaStm32::begin(uint32_t uOverride)
	{
	return this->begin(/* clear */ ~0, /* set */ uOverride);
	}

bool CatenaStm32::begin(
	uint32_t uClearMask,
	uint32_t uSetMask
	)
	{
	UniqueID_buffer_t CpuID;
	uint32_t OperatingFlags;

	/* do the platform begin */
	gLog.printf(
		gLog.kAlways,
		"+CatenaStm32::begin(%x, %x)\n",
		uClearMask,
		uSetMask
		);
	if (! this->Super::begin())
		{
		gLog.printf(gLog.kError, "CatenaStm32::Super::begin failed\n");
		return false;
		}

	/* get the CPU ID */
	this->GetUniqueID(&CpuID);
	this->m_pPlatform = this->GetPlatformForID(
				&CpuID,
				&this->m_SysEUI,
				&OperatingFlags
				);
	this->m_OperatingFlags = (OperatingFlags & ~uClearMask) | uSetMask;

	return true;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32_begin.cpp ****/
