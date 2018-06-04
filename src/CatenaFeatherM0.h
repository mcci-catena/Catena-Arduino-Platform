/* CatenaFeatherM0.h	Sun Mar 12 2017 19:40:46 tmm */

/*

Module:  CatenaFeatherM0.h

Function:
	Class CatenaFeatherM0

Version:
	V0.5.0	Sun Mar 12 2017 19:40:46 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2016-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	December 2016

Revision history:
   0.4.0  Sun Dec  4 2016 19:57:13  tmm
	Module created.

*/

#ifndef _CATENAFEATHERM0_H_		/* prevent multiple includes */
#define _CATENAFEATHERM0_H_

#pragma once

#ifndef _CATENASAMD21_H_
# include "CatenaSamd21.h"
#endif

//#define ARDUINO_LORAWAN_NETWORK_MACHINEQ 1

#ifdef ARDUINO_LORAWAN_NETWORK_TTN
# include <Arduino_LoRaWAN_ttn.h>
using Arduino_LoRaWAN_Network = Arduino_LoRaWAN_ttn;
#elif defined(ARDUINO_LORAWAN_NETWORK_MACHINEQ)
# include <Arduino_LoRaWAN_machineQ.h>
using Arduino_LoRaWAN_Network = Arduino_LoRaWAN_machineQ;
#else
// **** default **** : assume TTN
# include <Arduino_LoRaWAN_ttn.h>
using Arduino_LoRaWAN_Network = Arduino_LoRaWAN_ttn;
#endif

namespace McciCatena {

class CatenaFeatherM0 : public CatenaSamd21
	{
public:
        using Super = CatenaSamd21;

        CatenaFeatherM0() {};
	virtual ~CatenaFeatherM0() {};

	// forward reference
	class LoRaWAN;

	// all FeatherM0s put vbat on A7
	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A7,
		};

	enum DIGITAL_PINS
		{
		PIN_STATUS_LED = 13,
		};

	// read the current battery voltage, in engineering units
	float ReadVbat(void) const;

protected:
        // methods
        virtual const Arduino_LoRaWAN::ProvisioningInfo *GetProvisioningInfo(void);
        virtual const Arduino_LoRaWAN::ProvisioningTable *GetLoRaWANkeys(void) const
                { return nullptr; }

        // instance data
        const CATENA_PLATFORM *m_pPlatform;

private:
	};

class CatenaFeatherM0::LoRaWAN : public Arduino_LoRaWAN_Network,
                                 public cPollableObject
	{
public:
        using Super = Arduino_LoRaWAN_Network;

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
	virtual bool begin(CatenaFeatherM0 *pCatena);

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
	CatenaFeatherM0		*m_pCatena;
	const CATENA_PLATFORM	*m_pPlatform;
	};

} /* namespace McciCatena */

/**** end of CatenaFeatherM0.h ****/
#endif /* _CATENAFEATHERM0_H_ */
