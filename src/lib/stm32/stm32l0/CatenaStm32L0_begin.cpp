/* CatenaStm32L0_begin.cpp	Wed Oct 31 2018 10:57:42 chwon */

/*

Module:  CatenaStm32L0_begin.cpp

Function:
	Home for CatenaStm32L0::begin()

Version:
	V0.11.0	Wed Oct 31 2018 10:57:42 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	October 2018

Revision history:
   0.11.0  Wed Oct 31 2018 10:57:42  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32L0.h"
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

Name:	CatenaStm32L0::begin()

Function:
	Start up the framework on Stm32L0-based Catenas.

Definition:
	bool CatenaStm32L0::begin(void);

Description:
	These three forms of the STM32L0 startup code all set the operating
	flags according to the masks, and initialize the Stm32L0-specific
	fields of the Catena object according to platform data.

	The super-class begin() is called before ours is called.

Returns:
	true if initialied correctly, false otherwise.

*/

bool CatenaStm32L0::begin()
	{
	return this->m_Rtc.begin();
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32L0_begin.cpp ****/
