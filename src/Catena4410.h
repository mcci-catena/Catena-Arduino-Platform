/* Catena4410.h	Mon Oct 24 2016 23:40:17 tmm */

/*

Module:  Catena4410.h

Function:
	Arduino library header for Catena 4410

Version:
	V0.1.0	Mon Oct 24 2016 23:40:17 tmm	Edit level 2

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

#ifndef _CATENASAMD21_H_
# include "CatenaSamd21.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

class Catena4410 : public CatenaSamd21
	{
public:
	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A7,
		};
	enum DIGITAL_PINS
		{
		PIN_ONE_WIRE = 0,
		PIN_SHT10_CLK = 11,
		PIN_SHT10_DATA = 10,
		PIN_SX1276_NSS = 8,
		PIN_SX1276_NRESET = 4,
		PIN_SX1276_DIO0 = 3,
		PIN_SX1276_DIO1 = 6,
		};
	enum PLATFORM_FLAGS : uint32_t
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
	
	/*
	|| Methods
	*/
	Catena4410();

	float ReadVbat(void);

	/*
	|| LoRaWAN binding
	*/
	class LoRaWAN /* forward */;

private:
	const CATENA_PLATFORM *m_pPlatform;
	};

/*
|| The LoRaWAN class for the Catena4410. For now, we assume The Things
|| Network.
*/
class Catena4410::LoRaWAN : public Arduino_LoRaWAN_ttn
	{
public:
	/*
	|| the constructor.
	*/
	LoRaWAN();
		
	/*
	|| the begin function loads data from the local
	|| platform's stable storage and initializes
	|| the connection. 
	*/
	bool begin(void);

protected:
	/*
	|| we have to provide these for the lower level
	*/
	ProvisioningStyle GetProvisioningStyle(void);

	bool GetAbpProvisioningInfo(
			AbpProvisioningInfo *pProvisioningInfo
			);

	bool GetOtaaProvisioningInfo(
			OtaaProvisioningInfo *pProvisioningInfo
			);

private:
	ProvisioningStyle	m_ProvisioningStyle;
	AbpProvisioningInfo	m_AbpProvisioningInfo;
	OtaaProvisioningInfo	m_OtaaProvisioningInfo;
	};

/**** end of Catena4410.h ****/
#endif /* _CATENA4410_H_ */
