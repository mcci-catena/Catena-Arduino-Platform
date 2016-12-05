/* ReadVbat.cpp	Sat Oct 15 2016 23:33:29 tmm */

/*

Module:  ReadVbat.cpp

Function:
	CatenaFeatherM0::ReadVbat()

Version:
	V0.1.0	Sat Oct 15 2016 23:33:29 tmm	Edit level 1

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
   0.1.0  Sat Oct 15 2016 23:33:29  tmm
	Module created.

*/

#include "CatenaFeatherM0.h"

#include "Arduino.h"

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

float 
CatenaFeatherM0::ReadVbat(void) const
{
  float rawVoltage = analogRead(CatenaFeatherM0::APIN_VBAT_SENSE);
  return rawVoltage * 2 * 3.3 / 1024;
}

