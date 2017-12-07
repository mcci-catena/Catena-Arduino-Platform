/* CatenaBase_SafePrintf.cpp	Wed Dec 06 2017 19:01:23 tmm */

/*

Module:  CatenaBase_SafePrintf.cpp

Function:
	CatenaBase::SafePrintf()

Version:
	V0.7.0	Wed Dec 06 2017 19:01:23 tmm	Edit level 3

Copyright notice:
	This file copyright (C) 2016-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Sat Oct 15 2016 22:38:20  tmm
	Module created.

   0.7.0  Wed Dec 06 2017 19:01:23  tmm
        Rename file as part of refactoring.

*/

#include "CatenaBase.h"

#include <stdio.h>
#include <stdarg.h>
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


void 
CatenaBase::SafePrintf(
	const char *fmt, 
	...
	)
	{
	if (! Serial.dtr())
		return;

	char buf[128];
	va_list ap;

	va_start(ap, fmt);
	(void) vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
	va_end(ap);

	// in case we overflowed:
	buf[sizeof(buf) - 1] = '\0';
	if (Serial.dtr()) Serial.print(buf);
	}
