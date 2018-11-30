/* Catena461x_ReadVoltage.cpp	Mon Nov 26 2018 16:18:29 chwon */

/*

Module:  Catena461x_ReadVoltage.cpp

Function:
	Catena461x::ReadVbat() and Catena461x::ReadVbus()

Version:
	V0.12.0	Mon Nov 26 2018 16:18:29 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	November 2018

Revision history:
   0.12.0  Mon Nov 26 2018 16:18:29  chwon
	Module created.

*/

#ifdef ARDUINO_ARCH_STM32

#include "Catena461x.h"
#include "Catena_Log.h"

#include <Arduino.h>
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

float
Catena461x::ReadVbat(void) const
	{
	uint32_t vrefint = analogRead(Catena461x::APIN_VREF_SENSE);
	uint32_t vbat = analogRead(Catena461x::APIN_VBAT_SENSE);
	uint32_t vdda;
	float volt;

	vdda = __LL_ADC_CALC_VREFANALOG_VOLTAGE(vrefint, LL_ADC_RESOLUTION_12B);

	gLog.printf(
		gLog.kAlways,
		"Catena461x::ReadVbat: vrefint=%u vbat=%u vdda=%u\n",
		vrefint,
		vbat,
		vdda
		);
	volt = __LL_ADC_CALC_DATA_TO_VOLTAGE(vdda, vbat, LL_ADC_RESOLUTION_12B);
	return volt / 1000;
	}

float
Catena461x::ReadVbus(void) const
	{
	uint32_t vrefint = analogRead(Catena461x::APIN_VREF_SENSE);
	uint32_t vbus = analogRead(Catena461x::APIN_VBUS_SENSE);
	uint32_t vdda;
	float volt;

	vdda = __LL_ADC_CALC_VREFANALOG_VOLTAGE(vrefint, LL_ADC_RESOLUTION_12B);

	gLog.printf(
		gLog.kAlways,
		"Catena461x::ReadVbus: vrefint=%u vbus=%u vdda=%u\n",
		vrefint,
		vbus,
		vdda
		);
	volt = __LL_ADC_CALC_DATA_TO_VOLTAGE(vdda, vbus, LL_ADC_RESOLUTION_12B);
	return volt / 1000;
	}

#endif // ARDUINO_ARCH_STM32

/**** end of Catena461x_ReadVoltage.cpp ****/
