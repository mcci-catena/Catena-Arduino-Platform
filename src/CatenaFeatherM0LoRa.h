/* CatenaFeatherM0LoRa.h	Sat Mar 11 2017 15:28:20 tmm */

/*

Module:  CatenaFeatherM0LoRa.h

Function:
	class CatenaFeatherM0LoRa;

Version:
	V0.5.0	Sat Mar 11 2017 15:28:20 tmm	Edit level 1

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
   0.5.0  Sat Mar 11 2017 15:28:20  tmm
	Module created.

*/

#ifndef _CATENAFEATHERM0LORA_H_		/* prevent multiple includes */
#define _CATENAFEATHERM0LORA_H_

#pragma once

#ifndef _CATENAFEATHERM0_H_
# include "CatenaFeatherM0.h"
#endif

namespace McciCatena {

class CatenaFeatherM0LoRa : public CatenaFeatherM0
	{
public:
	using Super = CatenaFeatherM0;

	CatenaFeatherM0LoRa() {};
	virtual ~CatenaFeatherM0LoRa() {};

	// forward reference
	class LoRaWAN;

	enum DIGITAL_PINS
		{
		PIN_SX1276_NSS = 8,
		PIN_SX1276_NRESET = 4,
		PIN_SX1276_DIO0 = 3,
		PIN_SX1276_DIO1 = 6,
		PIN_SX1276_DIO2 = 
			Arduino_LoRaWAN::lmic_pinmap::LMIC_UNUSED_PIN,
		};

protected:

private:
	};

class CatenaFeatherM0LoRa::LoRaWAN : public CatenaFeatherM0::LoRaWAN
	{
public:
        using Super = CatenaFeatherM0::LoRaWAN;

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
	// bool begin(CatenaFeatherM0LoRa *pCatena) 

protected:
	/*
	|| we inherit these from the lower level
	*/
	// virtual ProvisioningStyle GetProvisioningStyle(void);

	// virtual bool GetAbpProvisioningInfo();
	// virtual bool GetOtaaProvisioningInfo();

private:
	};

} /* namespace McciCatena */

/**** end of CatenaFeatherM0LoRa.h ****/
#endif /* _CATENAFEATHERM0LORA_H_ */
