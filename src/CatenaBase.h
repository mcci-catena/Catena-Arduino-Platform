/*

Module:  CatenaBase.h

Function:
        class CatenaBase interfaces.

Copyright notice:
        See accompanying LICENSE file.

Author:
        Terry Moore, MCCI Corporation   October 2016

*/

#ifndef _CATENABASE_H_          /* prevent multiple includes */
#define _CATENABASE_H_

#pragma once

#ifndef _CATENA_POLLABLEINTERFACE_H_
# include <Catena_PollableInterface.h>
#endif

#include <stdint.h>
#include <Arduino.h>

#ifndef _CATENABASE_TYPES_H_
# include "CatenaBase_types.h"
#endif

#ifndef _CATENA_STREAMLINECOLLECTOR_H_
# include "Catena_StreamLineCollector.h"
#endif

#ifndef _CATENA_COMMANDSTREAM_H_
# include "Catena_CommandStream.h"
#endif

#ifndef _MCCIADK_GUID_H_
# include "mcciadk_guid.h"
#endif

#ifndef _CATENA_FRAM_H_
# include "Catena_Fram.h"
#endif

#include <Arduino_LoRaWAN.h>

#if ! (defined(ARDUINO_LORAWAN_VERSION_COMPARE_LT) && \
        ! ARDUINO_LORAWAN_VERSION_COMPARE_LT(ARDUINO_LORAWAN_VERSION, ARDUINO_LORAWAN_VERSION_CALC(0,9,1,1)))
# error Arduino_LoRaWAN library is out of date. Check ARDUINO_LORAWAN_VERSION.
#endif

// Catena-Arduino-Platform Version: uses semantic version for local (so local == 0 is > non-zero)
#define CATENA_ARDUINO_PLATFORM_VERSION_CALC(major, minor, patch, local)        \
        (((major) << 24u) | ((minor) << 16u) | ((patch) << 8u) | (local))

#define CATENA_ARDUINO_PLATFORM_VERSION \
        CATENA_ARDUINO_PLATFORM_VERSION_CALC(0, 22, 0, 7) /* v0.22.0-pre7 */

#define CATENA_ARDUINO_PLATFORM_VERSION_GET_MAJOR(v)    \
        (((v) >> 24u) & 0xFFu)

#define CATENA_ARDUINO_PLATFORM_VERSION_GET_MINOR(v)    \
        (((v) >> 16u) & 0xFFu)

#define CATENA_ARDUINO_PLATFORM_VERSION_GET_PATCH(v)    \
        (((v) >> 8u) & 0xFFu)

#define CATENA_ARDUINO_PLATFORM_VERSION_GET_LOCAL(v)    \
        ((v) & 0xFFu)

/// \brief convert a semantic version to an integer.
#define CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(v)       \
        (((v) & 0xFFFFFF00u) | (((v) - 1) & 0xFFu))

/// \brief compare two semantic versions
/// \return \c true if \b a is less than \b b (as a semantic version).
#define CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_LT(a, b)   \
        (CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(a) < CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(b))

/// \brief compare two semantic versions
/// \return \c true if \b a is greater than or equal to \b b (as a semantic version).
#define CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_GE(a, b)   \
        (CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(a) >= CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(b))

/// \brief compare two semantic versions
/// \return \c true if \b a is greater than \b b (as a semantic version).
#define CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_GT(a, b)   \
        (CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(a) > CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(b))

/// \brief compare two semantic versions
/// \return \c true if \b a is less than or equal to \b b (as a semantic version).
#define CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_LE(a, b)   \
        (CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(a) <= CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(b))

namespace McciCatena {


/* forward references */
struct CATENA_PLATFORM;

class CatenaBase
        {
public:
        CatenaBase();
        virtual ~CatenaBase() {};

        /*
        || CatenaBase::kMcciArduinoVersion is the MCCI Arduino version, if known
        || If not known, it's zero.
        */
#if defined(_mcci_arduino_version)
        static constexpr uint32_t kMcciArduinoVersion = _mcci_arduino_version;
#else
        static constexpr uint32_t kMcciArduinoVersion = 0;
#endif
#if defined(_mcci_arduino_version_is_semantic)
        static constexpr bool kMcciArduinoVersionIsSemantic = true;
#else
        static constexpr bool kMcciArduinoVersionIsSemantic = false;
#endif
        static constexpr uint32_t versionCalc(uint8_t major, uint8_t minor, uint8_t patch, uint8_t local)
                {
                return (major << 24u) | (minor << 16u) | (patch << 8u) | local;
                }

        /* an EUI64 */
        struct EUI64_buffer_t
                {
                uint8_t b[64/8];
                };
        struct EUI64_string_t
                {
                char    c[sizeof(EUI64_buffer_t) * 3 + 1];
                };

        struct  UniqueID_buffer_t
                {
                uint8_t b[128/8];
                };

        /* a buffer big enough to hold a stringified UniqueID_buffer_t */
        struct UniqueID_string_t
                {
                char    c[sizeof(UniqueID_buffer_t) * 3 + 1];
                };

        /* forward references */
        struct CPUID_PLATFORM_MAP;

        enum OPERATING_FLAGS : uint32_t
                {
                fUnattended = 1 << 0,
                fManufacturingTest = 1 << 1,
                fConfirmedUplink = 1 << 16,
                fDisableDeepSleep = 1 << 17,
                fQuickLightSleep = 1 << 18,
                fDeepSleepTest = 1 << 19,
                };

        // flags that describe generic platform capabilities
        enum PLATFORM_FLAGS : uint32_t
                {
                // platform has LoRa
                fHasLoRa = 1 << 0,
                // platform has Bluetooth LE
                fHasBLE = 1 << 1,
                // platform has Wi-Fi
                fHasWiFi = 1 << 2,
                // platform not only has LoRa, but it's wired according to TTN NYC standards
                fHasTtnNycLoRa = 1 << 3,
                // platform supports the BME280
                fHasBme280 = 1 << 4,
                // platform supports the TSL 2561 Lux meter
                fHasLux = 1 << 5,
                // platform supports soil probe
                fHasSoilProbe = 1 << 6,
                // platform supports external solar panel
                fHasSolarPanel = 1 << 7,
                // platform supports one-wire temperature sensor
                fHasWaterOneWire = 1 << 8,
                // platform not only has LoRa, but it's wired per the MCCI RadioWing standard
                fHasTtnMcciLoRa = 1 << 9,
                // platform has the Rohm Lux meter
                fHasLuxRohm = 1 << 10,
                // platform has i2c mux
                fHasI2cMux = 1 << 11,
                // platform has FRAM
                fHasFRAM = 1 << 12,
                // platform has 1MB or 2MB FLASH
                fHasFlash = 1 << 13,
                // platform has SI 1113 Lux sensor
                fHasLuxSi1133 = 1 << 14,
                // [[deprecated("use fHasLuxSi1133")]]
                fHasLuxSi1113 = fHasLuxSi1133,
                // platform has BME680
                fHasBme680 = 1 << 15,
                // platform has RS485 on Serial1, with A3
                // controlling power and A4 controlling TXE
                fHasRS485 = 1 << 16,
                // platform uses A2 to control VOUT1 (on terminals)
                fHasVout1 = 1 << 17,
                // platform has ZMOD4410 Air Quality sensor
                fHasZMOD4410 = 1 << 18,
                //platform has IDT HS001
                fHasHS001 = 1 << 19,
                //platform has SHT3x sensirion
                fHasSHT3x = 1 << 20,
                //platform has I2C Level Shifter
                fHasI2cLevelShifter = 1 << 21,
                //platform has LTR329 Lux sensor
                fHasLuxLtr329 = 1 << 22,

                // special wiring variants all are offsets from M100...
                // we support up to 127 variants, becuase we have 7
                // bits and variant 0 means "base model".
                fModNumber = 0x7Fu << 25,
                // a few variant values that are well known.
                  fM101 = 0x01 << 25,
                  fM102 = 0x02 << 25,
                  fM103 = 0x03 << 25,
                  fM104 = 0x04 << 25,
                };

        // Get the model number from flags. constexpr to allow for
        // most aggressive optimization.
        static uint32_t constexpr PlatformFlags_GetModNumber(uint32_t flags)
                {
                return (flags & fModNumber) ? 100u + ((flags & fModNumber) / (fModNumber & (~fModNumber + 1u))) : 0;
                };

        // Return true if this unit has been modded. constexpr to allow for
        // most aggressive optimization.
        static bool constexpr PlatformFlags_IsModded(uint32_t flags)
                {
                return (flags & fModNumber) != 0;
                }


        void SafePrintf(
                const char *fmt, ...
                );

        virtual const EUI64_buffer_t *GetSysEUI(void);

        const CATENA_PLATFORM *GetPlatformForID(
                const UniqueID_buffer_t *pIdBuffer,
                EUI64_buffer_t *pSysEUI
                )
                {
                return this->GetPlatformForID(pIdBuffer, pSysEUI, nullptr);
                }

        virtual const CATENA_PLATFORM *GetPlatformForID(
                const UniqueID_buffer_t *pIdBuffer,
                EUI64_buffer_t *pSysEUI,
                uint32_t *pOperatingFlags
                );

        virtual void GetUniqueID(
                UniqueID_buffer_t *pIdBuffer
                ) = 0; // requires that an override be provided.

        char *GetUniqueIDstring(
                UniqueID_string_t *pIdStringBuffer
                );

        const inline CATENA_PLATFORM *GetPlatform(void)
                {
                return this->m_pPlatform;
                }

        inline uint32_t GetOperatingFlags(void)
                {
                return this->m_OperatingFlags;
                }
        inline void SetOperatingFlags(uint32_t OperatingFlags)
                {
                this->m_OperatingFlags = OperatingFlags;
                }

        inline uint32_t GetPlatformFlags(void);

        // get system clock rate in Hz; must be overridden
        virtual uint64_t GetSystemClockRate(void) const = 0;

        // start the Catena framework.
        virtual bool begin(void);

        virtual const char *CatenaName(void) const = 0; // requires that an override be provided.
        virtual void Sleep(uint32_t howLongInSeconds) = 0; // require a concrete method

        virtual McciCatena::cFram *getFram(void)
                {
                return nullptr;
                }

        // poll the engine
        void poll(void);
        void registerObject(McciCatena::cPollableObject *pObject);

        // command handling
        void addCommands(McciCatena::cCommandStream::cDispatch &, void *);

        Arduino_LoRaWAN::ProvisioningStyle GetProvisioningStyle(void);
        bool GetAbpProvisioningInfo(Arduino_LoRaWAN::AbpProvisioningInfo *);
        bool GetOtaaProvisioningInfo(Arduino_LoRaWAN::OtaaProvisioningInfo *);

        void NetSaveSessionInfo(
                const Arduino_LoRaWAN::SessionInfo &Info,
                const uint8_t *pExtraInfo,
                size_t nExtraInfo
                );
        void NetSaveSessionState(
                const Arduino_LoRaWAN::SessionState &State
                );
        bool NetGetSessionState(
                Arduino_LoRaWAN::SessionState &State
                );

        bool addLoRaWanCommands(void);

        // calibrate the system clock, if possible
        virtual uint32_t CalibrateSystemClock(void)
                {
                // if it's not possible; do nothing and return 0.
                return 0;
                }

        // return the command processor object
        cCommandStream *getCommandStream()
                {
                return &this->m_CommandStream;
                }

        /// \brief return the command line collector
        cStreamLineCollector *getCommandCollector()
                {
                return &this->m_Collector;
                }

/****************************************************************************\
|
|       The global CatenaBase pointer.
|
|       Code can't readily reference a global (like "gCatena") that is of
|       a more specialized type than the one they know. So instead, we have
|       a global pointer to the Catena, taken as a pointer to the most
|       general object.  It's initialzed by the constructor.
|
\****************************************************************************/

        static CatenaBase *pCatenaBase;

protected:
        virtual void registerCommands(void);

        // subclasses override a method for getting the platform table
        virtual void getPlatformTable(
                const CATENA_PLATFORM * const * &vPlatforms,
                size_t &nvPlatforms
                )
                {
                vPlatforms = nullptr;
                nvPlatforms = 0;
                }

        // subclasses override a method for getting the CPU ID platform table
        virtual void getCpuIdPlatformTable(
                const CPUID_PLATFORM_MAP * &vCpuIdToPlatform,
                size_t &nvCpuIdToPlatform
                )
                {
                vCpuIdToPlatform = nullptr;
                nvCpuIdToPlatform = 0;
                }

        // help for the command-processing framework.
        class cSerialReady : public McciCatena::cStreamLineCollector::cStreamReady
                {
        public:
                // return true if Serial is ready. Overridden because
                // the Arduino !!Serial() delays 10ms unconditionally!
                // so we need special business.
                virtual bool isReady() const override;
                };

        // the callback object to use for commands (since we're on USB)
        cSerialReady    m_SerialReady;

        // data objects
        EUI64_buffer_t m_SysEUI;
        McciCatena::cPollingEngine m_PollingEngine;

        // the line collector
        McciCatena::cStreamLineCollector        m_Collector;

        // the command processor
        McciCatena::cCommandStream              m_CommandStream;

private:
        uint32_t                m_OperatingFlags;
        const CATENA_PLATFORM * m_pPlatform;

        // internal methods
        void savePlatform(
                const CATENA_PLATFORM &Platform,
                const EUI64_buffer_t *pSysEUI,
                const uint32_t *pOperatingFlags
                );

        const CATENA_PLATFORM *getPlatformForCpuId(
                const UniqueID_buffer_t *pIdBuffer,
                EUI64_buffer_t *pSysEUI,
                uint32_t *pOperatingFlags
                );
        };

/*

Type:   CATENA_PLATFORM

Function:
        Represents common info about any Catena variant.

Description:
        Every Catena model is represented by a CATENA_PLATFORM instance.
        This instance respresents common information about all Catenas of
        that kind.

        The platforms are organized as a tree; each node has a pointer to
        a parent node which is a more general version of the same platform.

Contents:
        MCCIADK_GUID_WIRE Guid;
                The GUID for this platform.

        const CATENA_PLATFORM *pParent;
                The parent platform, or NULL if this is the root for
                this family of models.

        uint32_t PlatformFlags;
                The flags describing the capabilites of this platform. These
                are formed by oring together flags from
                CatenaSam21::PLATFORM_FLAGS.

        uint32_t OperatingFlags;
                Default operating flags. The actual operating flags may be
                modified on a per-device basis.

Notes:
        Typically the platforms are referenced by name from the table of well-
        known CPU IDs, or from the code that supplies the default platform.

See Also:
        CatenaBase, CATENA_CPUID_TO_PLATFORM

*/

struct CATENA_PLATFORM
        {
        MCCIADK_GUID_WIRE       Guid;
        const CATENA_PLATFORM   *pParent;
        uint32_t                PlatformFlags;
        uint32_t                OperatingFlags;
        };

inline uint32_t CatenaBase::GetPlatformFlags(void)
        {
        const CATENA_PLATFORM * const pPlatform = this->m_pPlatform;

        if (pPlatform != nullptr)
                return pPlatform->PlatformFlags;
        else
                return 0;
        }

/*

Type:   CatenaBase::CPUID_PLATFORM_MAP

Function:
        Simple structure to map a CPU ID to a Platform code.

Description:
        Many Catenas don't have NVRAM, and need to have information about
        how the SAMD21 CPU is wired up and connected to the world. However,
        all SAMD21s have a unique CPU ID. We take advantage of this, and the
        relatively large Flash memory of the SAMD21, to map the unique CPU ID
        onto a platform pointer. Unfortunately this requires manually adding
        CPU IDs to the flash table, so this is only suitable for small volume;
        but it works well if the number of CPUs is less than a hundred or so.

        The file gk_WellKnownCpuBindings.cpp instantiates an array,
        gk_WellKnownCpuBindings[], containing CPU ID => platform mappings.

        For convenience, we also allow you to override operating flags on
        a CPU-ID-by-CPU-ID basis.

        Catenas with NVRAM use a similar concept, but the data in the NVRAM
        provides a platform GUID, and the SysEUI, rather than mapping the
        CPUID to a platform pointer. The platform GUID is used to find
        a CATENA_PLATFORM, and the SysEUI provides the identity.

Contents:
        CatenaBase::UniqueID_buffer_t   CpuID;
                The Unique CPU ID.

        const CATENA_PLATFORM *pPlatform;
                A pointer to the platform that defines this assembly.

        CatenaBase::EUI64_buffer_t      SysEUI;
                The EUI64 for this platform. If zero, then none is defined.

        uint32_t OperatingFlagsClear;
        uint32_t OperatingFlagsSet;
                These two entries, taken together, are used to selectively
                clear and set bits in the operating flags. Bits set in
                OperatingFlagsClear will be cleared in the operating flags.
                Bits set in OperatingFlagsSet will be set in the operating
                flags. Clear is applied before set.

Notes:
        The following bash macro is generally used to generate the first
        two lines of initiailization given the output from an MCCI test
        program:

.       function _cpuid {
.               echo "$1" |
.               sed -e 's/^/0x/' -e 's/-/, 0x/g' |
.               awk '{
.                       $1 = "        { CpuID:  { " $1;
.                       $8 = $8 "\n\t\t   ";
.                       $16 = $16 " },";
.                       print }' ;
.       }

        With this macro in scope, you can write (e.g.):

.       _cpuid 05-25-dc-ef-54-53-4b-50-4a-31-2e-39-36-1a-07-ff

        and you'll get the first two lines of a suitable table entry.

See Also:
        gk_WellKnownCpuBindings[], CATENA_PLATFORM

*/

struct CatenaBase::CPUID_PLATFORM_MAP
        {
        CatenaBase::UniqueID_buffer_t   CpuID;

        const CATENA_PLATFORM           *pPlatform;
        CatenaBase::EUI64_buffer_t      SysEUI;
        uint32_t                        OperatingFlagsClear;
        uint32_t                        OperatingFlagsSet;
        };


/*

Name:   CatenaBase::PlatformFlags_GetModNumber()

Index:  Function:       CatenaBase::PlatformFlags_IsModded();

Function:
        Return M101 etc info about this Catena instance given platform flags.

Definition:
        #include <CatenaBase.h>

        static constexpr uint32_t
                CatenaBase::PlatformFlags_GetModNumber(
                        uint32_t uPlatformFlags
                        );


        static constexpr bool
                CatenaBase::PlatformFlags_IsModded(
                        uint32_t uPlatformFlags
                        );

Description:
        Catenas have a "stock" or "base" configuration -- this is how they
        are built by default. At MCCI, we track variants using "M numbers"
        (a concept that we got from the Ithaca electronics scene via Ithaco,
        and ultimately, no doubt, from GE). M numbers are simply unique
        3-digit numbers; normally they start with 101, and are assigned in
        sequence. For example, the Catena 4450-M101 has been optimized for
        AC power measurement use.

        We reserve 7 bits in the platform flags for representing M-numbers.
        Initially, at any rate, your code must know what the numbers mean.

Returns:
        CatenaBase::PlatformFlags_GetModNumber() extracts the mod-number
        from the platform flags, and returns it as a number. If there is no
        mod number for this device, then this will return zero; otherwise it
        returns the mod number (which is always in the range [101..227].

        CatenaBase::PlatformFlags_IsModded() returns true if the platform
        flags indicate that this instance has a non-zero mod number.

*/
// actual function is above.

}; // end namespace McciCatena


/**** end of CatenaBase.h ****/
#endif /* _CATENABASE_H_ */
