/* CatenaWingFram2k_registerCommands.cpp	Wed Dec 05 2018 14:33:00 chwon */

/*

Module:  CatenaWingFram2k_registerCommands.cpp

Function:
	CatenaWingFram2k::registerCommands()

Version:
	V0.12.0	Wed Dec 05 2018 14:33:00 chwon	Edit level 2

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
   0.5.0  Sun Mar 19 2017 14:53:33  tmm
	Module created.

   0.12.0  Wed Dec 05 2018 14:33:00  chwon
	Move adding commands to CatenaBase class.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaWingFram2k.h"

#include "Catena_CommandStream.h"

using namespace McciCatena;

/****************************************************************************\
|
|	Manifest constants
|
\****************************************************************************/


/****************************************************************************\
|
|	The command tables
|
\****************************************************************************/


/****************************************************************************\
|
|	The method function
|
\****************************************************************************/

/* protected virtual */
void
CatenaWingFram2k::registerCommands()
	{
	this->Super::registerCommands();
	}

#endif // ARDUINO_ARCH_SAMD
