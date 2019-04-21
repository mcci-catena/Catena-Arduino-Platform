/* CatenaFeatherM0.h	Wed Dec 05 2018 14:12:40 chwon */

/*

Module:  CatenaFeatherM0.h

Function:
	Class CatenaFeatherM0

Version:
	V0.12.0	Wed Dec 05 2018 14:12:40 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2016-2018 by

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

   0.12.0  Wed Dec 05 2018 14:12:41  chwon
	Add CatenaFeatherM0::begin() and override provisioning stuff.

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

	// get system clock rate in Hz. Always fixed for Feathers.
	virtual uint64_t GetSystemClockRate(void) const override
		{
		return 48 * 1000 * 1000;
		}

	// methods
	virtual bool begin() override;

	// read the current battery voltage, in engineering units
	float ReadVbat(void) const;

protected:
	// methods
	virtual const Arduino_LoRaWAN::ProvisioningInfo *
		GetProvisioningInfo(void);

	virtual const Arduino_LoRaWAN::ProvisioningTable *
		GetLoRaWANkeys(void) const
			{
			return nullptr;
			}

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
	LoRaWAN() {};
		
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
	virtual ProvisioningStyle GetProvisioningStyle(void) override;

	virtual bool GetAbpProvisioningInfo(
			AbpProvisioningInfo *pProvisioningInfo
			) override;

	virtual bool GetOtaaProvisioningInfo(
			OtaaProvisioningInfo *pProvisioningInfo
			) override;

	//
	// TODO(tmm@mcci.com) -- the following are not used but are always
	// hanging around even when we have better ways to do things.
	//
private:
	CatenaFeatherM0		*m_pCatena;
	};

} /* namespace McciCatena */

/**** end of CatenaFeatherM0.h ****/
#endif /* _CATENAFEATHERM0_H_ */
