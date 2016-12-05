
/*

Module:  CatenaFeatherM0.h

Function:
	Class CatenaFeatherM0

Version:
	V0.4.0	Sun Dec  4 2016 19:57:13 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

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

#include <Arduino_LoRaWAN_ttn.h>

class CatenaFeatherM0 : public CatenaSamd21
	{
public:
	CatenaFeatherM0() {};
	~CatenaFeatherM0() {};

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

class CatenaFeatherM0::LoRaWAN : public Arduino_LoRaWAN_ttn
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
	virtual bool begin(CatenaFeatherM0 *pCatena);

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

private:
	CatenaFeatherM0		*m_pCatena;
	const CATENA_PLATFORM	*m_pPlatform;
	};

/**** end of CatenaFeatherM0.h ****/
#endif /* _CATENAFEATHERM0_H_ */
