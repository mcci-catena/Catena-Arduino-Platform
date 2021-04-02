/* CatenaWingFram2k_ReadVbat.cpp	Tue Dec 04 2018 13:32:29 chwon */

/*

Module:  CatenaWingFram2k_ReadVbat.cpp

Function:
	CatenaWingFram2k::ReadVbat()

Version:
	V0.12.0	Tue Dec 04 2018 13:32:29 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	December 2018

Revision history:
   0.12.0  Tue Dec 04 2018 13:32:29  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaWingFram2k.h"

#include "Arduino.h"
using namespace McciCatena;


float
CatenaWingFram2k::ReadVbat(void) const
	{
	float rawVoltage = analogRead(CatenaWingFram2k::APIN_VBAT_SENSE);
	return rawVoltage * 2 * 3.3 / 1024;
	}

#endif // ARDUINO_ARCH_SAMD
