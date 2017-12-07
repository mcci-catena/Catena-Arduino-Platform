/* catena4410_getlorawankeys.cpp	Sun Dec  4 2016 22:26:46 tmm */

/*

Module:  catena4410_getlorawankeys.cpp

Function:
	Catena4410::GetLoRaWANkeys()

Version:
	V0.4.0	Sun Dec  4 2016 22:26:46 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	December 2016

Revision history:
   0.4.0  Sun Dec  4 2016 22:26:46  tmm
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include <Catena4410.h>
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

const Arduino_LoRaWAN::ProvisioningTable *
Catena4410::GetLoRaWANkeys(
	void
	) const
	{
	return &Catena4410::gk_LoRaWAN_Keys;
	}

#endif // ARDUINO_ARCH_SAMD
