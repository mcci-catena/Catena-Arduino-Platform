/* catenafeatherm0_readvbat.cpp	Wed Dec 06 2017 18:32:28 tmm */

/*

Module:  catenafeatherm0_readvbat.cpp

Function:
	CatenaFeatherM0::ReadVbat()

Version:
	V0.7.0	Wed Dec 06 2017 18:32:28 tmm	Edit level 2

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
   0.1.0  Sat Oct 15 2016 23:33:29  tmm
	Module created.

   0.7.0  Wed Dec 06 2017 18:32:28  tmm
        Refactoring for adding Catena 4551.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaFeatherM0.h"

#include "Arduino.h"
using namespace McciCatena;


float 
CatenaFeatherM0::ReadVbat(void) const
{
  float rawVoltage = analogRead(CatenaFeatherM0::APIN_VBAT_SENSE);
  return rawVoltage * 2 * 3.3 / 1024;
}

#endif // ARDUINO_ARCH_SAMD
