/* CatenaStm32.cpp	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  CatenaStm32.cpp

Function:
	Miscellaneous methods for CatenaStm32.

Version:
	V0.6.0	Fri Oct 13 2017 15:19:30 chwon	Edit level 1

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
   0.6.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "CatenaStm32.h"

using namespace McciCatena;

/* public virtual */
bool
CatenaStm32::cSerialReady::isReady() const
	{
        // unfortunately, we can't use !! Serial (or bool(Serial) --
        // it doesn't really check what we want, and then it inserts
        // a 10ms delay unconditionally.
        // so we just check DTR instead.
	return (Serial.dtr());
	}

#endif // ARDUINO_ARCH_STM32

/**** end of CatenaStm32.cpp ****/
