/* CatenaStm32L0LoRa.h	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  CatenaStm32L0LoRa.h

Function:
	Class CateanStm32L0LoRa;

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

#ifndef _CATENASTM32L0LORA_H_		/* prevent multiple includes */
#define _CATENASTM32L0LORA_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

namespace McciCatena {

class CatenaStm32L0LoRa : public CatenaStm32L0
	{
public:
	using Super = CatenaStm32L0;

	CatenaStm32L0LoRa() {};
	virtual ~CatenaStm32L0LoRa() {};

	// forward reference
	class LoRaWAN;

	enum DIGITAL_PINS
		{
		PIN_SX1276_NSS = D7,
		PIN_SX1276_NRESET = D8,
		PIN_SX1276_DIO0 = D25,
		PIN_SX1276_DIO1 = D26,
		PIN_SX1276_DIO2 = D27,
		PIN_SX1276_ANT_SWITCH_RX = D29,
		PIN_SX1276_ANT_SWITCH_TX_BOOST = D30,
		PIN_SX1276_ANT_SWITCH_TX_RFO = D31,
		};

protected:

private:
	};

class CatenaStm32L0LoRa::LoRaWAN : public CatenaStm32L0::LoRaWAN
	{
public:
        using Super = CatenaStm32L0::LoRaWAN;

	/*
	|| the constructor. deliberately an external reference, as pulling
	|| in that module will also bring the pinmap in.
	*/
	LoRaWAN();
        LoRaWAN(const lmic_pinmap &pinmap) : Super(pinmap) {};

	/*
	|| the begin function loads data from the local
	|| platform's stable storage and initializes
	|| the connection. Just use the built-in version.
	*/
	// bool begin(CatenaStm32L0LoRa *pCatena)

protected:
	/*
	|| we inherit these from the lower level
	*/
	// virtual ProvisioningStyle GetProvisioningStyle(void);

	// virtual bool GetAbpProvisioningInfo();
	// virtual bool GetOtaaProvisioningInfo();

private:
	};

} // namespace McciCatena

/**** end of CatenaStm32L0LoRa.h ****/
#endif /* _CATENASTM32L0LORA_H_ */
