/*

Module:  CatenaWingFram2k.h

Function:
	class CatenaWingFram2k: CatenaBase Platform to represent a Catena
	Feather Wing with 2k FRAM and the DIO1>D6 loopback.

Copyright notice:
	See LICENSE file accompanying this project.

Author:
	Terry Moore, MCCI Corporation	March 2017

*/

#ifndef _CATENAWINGFRAM2K_H_		/* prevent multiple includes */
#define _CATENAWINGFRAM2K_H_

#pragma once

#ifndef _CATENASAMD21_H_
# include "CatenaSamd21.h"
#endif

#ifndef _CATENA_FRAM2K_H_
# include "Catena_Fram2k.h"
#endif

#include <Arduino_LoRaWAN_network.h>

static_assert(
        ! ARDUINO_LORAWAN_VERSION_COMPARE_LT(
                ARDUINO_LORAWAN_VERSION,
                ARDUINO_LORAWAN_VERSION_CALC(0, 9, 0, 1)
                ),
        "ARDUINO_LORAWAN_VERSION must be at least 0.9.0-1"
        );

//
// TODO(tmm@mcci.com)
// For some reason CatenaWingFram2k doesn't depend on CatenaFeatherM0.
// This seems wrong, and should be straightened out.
//

namespace McciCatena {

class CatenaWingFram2k : public CatenaSamd21
	{
public:
        using Super = CatenaSamd21;

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

	// all FeatherM0s put vbat on A7
	enum ANALOG_PINS
		{
		APIN_VBAT_SENSE = A7,
		};

	enum DIGITAL_PINS
		{
		PIN_STATUS_LED = 13,
		};

	// methods
	// get system clock rate in Hz. Always fixed for Feathers.
	virtual uint64_t GetSystemClockRate(void) const override
		{
		return 48 * 1000 * 1000;
		}

	virtual bool begin() override;

        virtual McciCatena::cFram *getFram(void) override
        	{
        	return &this->m_Fram;
        	};

        bool getBootCount(uint32_t &bootCount)
                {
                bootCount = this->m_BootCount;
                return true;
                };

	// read the current battery voltage, in engineering units
	float ReadVbat(void) const;

protected:
        virtual void registerCommands(void);

private:
	// the FRAM instance
	McciCatena::cFram2k			m_Fram;
        uint32_t                                m_BootCount;
	};

/*
|| The LoRaWAN class for the Catena Wings. Needed because we have a
|| specific storage implementation and pin table.
*/
class CatenaWingFram2k::LoRaWAN : public Arduino_LoRaWAN_network,
                                  public McciCatena::cPollableObject
	{
public:
	using Super = Arduino_LoRaWAN_network;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the owrk.
	*/
	LoRaWAN() {};

	/* start operations: this starts up the LMIC */
	bool begin(CatenaWingFram2k *pParent);

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
        virtual void NetSaveSessionInfo(
        		const SessionInfo &Info,
        		const uint8_t *pExtraInfo,
        		size_t nExtraInfo
        		) override;
        virtual void NetSaveSessionState(
                        Arduino_LoRaWAN::SessionState const &State
                        ) override;
        virtual bool NetGetSessionState(
                        Arduino_LoRaWAN::SessionState &State
                        ) override;

private:
	CatenaWingFram2k	*m_pCatena;
	};

} /* namespace McciCatena */

/**** end of CatenaWingFram2k.h ****/
#endif /* _CATENAWINGFRAM2K_H_ */
