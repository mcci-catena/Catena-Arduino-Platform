/* CatenaWingFram2k.h	Sun Mar 19 2017 15:50:15 tmm */

/*

Module:  CatenaWingFram2k.h

Function:
	class CatenaWingFram2k: CatenaBase Platform to represent a Catena
        Feather Wing with 2k FRAM and the DIO1>D6 loopback.

Version:
	V0.5.0	Sun Mar 19 2017 15:50:15 tmm	Edit level 2

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
   0.5.0  Sun Mar 12 2017 18:16:07  tmm
	Module created.

*/

#ifndef _CATENAWINGFRAM2K_H_		/* prevent multiple includes */
#define _CATENAWINGFRAM2K_H_

#pragma once

#ifndef _CATENAFEATHERM0LORA_H_
# include "CatenaFeatherM0LoRa.h"
#endif

#ifndef _CATENA_FRAM2K_H_
# include "Catena_Fram2k.h"
#endif

namespace McciCatena {

class CatenaWingFram2k : public CatenaFeatherM0LoRa
	{
public:
        using Super = CatenaFeatherM0LoRa;

        // no specific constructor.
        CatenaWingFram2k() {};

	// uses default destructor

	// neither copyable nor movable
	CatenaWingFram2k(const CatenaWingFram2k&) = delete;
	CatenaWingFram2k& operator=(const CatenaWingFram2k&) = delete;
	CatenaWingFram2k(const CatenaWingFram2k&&) = delete;
	CatenaWingFram2k& operator=(const CatenaWingFram2k&&) = delete;

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

        McciCatena::cFram2k *getFram() { return &this->m_Fram; };

        bool getBootCount(uint32_t &bootCount)
                { 
                bootCount = this->m_BootCount;
                return true;
                };

protected:
        virtual void registerCommands(void);

        // subclasses must provide a method for getting the platform table
        virtual void getPlatformTable(
                const CATENA_PLATFORM * const * &vPlatforms,
                size_t &nvPlatforms
                ) = 0;

private:
	// the FRAM instance
	McciCatena::cFram2k			m_Fram;
        uint32_t                                m_BootCount;

        // internal methods
        void savePlatform(
                const CATENA_PLATFORM &Platform,
                const EUI64_buffer_t *pSysEUI,
                const uint32_t *pOperatingFlags
                );
	};

/*
|| The LoRaWAN class for the Catena Wings. Needed because we have a
|| specific storage implementation and pin table.
*/
class CatenaWingFram2k::LoRaWAN : public CatenaFeatherM0LoRa::LoRaWAN
	{
public:
	using Super = CatenaFeatherM0LoRa::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the owrk.
	*/
	LoRaWAN() {};
	LoRaWAN(const lmic_pinmap &pinmap) : Super(pinmap) {};

        /* start operations: this starts up the LMIC */
	bool begin(CatenaWingFram2k *pParent);

        /* helper to get the Catena pointer for this LoRaWAN instance */
        CatenaWingFram2k *getCatena() const { return this->m_pCatena; };

protected:
        virtual ProvisioningStyle GetProvisioningStyle(void) override;
        virtual bool GetAbpProvisioningInfo(Arduino_LoRaWAN::AbpProvisioningInfo*) override;
        virtual bool GetOtaaProvisioningInfo(Arduino_LoRaWAN::OtaaProvisioningInfo*) override;
        virtual void NetSaveFCntUp(uint32_t uFCntUp) override;
        virtual void NetSaveFCntDown(uint32_t uFCntDown) override;
        virtual void NetSaveSessionInfo(const SessionInfo &Info, const uint8_t *pExtraInfo, size_t nExtraInfo) override;

private:
	CatenaWingFram2k		*m_pCatena;

        // initialize the commands
        bool addCommands();
	};

} /* namespace McciCatena */

/**** end of CatenaWingFram2k.h ****/
#endif /* _CATENAWINGFRAM2K_H_ */
