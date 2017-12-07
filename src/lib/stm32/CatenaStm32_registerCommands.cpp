/* CatenaStm32_registerCommands.cpp	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  CatenaStm32_registerCommands.cpp

Function:
	CatenaStm32::registerCommands()

Version:
	V0.5.0	Fri Oct 13 2017 15:19:30 chwon	Edit level 1

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
   0.5.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32.h"

#include "Catena_CommandStream.h"

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants
|
\****************************************************************************/

static cCommandStream::CommandFn doCpuid;

/****************************************************************************\
|
|	The command tables
|
\****************************************************************************/

static const cCommandStream::cEntry sDefaultEntries[] =
        {
        { "cpuid", doCpuid },
        };

static cCommandStream::cDispatch 
sDispatch(sDefaultEntries, sizeof(sDefaultEntries));

/****************************************************************************\
|
|	The method function
|
\****************************************************************************/

/* protected virtual */
void
CatenaStm32::registerCommands()
	{
	this->Super::registerCommands();

	this->addCommands(sDispatch, (void *) this);
	}

/****************************************************************************\
|
|	The commands
|
\****************************************************************************/

static cCommandStream::CommandStatus
doCpuid(
        cCommandStream *pThis,
	void *pContext,
        int argc,
        char **argv
        )
        {
	CatenaStm32 * const pCatena = static_cast<CatenaStm32 *>(pContext);

	CatenaStm32::UniqueID_string_t CpuIDstring;

	pThis->printf("%s\n", pCatena->GetUniqueIDstring(&CpuIDstring));
        return pThis->kSuccess;
        }

#endif // ARDUINO_ARCH_STM32
