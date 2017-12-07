/* CatenaStm32L0.h	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  CatenaStm32L0.h

Function:
	Class CatenaStm32L0

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

#ifndef _CATENASTM32L0_H_		/* prevent multiple includes */
#define _CATENASTM32L0_H_

#pragma once

#ifndef _CATENASTM32_H_
# include "CatenaStm32.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

namespace McciCatena {

class CatenaStm32L0 : public CatenaStm32
	{
public:
        using Super = CatenaStm32;

        CatenaStm32L0() {};
	virtual ~CatenaStm32L0() {};

	// forward reference
	class LoRaWAN;

	// all Stm32L0s put vbat on A7
	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A3,
		APIN_VBUS_SENSE = A4,
		};

	enum DIGITAL_PINS
		{
		PIN_STATUS_LED = D13,
		PIN_SPI2_FLASH_SS = D19,
		PIN_SPI2_MOSI = D23,
		PIN_SPI2_MISO = D22,
		PIN_SPI2_SCK = D24,
		};

	// read the current battery voltage, in engineering units
	float ReadVbat(void) const;
	float ReadVbus(void) const;

protected:
        // methods
        virtual const Arduino_LoRaWAN::ProvisioningInfo *GetProvisioningInfo(void);
        virtual const Arduino_LoRaWAN::ProvisioningTable *GetLoRaWANkeys(void) const
                { return nullptr; }

        // instance data
        const CATENA_PLATFORM *m_pPlatform;

private:
	};

class CatenaStm32L0::LoRaWAN : public Arduino_LoRaWAN_ttn,
                                 public McciCatena::cPollableObject
	{
public:
        using Super = Arduino_LoRaWAN_ttn;

	/*
	|| the constructor.
	*/
	LoRaWAN();
        LoRaWAN(const lmic_pinmap &pinmap) : Super(pinmap) {};

	/*
	|| the begin function loads data from the local
	|| platform's stable storage and initializes
	|| the connection.
	*/
	virtual bool begin(CatenaStm32L0 *pCatena);

        virtual void poll() { this->Super::loop(); };

protected:
	/*
	|| we have to provide these for the lower level
	*/
	virtual ProvisioningStyle GetProvisioningStyle(void);

	virtual bool GetAbpProvisioningInfo(
			AbpProvisioningInfo *pProvisioningInfo
			);

	virtual bool GetOtaaProvisioningInfo(
			OtaaProvisioningInfo *pProvisioningInfo
			);

	//
	// TODO(tmm@mcci.com) -- the following are not used but are always
	// hanging around even when we have better ways to do things.
	//
private:
	CatenaStm32L0		*m_pCatena;
	const CATENA_PLATFORM	*m_pPlatform;
	};

} // namespace McciCatena

/**** end of CatenaStm32L0.h ****/
#endif /* _CATENASTM32L0_H_ */
