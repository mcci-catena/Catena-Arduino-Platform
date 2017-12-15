/* Catena455x.h	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  Catena455x.h

Function:
	class Catena455x: CatenaBase Platform to represent a Catena 455x
	(4550, 4551, etc.)

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

#ifndef _CATENA455X_H_		/* prevent multiple includes */
#define _CATENA455X_H_

#pragma once

#ifndef _CATENASTM32L0LORA_H_
# include "CatenaStm32L0LoRa.h"
#endif

#ifndef _CATENA_FRAM8K_H_
# include "Catena_Fram8k.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

namespace McciCatena {

class Catena455x : public CatenaStm32L0LoRa
	{
public:
        using Super = CatenaStm32L0LoRa;

        // no specific constructor.
        Catena455x() {};

	// uses default destructor

	// neither copyable nor movable
	Catena455x(const Catena455x&) = delete;
	Catena455x& operator=(const Catena455x&) = delete;
	Catena455x(const Catena455x&&) = delete;
	Catena455x& operator=(const Catena455x&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	// methods
	virtual bool begin() override;
        virtual const EUI64_buffer_t *GetSysEUI(void) override;
	virtual const CATENA_PLATFORM *GetPlatformForID(
					const UniqueID_buffer_t *pIdBuffer,
					EUI64_buffer_t *pSysEUI,
					uint32_t *pOperatingFlags
					) override;

        McciCatena::cFram8k *getFram() { return &this->m_Fram; };

        bool getBootCount(uint32_t &bootCount)
                {
                bootCount = this->m_BootCount;
                return true;
                };

protected:
        virtual void registerCommands(void);

private:
	// the FRAM instance
	McciCatena::cFram8k	m_Fram;
        uint32_t		m_BootCount;

        // the known platforms
        static const CATENA_PLATFORM(* const vPlatforms[]);
        static const size_t nvPlatforms;

        // internal methods
        void savePlatform(
                const CATENA_PLATFORM &Platform,
                const EUI64_buffer_t *pSysEUI,
                const uint32_t *pOperatingFlags
                );
	};

/*
|| The LoRaWAN class for the Catena 455x. Assumes The Things Network
*/
class Catena455x::LoRaWAN : public CatenaStm32L0LoRa::LoRaWAN
	{
public:
	using Super = CatenaStm32L0LoRa::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the owrk.
	*/
	LoRaWAN() {};
	LoRaWAN(const lmic_pinmap &pinmap) : Super(pinmap) {};

	bool begin(Catena455x *pParent);
        Catena455x *getCatena() const { return this->m_pCatena; };

protected:
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

private:
	Catena455x *	m_pCatena;

        // initialize the commands
        bool addCommands();
	};

} // namespace McciCatena

/**** end of Catena455x.h ****/
#endif /* _CATENA455X_H_ */
