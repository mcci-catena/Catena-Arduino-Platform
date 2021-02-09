/*

Module:  CateenaFeatherM0.h

Function:
	Class CatenaFeatherM0

Copyright notice:
	See LICENSE file accompanying this project.

Author:
	Terry Moore, MCCI Corporation   December 2016

*/

#ifndef _CATENAFEATHERM0_H_		/* prevent multiple includes */
#define _CATENAFEATHERM0_H_

#pragma once

#ifndef _CATENASAMD21_H_
# include "CatenaSamd21.h"
#endif

#include <Arduino_LoRaWAN_network.h>

#include <Catena_Sigfox_wapper.h>

namespace McciCatena {

class CatenaFeatherM0 : public CatenaSamd21
	{
public:
        using Super = CatenaSamd21;

        CatenaFeatherM0() {};
	virtual ~CatenaFeatherM0() {};

	// forward reference
	class LoRaWAN;

	// Sigfox binding
	class Sigfox /* forward */;

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

class CatenaFeatherM0::LoRaWAN : public Arduino_LoRaWAN_network,
                                 public cPollableObject
	{
public:
        using Super = Arduino_LoRaWAN_network;

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

class CatenaFeatherM0::Sigfox : public MCCI_Catena_Sigfox,
                                 public cPollableObject
	{
public:
        using Super = MCCI_Catena_Sigfox;

	/*
	|| the constructor.
	*/
	Sigfox() {};

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
	virtual bool GetSigfoxConfiguringInfo(
					SigfoxConfiguringInfo *pProvisioningInfo
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
