/*

Module:  CatenaStm32L0.h

Function:
        Class CatenaStm32L0

Copyright notice:
        See LICENSE file accompanying this project.

Author:
        ChaeHee Won, MCCI Corporation   October 2017

*/

#ifndef _CATENASTM32L0_H_               /* prevent multiple includes */
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

#include <Arduino_LoRaWAN_network.h>

#include <Catena_Sigfox_wapper.h>

namespace McciCatena {

class CatenaStm32L0 : public CatenaStm32
        {
public:
        using Super = CatenaStm32;

        CatenaStm32L0() {};
        virtual ~CatenaStm32L0() {};

        // forward reference
        class LoRaWAN;

	// Sigfox binding
	class Sigfox /* forward */;

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

        CatenaStm32L0Rtc::CalendarTime GetCalendarTime(void)
                {
                return this->m_Rtc.GetTime();
                }

        virtual uint32_t CalibrateSystemClock(void) override;

protected:
        // methods
        virtual void registerCommands(void);

private:
        // In future versions, this might change; some STM32L0 implementations
        // might not have a crystal so LSE won't work. But for now, we assume
        // that there's a crystal for LSE.
#if defined(_mcci_arduino_version) 
# if _mcci_arduino_version >= _mcci_arduino_version_calc(2,4,0,90)
        static constexpr bool kUsesLSE = true;          // _mcci_arduino_version indicates that LSE clock is used.
# else
        // versions before 2.4.0.90 use LSI clock. Can't calibrate.
        static constexpr bool kUsesLSE = false;         // _mcci_arduino_version defined, too small
# endif
#else
        // versions before 2.4.0.90 use LSI clock. Can't calibrate.
        static constexpr bool kUsesLSE = false;         // _mcci_arduino_version not defined
#endif

        // return true if this BSP is using the LSE clock.
        static bool usesLseClock() { return kUsesLSE; }

        // the FRAM instance
        McciCatena::cFram8k             m_Fram;
        uint32_t                        m_BootCount;
        McciCatena::CatenaStm32L0Rtc    m_Rtc;
        };

class CatenaStm32L0::LoRaWAN : public Arduino_LoRaWAN_network,
                               public McciCatena::cPollableObject
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
        CatenaStm32L0           *m_pCatena;
        };

class CatenaStm32L0::Sigfox : public MCCI_Catena_Sigfox,
                               public McciCatena::cPollableObject
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
        virtual bool begin(CatenaStm32L0 *pCatena);

        virtual void poll() { this->Super::loop(); };

protected:
        /*
        || we have to provide these for the lower level
        */
        virtual bool GetSigfoxConfiguringInfo(
                        MCCI_Catena_Sigfox::SigfoxConfiguringInfo *
                        ) override;
private:
        CatenaStm32L0           *m_pCatena;
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
