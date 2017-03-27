/* Catena_Log.cpp	Sun Mar 19 2017 01:51:18 tmm */

/*

Module:  Catena_Log.cpp

Function:
	class McciCatena::cLog.

Version:
	V0.5.0	Sun Mar 19 2017 01:51:18 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2017

Revision history:
   0.5.0  Sun Mar 19 2017 01:51:18  tmm
	Module created.

*/

#include "Catena_Log.h"

#include <Arduino.h>
#include <stdio.h>
#include <stdarg.h>

using namespace McciCatena;

// the global instance
cLog McciCatena::gLog;

void 
cLog::printf(
	DebugFlags uDebugFlags,
	const char *fmt, 
	...
	)
	{
	if (! Serial.dtr())
		return;

	// either inbound mask must be zero or must match
	if (this->isenabled(uDebugFlags))
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
