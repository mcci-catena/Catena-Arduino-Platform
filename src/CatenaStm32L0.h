/* CatenaStm32L0.h	Fri Dec 28 2018 14:03:19 chwon */

/*

Module:  CatenaStm32L0.h

Function:
	Class CatenaStm32L0

Version:
	V0.13.0	Fri Dec 28 2018 14:03:19 chwon	Edit level 3

Copyright notice:
	This file copyright (C) 2017-2018 by

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

   0.11.0  Mon Nov 19 2018 12:11:25  chwon
	Add Sleep() method override and add m_Rtc in the private context.

   0.12.0  Wed Dec 05 2018 14:10:13  chwon
	Remove GetSysEUI() and GetPlatformForID() methods override.
	Add getFRAM() method override.

   0.13.0  Fri Dec 28 2018 14:03:19  chwon
	Add ReadAnalog() method and add CatenaStm32L0_ReadAnalog() prototype.

*/

#ifndef _CATENASTM32L0_H_		/* prevent multiple includes */
#define _CATENASTM32L0_H_

#pragma once

#ifndef _CATENASTM32_H_
# include "CatenaStm32.h"
#endif

#ifndef _CATENA_FRAM8K_H_
# include "Catena_Fram8k.h"
#endif

#ifndef _CATENASTM32L0RTC_H_
# include "CatenaStm32L0Rtc.h"
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

        // start the Stm32L0 level
	virtual bool begin(void) override;

	virtual McciCatena::cFram *getFram(void) override
		{
		return &this->m_Fram;
		};

	bool getBootCount(uint32_t &bootCount)
		{
		bootCount = this->m_BootCount;
		return true;
		};

	uint32_t ReadAnalog(
			uint32_t Channel,
			uint32_t ReadCount = 1,
			uint32_t Multiplier = 1
			) const;

	// get system clock rate in Hz; must be overridden
	virtual uint64_t GetSystemClockRate(void) const override;

	// read the current battery voltage, in engineering units
	virtual float ReadVbat(void) const = 0;
	virtual float ReadVbus(void) const = 0;

	virtual void Sleep(uint32_t howLongInSeconds) override;

protected:
	// methods
	virtual void registerCommands(void);

private:
	// the FRAM instance
	McciCatena::cFram8k		m_Fram;
        uint32_t			m_BootCount;
	McciCatena::CatenaStm32L0Rtc	m_Rtc;
	};

class CatenaStm32L0::LoRaWAN : public Arduino_LoRaWAN_ttn,
                               public McciCatena::cPollableObject
	{
public:
        using Super = Arduino_LoRaWAN_ttn;

	/*
	|| the constructor.
	*/
	LoRaWAN() {};

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
        virtual ProvisioningStyle GetProvisioningStyle(void) override;
        virtual bool GetAbpProvisioningInfo(
        		Arduino_LoRaWAN::AbpProvisioningInfo *
        		) override;
        virtual bool GetOtaaProvisioningInfo(
        		Arduino_LoRaWAN::OtaaProvisioningInfo *
        		) override;
        virtual void NetSaveFCntUp(uint32_t uFCntUp) override;
        virtual void NetSaveFCntDown(uint32_t uFCntDown) override;
        virtual void NetSaveSessionInfo(
        		const SessionInfo &Info,
        		const uint8_t *pExtraInfo,
        		size_t nExtraInfo
        		) override;

	//
	// TODO(tmm@mcci.com) -- the following are not used but are always
	// hanging around even when we have better ways to do things.
	//
private:
	CatenaStm32L0		*m_pCatena;
	};

// this function is called from a trampoline C function that
// needs to invoke analog reads for checking USB presence.
bool CatenaStm32L0_ReadAnalog(
	uint32_t Channel,
	uint32_t ReadCount,
	uint32_t Multiplier,
	uint32_t *pValue
	);

} // namespace McciCatena

/**** end of CatenaStm32L0.h ****/
#endif /* _CATENASTM32L0_H_ */
