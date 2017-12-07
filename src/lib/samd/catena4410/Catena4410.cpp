/* Catena4410.cpp	Sat Oct 15 2016 22:50:43 tmm */

/*

Module:  Catena4410.cpp

Function:
	Catena4410::Catea4410()

Version:
	V0.1.0	Sat Oct 15 2016 22:50:43 tmm	Edit level 1

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
   0.1.0  Sat Oct 15 2016 22:50:43  tmm
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "Catena4410.h"

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

Catena4410::Catena4410()
	{
	/* this function does nothing */
	}

#endif // ARDUINO_ARCH_SAMD
