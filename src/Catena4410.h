/* Catena4410.h	Sat Oct 15 2016 22:15:02 tmm */

/*

Module:  Catena4410.h

Function:
	Arduino library header for Catena 4410

Version:
	V0.1.0	Sat Oct 15 2016 22:15:02 tmm	Edit level 1

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
   1.00a  Sat Oct 15 2016 22:12:56  tmm
	Module created.

*/

#ifndef _CATENA4410_H_		/* prevent multiple includes */
#define _CATENA4410_H_

#include <stdint.h>
#include <Arduino.h>
#include "mcciadk_guid.h"

class Catena4410
	{
public:
	typedef	uint8_t	UniqueID_buffer_t[128/8];
	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A7,
		};
	enum DIGITAL_PINS
		{
		PIN_ONE_WIRE = 0,
		PIN_SHT10_CLK = 11,
		PIN_SHT10_DATA = 10,
		};
	enum PLATFORM_FLAGS
		{
		fHasLoRa = 1 << 0,
		fHasBLE = 1 << 1,
		fHasWiFi = 1 << 2,
		fHasTtnNycLoRa = 1 << 3,
		fHasBme280 = 1 << 4,
		fHasLux = 1 << 5,
		fHasSoilProbe = 1 << 6,
		fHasSolarPanel = 1 << 7,
		fHasWaterOneWire = 1 << 8,
		};

	Catena4410();
	void GetUniqueID(
		UniqueID_buffer_t pIdBuffer
		);
	float ReadVbat(void);
	void SafePrintf(
		const char *fmt, ...
		);


private:
	};

typedef struct CATENA_PLATFORM_s  CATENA_PLATFORM;
struct CATENA_PLATFORM_s
	{
	MCCIADK_GUID		Guid;
	const CATENA_PLATFORM	*pParent;
	uint32_t		Flags;
	};

//struct CATENA_CPUID_TO_PLATFORM_s
//	{
//	
//	};

/**** end of Catena4410.h ****/
#endif /* _CATENA4410_H_ */
