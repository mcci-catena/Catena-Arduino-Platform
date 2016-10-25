/* SafePrintf.cpp	Sat Oct 15 2016 22:38:20 tmm */

/*

Module:  SafePrintf.cpp

Function:
	CatenaBase::SafePrintf()

Version:
	V0.1.0	Sat Oct 15 2016 22:38:20 tmm	Edit level 1

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
   0.1.0  Sat Oct 15 2016 22:38:20  tmm
	Module created.

*/

#include "CatenaBase.h"

#include <stdio.h>
#include <stdarg.h>

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
	if (! Serial) 
		return;

	char buf[128];
	va_list ap;
	int nc;

	va_start(ap, fmt);
	nc = vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
	va_end(ap);

	// in case we overflowed:
	buf[sizeof(buf) - 1] = '\0';
	if (Serial) Serial.print(buf);
	}
