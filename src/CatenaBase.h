/*

Module:  CatenaBase.h

Function:
        class CatenaBase interfaces.

Copyright notice:
        See accompanying LICENSE file.

Author:
        Terry Moore, MCCI Corporation   October 2016

*/

/// \file

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

// make sure that the library we're calling is supported
#if ! (defined(ARDUINO_LORAWAN_VERSION_COMPARE_LT) && \
        ! ARDUINO_LORAWAN_VERSION_COMPARE_LT(ARDUINO_LORAWAN_VERSION, ARDUINO_LORAWAN_VERSION_CALC(0,9,1,1)))
# error Arduino_LoRaWAN library is out of date. Check ARDUINO_LORAWAN_VERSION.
#endif

/// \brief Pre-processor Catena-Arduino-Platform Version: uses semantic version for local (so local == 0 is > non-zero)
#define CATENA_ARDUINO_PLATFORM_VERSION_CALC(major, minor, patch, local)        \
        (((UINT32_C(0) + (major)) << 24) | ((UINT32_C(0) + (minor)) << 16) | ((UINT32_C(0) + (patch)) << 8) | (UINT32_C(0) + (local)))

///
/// \brief The current version of this library, as a C pre-processor constant
///
/// This is a Semantic Version 2.0 version; the fourth field is the pre-release
/// number.
///
/// \note Always compare versions using one of CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_LT(),
///     CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_LE(), CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_GT(),
///     or CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_GE().
///
/// \sa CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_LT()
///
#define CATENA_ARDUINO_PLATFORM_VERSION \
        CATENA_ARDUINO_PLATFORM_VERSION_CALC(0, 23, 0, 1) /* v0.23.0-pre1 */

/// \brief extract major number from a version code
#define CATENA_ARDUINO_PLATFORM_VERSION_GET_MAJOR(v)    \
        (((UINT32_C(0) + (v)) >> 24) & 0xFFu)

/// \brief extract minor number from a version code
#define CATENA_ARDUINO_PLATFORM_VERSION_GET_MINOR(v)    \
        (((UINT32_C(0) + (v)) >> 16) & 0xFFu)

/// \brief extract patch number from a version code
#define CATENA_ARDUINO_PLATFORM_VERSION_GET_PATCH(v)    \
        (((UINT32_C(0) + (v)) >> 8) & 0xFFu)

///
/// \brief extract local (pre-release) number from a version code
///
/// \note The name is historical, from before we introduced pre-release.
/// \sa CATENA_ARDUINO_PLATFORM_VERSION_GET_PRERELEASE()
///
#define CATENA_ARDUINO_PLATFORM_VERSION_GET_LOCAL(v)    \
        ((UINT32_C(0) + (v)) & 0xFFu)

///
/// \brief extract pre-release number from a version code
///
#define CATENA_ARDUINO_PLATFORM_VERSION_GET_PRERELEASE(v)    \
        ((UINT32_C(0) + (v)) & 0xFFu)


/// \brief convert a semantic version to an ordinal integer that can be compared.
#define CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(v)       \
        (((v) & UINT32_C(0xFFFFFF00) | (((v) - UINT32_C(1)) & 0xFFu))

/// \brief compare two semantic versions.
/// \param[in] a, b versions to compare
/// \return \c true if \p a is less than \p b (as a semantic version).
#define CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_LT(a, b)   \
        (CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(a) < CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(b))

/// \brief compare two semantic versions
/// \param[in] a, b versions to compare
/// \return \c true if \b a is greater than or equal to \b b (as a semantic version).
#define CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_GE(a, b)   \
        (CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(a) >= CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(b))

/// \brief compare two semantic versions
/// \param[in] a, b versions to compare
/// \return \c true if \b a is greater than \b b (as a semantic version).
#define CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_GT(a, b)   \
        (CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(a) > CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(b))

/// \brief compare two semantic versions
/// \param[in] a, b versions to compare
/// \return \c true if \b a is less than or equal to \b b (as a semantic version).
#define CATENA_ARDUINO_PLATFORM_VERSION_COMPARE_LE(a, b)   \
        (CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(a) <= CATENA_ARDUINO_PLATFORM_VERSION_TO_INT(b))

class TwoWire;        // a forward reference.

///
/// \brief The common namespace for this library.
///
namespace McciCatena {

///
/// \brief Represent a Semantic Version constant numerically
///
/// \details
///     Objects of type \c Version_t represent a subset of Semantic Version values,
///     as defined by the Semantic Version 2.0 specification. \c major, \c minor, and
///     \c patch may range from 0 to 255, and have the same meaning as in the specification.
///     \c prerelease, if not zero, indicates that this version is a pre-release for
///     the specified \c major.minor.patch release. Relational operators are defined
///     so that pre-releases will compare less than the corresponding releases.
///
class Version_t
        {
public:
        /// \brief create a version constant uint32_t
        static constexpr uint32_t makeVersion(uint8_t major, uint8_t minor, uint8_t patch, uint8_t prerelease)
                {
                return (uint32_t(major) << 24) | (uint32_t(minor) << 16) | (uint32_t(patch << 8)) | prerelease;
                }

        /// \brief construct a \c Version_t object from parts
        constexpr Version_t(uint8_t major, uint8_t minor, uint8_t patch, uint8_t prerelease = 0)
                : m_version(makeVersion(major, minor, patch, prerelease)) {}

        /// \brief default constuctor
        Version_t() {};

        /// \brief construct a \c Version_t object given a code.
        constexpr Version_t(uint32_t versionCode)
                : m_version(versionCode) {}

        /// \brief return the version constant as a uint32_t. Can't be compared!
        constexpr uint32_t getUint32() const
                {
                return this->m_version;
                }

        /// \brief return the version as a sequential constant. Can be compared, but doesn't match what was given.
        constexpr uint32_t getOrdinal() const
                {
                return (this->m_version & 0xFFFFFF00u) | ((this->m_version - 1) & 0xFFu);
                }

        /// \brief basic relational comparison operator for \c Version_t objects
        friend constexpr bool operator<(const Version_t &lhs, const Version_t &rhs) { return lhs.getOrdinal() < rhs.getOrdinal(); }

        /// \brief basic identity comparison operator for \c Version_t objects
        friend constexpr bool operator==(const Version_t &lhs, const Version_t &rhs) { return lhs.getUint32() == rhs.getUint32(); }

        /// \brief return the Semantic Version major version of a \c Version_t value
        constexpr uint8_t getMajor() const
                {
                return uint8_t(this->m_version >> 24);
                }

        /// \brief return the Semantic Version minor version of a \c Version_t value
        constexpr uint8_t getMinor() const
                {
                return uint8_t(this->m_version >> 16);
                }

        /// \brief return the Semantic Version patch number from a \c Version_t value.
        constexpr uint8_t getPatch() const
                {
                return uint8_t(this->m_version >> 8);
                }

        /// \breif return the Semantic Version pre-release from a \c Version_t value.
        constexpr uint8_t getPrerelease() const
                {
                return uint8_t(this->m_version >> 0);
                }

        constexpr bool isPrerelease() const
                {
                return this->getPrerelease() != 0;
                }

private:
        uint32_t m_version;     ///< the encoded version number
        };

/// derived relational operator
constexpr bool operator> (const Version_t& lhs, const Version_t& rhs){ return rhs < lhs; }
/// derived relational operator
constexpr bool operator<=(const Version_t& lhs, const Version_t& rhs){ return !(lhs > rhs); }
/// derived relational operator
constexpr bool operator>=(const Version_t& lhs, const Version_t& rhs){ return !(lhs < rhs); }
/// derived identity operator
constexpr bool operator!=(const Version_t& lhs, const Version_t& rhs){ return !(lhs == rhs); }

/* forward reference */
struct CATENA_PLATFORM;

///
/// \brief Base class for power controls
///
/// \details
///     Being battery-powered devices, Catenas have implementation-specific
///     features for controlling local power. A specific Catena instance will
///     provide a number of objects derived from McciCatena::cPowerControl.
///
///     Concrete types derived from cPowerControl include dummy (cPowerControlDummy),
///     power supplies controlled by GPIOs (cPowerControlGPIO), and nested
///     (or shared) power supplies (cPowerControlNested). The concrete McciCatena::Catena
///     object will set up the relationships.
///
///     The portable methods for controlling specific supplies are defined in CatenaBase,
///     and supplied by McciCatena::Catena (or sometimes by the concrete CatenaXXXX object.)
///
class cPowerControl
        {
public:
        cPowerControl() {}
        ///
        /// \brief Enable/disable power
        ///
        /// \param fRequest indicates whether power is to be requested (true)
        ///     or whether a request is to be released (false).
        ///
        /// \return
        ///     Number of milliseconds to delay for change to take effect.
        ///
        ///     Power controls maintain a reference count of power requests.
        ///     The count is initially zero. It increments with each true
        ///     fRequest, and it decrements with each false fRequest. Thus,
        ///     enable/disable requests nest.
        ///
        ///     When the control changes state from off to on, the control
        ///     typically will return a non-zero number of millilseconds to
        ///     delay to allow the power supply to turn on.  Similarly, when
        ///     the control changes state from on to off, the concrete control
        ///     will return a count of milliseconds to delay to allow the
        ///     power supply to turn off.
        ///
        ///     By convention, a group of related controls will turn on the
        ///     related power supply whenever an control in the group is
        ///     requesting power; and will turn off the related power supply
        ///     whenever no controls are requesting power.
        ///    
        virtual uint32_t enable(bool fRequest) = 0;

        ///
        /// \brief Get current power request state for a control.
        ///
        /// \return
        ///     `true` if control is logically requesting power, `false` otherwise.
        ///     The result is not specified if this->hasControl() is false. 
        ///
        virtual bool getRequest() const = 0;

        ///
        /// \brief Query whether this power control object implements control.
        ///
        /// \return
        ///     \c true if this->enable() actually manipulates power.
        ///
        ///     For consistency, it's convenient to provide power controls,
        ///     even if they're not implemented. For example, you might want
        ///     to make sure power to the first screw terminal is turned on.
        ///     However, it may also be important to know whether power
        ///     control is really implemented on the board that's the target
        ///     of this compilation. Hence this API.
        ///
        virtual bool hasControl() const = 0;
        };

///
/// \brief Concrete power control object for unimplemented power features.
///
/// \details
///     We model unimplemented power features as "always on". We don't
///     bother to waste effort incrementing/decrementing counters.
///
class cPowerControlDummy : public cPowerControl
        {
public:
        ///
        /// \brief Implment power control for a dummy control.
        ///
        /// \return Always zero, as no delay is ever needed.
        ///
        virtual uint32_t enable(bool fRequest) override
                {
                return 0;
                }
        ///
        /// \brief Get power-request state for a dummy control.
        ///
        /// \return \c true, as a dummy control is modeled as
        ///     a power supply that's aways on.
        ///
        virtual bool getRequest() const override
                {
                return true;
                }
        ///
        /// \brief Return appropriate results for a dummy power control.
        ///
        /// \return \c false, because this control can't be enabled/disabled.
        ///
        virtual bool hasControl() const override
                {
                return false;
                }

private:
        };

///
/// \brief Represent a power supply controlled by a GPIO.
///
/// \details
///     Often, power supplies are controled by a GPIO that is active
///     high. This version of a cPowerControl implements such a
///     supply.
///
///     GPIO controls are characterized by two parameters: the
///     (Arduino) pin that controls the supply, and the delay (in
///     milliseconds to wait when changing the state of the power
///     supply. These are supplied to the constructor.
///
class cPowerControlGPIO : public cPowerControl
        {
public:
        ///
        /// \brief Construct a GPIO-controlled power supply.
        ///
        /// \param [in] pin species the GPIO pin that controls the supply.
        /// \param [in] delayMs the delay to request, in milliseconds, after any change.
        ///
        cPowerControlGPIO(uint8_t pin, uint16_t delayMs)
                : m_delayOff(delayMs)
                , m_delayOn(delayMs)
                , m_pin(pin)
                {}

        ///
        /// \brief Request power from a GPIO-controlled supply.
        ///
        /// If the state changes from off to on, the output pin is configured,
        /// and then the pin is driven high.  If the state changes from on to off,
        /// the pin is driven low.
        ///
        /// \return Number of milliseconds to delay. Zero if not changing state.
        ///
        virtual uint32_t enable(bool fRequest) override
                {
                uint8_t oldCount = this->m_count;

                if (fRequest)
                        {
                        this->m_count = oldCount + 1;
                        if (oldCount == 0)
                                {
                                pinMode(this->m_pin, OUTPUT);
                                digitalWrite(this->m_pin, 1);
                                return this->m_delayOn;
                                }
                        }
                else
                        {
                        this->m_count = oldCount - 1;
                        if (oldCount == 1)
                                {
                                digitalWrite(this->m_pin, 0);
                                return this->m_delayOff;
                                }
                        }
                return 0;
                }
        virtual bool getRequest() const override
                {
                return this->m_count != 0;
                }
        virtual bool hasControl() const override
                {
                return false;
                }

private:
        uint16_t m_delayOn;
        uint16_t m_delayOff;
        uint8_t m_pin;
        uint8_t m_count = 0;
        };

///
/// \brief Nested/shared power control
///
/// \details
///     It's common for functions to share a common power supply. It's
///     also common for the sharing arrangements to be different on
///     different Catena boards. This type of control lets the control
///     of several logical functions be aggregated in a single logical
///     supply.
/// 
class cPowerControlNested : public cPowerControl
        {
public:
        ///
        /// \brief Instantiate a nested supply.
        ///
        /// \param [in] parent refers to the parent power supply,
        ///     which is typicaly (but not necessarily) a cPowerControlGPIO
        ///     object.
        ///
        /// The parent object is reference is stored internally.
        ///
        cPowerControlNested(cPowerControl &parent)
                : m_parent(parent)
                {}
        ///
        /// \brief Reqeust a change to a nested supply.
        ///
        /// \param [in] fRequest signals the desired state.
        ///
        /// Whenever the local state changes from not-requested to requested,
        /// the parent object is requested. Similary, whenever the local
        /// state changes from requested to not-requested, the parent object
        /// is released.
        ///
        /// \return The number of milliseconds to delay to allow the power
        ///     suppply to settle. This is zero unless we try to change the
        ///     state of the parent; and in that case, it's the value retured
        ///     by the parent.
        ///
        virtual uint32_t enable(bool fRequest) override
                {
                uint8_t oldCount = this->m_count;
                if (fRequest)
                        {
                        this->m_count = oldCount + 1;
                        if (oldCount == 0)
                                return this->m_parent.enable(true);
                        }
                else
                        {
                        if (oldCount > 0)
                                this->m_count = oldCount - 1;
                        if (oldCount == 1)
                                return this->m_parent.enable(false);
                        }
                return 0;
                }
        virtual bool getRequest() const override
                {
                return this->m_count != 0;
                }
        virtual bool hasControl() const override
                {
                return this->m_parent.hasControl();
                }

private:
        cPowerControl &m_parent;
        uint8_t m_count = 0;
        };

///
/// \brief Base class for all Catena objects
///
class CatenaBase
        {
public:
        /// \brief The constructor for the common behavior of any `Catena`-like object.
        CatenaBase();

        /// \brief the destructor; virtual because this is a base class.
        virtual ~CatenaBase() {};

        ///
        /// \property kMcciArduinoVersion
        ///
        /// \brief the MCCI Arduino verion, if known. If not known, set to zero.
        ///
#if defined(_mcci_arduino_version)
        static constexpr uint32_t kMcciArduinoVersion = _mcci_arduino_version;
#else
        static constexpr uint32_t kMcciArduinoVersion = 0;
#endif

        ///
        /// \property kMcciArduinoVersionIsSemantic
        ///
        /// \brief \c true if the MCCI Arduino version is known to be a semantic
        ///     verision, i.e., if non-zero last fields are pre-release numbers.
        ///     Otherwise \c false.
        ///
#if defined(_mcci_arduino_version_is_semantic)
        static constexpr bool kMcciArduinoVersionIsSemantic = true;
#else
        static constexpr bool kMcciArduinoVersionIsSemantic = false;
#endif
        ///
        /// \brief calculate a version number from the canonical four components.
        ///
        /// \param major is the major version number
        /// \param minor is the minor version number
        /// \param patch is the patch number
        /// \param prerelease is the pre-release number
        ///
        /// \note Semantic Version non-zero pre-releases require care.
        ///
        [[deprecated("Use Version_t::makeVersion instead")]]
        static constexpr uint32_t versionCalc(uint8_t major, uint8_t minor, uint8_t patch, uint8_t prerelease)
                {
                return (uint32_t(major) << 24) | (uint32_t(minor) << 16) | (uint32_t(patch) << 8) | prerelease;
                }

        /// \brief the version of the library as a constexpr
        static constexpr Version_t kVersion = Version_t(CATENA_ARDUINO_PLATFORM_VERSION);

        ///
        /// \brief extract the base name of a file
        ///
        /// \param[in] s points the best candidate for the base name.
        /// \param[in] p (if non-NULL) points to the next character to be considered.
        ///
        /// The string at s is scanned until the last '/' or '\\' is found, and
        /// a pointer to the remainder of the string is returned. If \c s is NULL,
        /// a pointer to an empty string is returned.
        ///
        /// \note
        ///     This is written as a constexpr using recursion (not iteration)
        ///     for maximum portability.
        //
        static constexpr const char *
                getBaseFileName(
                        const char *s,
                        const char *p = nullptr
                        )
                {
                return s == nullptr                  ? ""                            :
                       p == nullptr                  ? getBaseFileName(s, s)         :
                       p[0] == '\0'                  ? s                             :
                       (p[0] == '/' || p[0] == '\\') ? getBaseFileName(p + 1, p + 1) :
                                                       getBaseFileName(s, p + 1)     ;
                }

        ///
        /// \brief a buffer for carrying an EUI64.
        ///
        struct EUI64_buffer_t
                {
                uint8_t b[64/8];
                };

        ///
        /// \brief a string buffer for carrying the canonical representation of
        /// an EUI64, in hexadecimal with separators between bytes and a trailing
        /// '\0'.
        ///
        struct EUI64_string_t
                {
                char    c[sizeof(EUI64_buffer_t) * 3 + 1];
                };

        ///
        /// \brief a buffer for carrying the unique ID of the underlying chip.
        ///
        struct  UniqueID_buffer_t
                {
                uint8_t b[128/8];
                };

        /// \brief a buffer big enough to hold a stringified UniqueID_buffer_t
        struct UniqueID_string_t
                {
                char    c[sizeof(UniqueID_buffer_t) * 3 + 1];
                };

        /* forward references */
        struct CPUID_PLATFORM_MAP;

        ///
        /// \brief the well-known bits in the operating flags for the device.
        ///
        /// Of these flags, only kUnattended and fManufacturingTest should be used
        /// in new code.
        ///
        enum OPERATING_FLAGS : uint32_t
                {
                fUnattended = 1 << 0,                           ///< operating unattended.
                fManufacturingTest = 1 << 1,                    ///< manufacturing test mode.
                fConfirmedUplink [[deprecated]] = 1 << 16,      ///< request confirmed LoRaWAN uplinks.
                fDisableDeepSleep [[deprecated]] = 1 << 17,     ///< disable deep sleep for testing
                fQuickLightSleep [[deprecated]] = 1 << 18,      ///< use quick, light sleep.
                fDeepSleepTest [[deprecated]] = 1 << 19,        ///< test deep sleep.
                };

        ///
        /// \brief These flags describe generic platform capabilities.
        ///
        /// The use of these flags is historical. New code should not use them.
        ///
        enum PLATFORM_FLAGS : uint32_t
                {
                /// platform has LoRa
                fHasLoRa = 1 << 0,
                /// platform has Bluetooth LE
                fHasBLE = 1 << 1,
                /// platform has Wi-Fi
                fHasWiFi = 1 << 2,
                /// platform not only has LoRa, but it's wired according to TTN NYC standards
                fHasTtnNycLoRa = 1 << 3,
                /// platform supports the BME280
                fHasBme280 = 1 << 4,
                /// platform supports the TSL 2561 Lux meter
                fHasLux = 1 << 5,
                /// platform supports soil probe
                fHasSoilProbe = 1 << 6,
                /// platform supports external solar panel
                fHasSolarPanel = 1 << 7,
                /// platform supports one-wire temperature sensor
                fHasWaterOneWire = 1 << 8,
                /// platform not only has LoRa, but it's wired per the MCCI RadioWing standard
                fHasTtnMcciLoRa = 1 << 9,
                /// platform has the Rohm Lux meter
                fHasLuxRohm = 1 << 10,
                /// platform has i2c mux
                fHasI2cMux = 1 << 11,
                /// platform has FRAM
                fHasFRAM = 1 << 12,
                /// platform has 1MB or 2MB FLASH
                fHasFlash = 1 << 13,
                /// platform has SI 1133 Lux sensor
                fHasLuxSi1133 = 1 << 14,
                fHasLuxSi1113 [[deprecated("use fHasLuxSi1133")]] = fHasLuxSi1133,
                /// platform has BME680
                fHasBme680 = 1 << 15,
                /// platform has RS485 on Serial1, with A3
                /// controlling power and A4 controlling TXE
                fHasRS485 = 1 << 16,
                /// platform uses A2 to control VOUT1 (on terminals)
                fHasVout1 = 1 << 17,
                /// platform has ZMOD4410 Air Quality sensor
                fHasZMOD4410 = 1 << 18,
                /// platform has IDT HS001
                fHasHS001 = 1 << 19,
                /// platform has SHT3x sensirion
                fHasSHT3x = 1 << 20,
                /// platform has I2C Level Shifter
                fHasI2cLevelShifter = 1 << 21,

                /// special wiring variants all are offsets from M100...
                /// we support up to 127 variants, becuase we have 7
                /// bits and variant 0 means "base model".
                fModNumber = 0x7Fu << 25,
                // a few variant values that are well known.
                  fM101 = 0x01 << 25,   ///< this is an M101 platform
                  fM102 = 0x02 << 25,   ///< this is an M102 platform
                  fM103 = 0x03 << 25,   ///< this is an M103 platform
                  fM104 = 0x04 << 25,   ///< this is an M104 platform
                };

        /// \brief Get the model number from flags. constexpr to allow for
        /// most aggressive optimization.
        ///
        /// \details
        ///     Catenas have a "stock" or "base" configuration -- this is how they
        ///     are built by default. At MCCI, we track variants using "M numbers"
        ///     (a concept that we got from the Ithaca electronics scene via Ithaco,
        ///     and ultimately, no doubt, from GE). M numbers are simply unique
        ///     3-digit numbers; normally they start with 101, and are assigned in
        ///     sequence. For example, the Catena 4450-M101 has been optimized for
        ///     AC power measurement use.
        ///
        ///     We reserve 7 bits in the platform flags for representing M-numbers.
        ///     Initially, at any rate, your code must know what the numbers mean.
        ///
        /// \return This function extracts the mod-number
        ///     from the platform flags, and returns it as a number. If there is no
        ///     mod number for this device, then this will return zero; otherwise it
        ///     returns the mod number (which is always in the range [101..227].
        ///
        /// \sa PlatformFlags_IsModded()
        //
        static uint32_t constexpr PlatformFlags_GetModNumber(uint32_t flags)
                {
                return (flags & fModNumber) ? 100u + ((flags & fModNumber) / (fModNumber & (~fModNumber + 1u))) : 0;
                };

        ///
        /// \brief Return true if this unit has been modded. constexpr to allow for
        /// most aggressive optimization.
        ///
        /// \sa PlatformFlags_GetModNumber()
        ///
        static bool constexpr PlatformFlags_IsModded(uint32_t flags)
                {
                return (flags & fModNumber) != 0;
                }

        /// \brief print with format to \c Serial, but don't block if using
        /// USB Serial and the host is not connected.
        void SafePrintf(
                const char *fmt, ...
                );

        /// \brief return the SysEUI for this system, AKA the serial number.
        virtual const EUI64_buffer_t *GetSysEUI(void);

        /// \brief return the CATENA_PLATFORM object given the CPU ID and the serial number.
        const CATENA_PLATFORM *GetPlatformForID(
                const UniqueID_buffer_t *pIdBuffer,
                EUI64_buffer_t *pSysEUI
                )
                {
                return this->GetPlatformForID(pIdBuffer, pSysEUI, nullptr);
                }

        ///
        /// \brief find the appropriate CATENA_PLATFORM object given the CPU ID and the serial number.
        /// set \c *pOperatingFlags to the default value of the operating flags for this platform.
        ///
        virtual const CATENA_PLATFORM *GetPlatformForID(
                const UniqueID_buffer_t *pIdBuffer,
                EUI64_buffer_t *pSysEUI,
                uint32_t *pOperatingFlags
                );

        ///
        /// \brief return the UniqueID for this CPU/SOC.
        ///
        virtual void GetUniqueID(
                UniqueID_buffer_t *pIdBuffer
                ) = 0; // requires that an override be provided.

        ///
        /// \brief return the UniqueID for this CPU/SOC, as a string.
        ///
        char *GetUniqueIDstring(
                UniqueID_string_t *pIdStringBuffer
                );

        ///
        /// \brief return the platform structure we are to use
        ///
        const inline CATENA_PLATFORM *GetPlatform(void) const
                {
                return this->m_pPlatform;
                }

        ///
        /// \brief return the current operating flags
        ///
        inline uint32_t GetOperatingFlags(void) const
                {
                return this->m_OperatingFlags;
                }

        ///
        /// \brief set the operating flags
        ///
        inline void SetOperatingFlags(uint32_t OperatingFlags)
                {
                this->m_OperatingFlags = OperatingFlags;
                }

        ///
        /// \brief get the platform flags.
        ///
        inline uint32_t GetPlatformFlags(void);

        /// \brief get system clock rate in Hz; must be overridden
        virtual uint64_t GetSystemClockRate(void) const = 0;

        ///
        /// \brief start the Catena framework.
        ///
        /// \return \c true if things were successfully started.
        ///
        virtual bool begin(void);

        /// \name Setup & Configuration
        ///
        /// \details
        ///     Methods for dealing with Catena configuration variations.
        /// \{

        ///
        /// \brief set up everything on this particular board.
        ///
        ///     This should be called from your `setup()` function.
        ///
        /// \return \c true if setup was successful.
        ///
        virtual bool setup(void) = 0;

        //********************************************************************
        ///
        /// \name 3.3V Boost Regulator
        ///
        /// \details
        ///     Some Catenas have an on-board 3.3V boost regulator. These
        ///     APIs provide generic access to that feature.
        ///
        //********************************************************************

        ///     \{

        ///
        /// \brief enable the on-board 3.3v boost regulator. Ignored if not supported.
        ///
        /// \param [in] fRequest \c true to request power, \c false to retract a previous
        ///             request.
        ///
        /// \return time (from millis()) when power will be ready.
        ///
        virtual uint32_t enable_3v3Boost(bool fRequest) = 0;

        ///
        /// \brief return \c true if the on-board 3.3v boost regulator is enabled
        ///
        virtual bool get_3v3BoostRequest() const = 0;

        ///
        /// \brief return \c true if there's control for an on-board 3.3v boost regulator
        ///
        virtual bool has_3v3BoostControl() const = 0;

        ///     \}

        //********************************************************************
        ///
        /// \name Screw Terminals
        ///
        /// \details
        ///     Most Catenas have one or more 4-pin screw
        ///     terminals. These APIs provide generic access for prompting
        ///     and for writing portable code.
        ///
        //********************************************************************

        ///     \{

        ///
        /// \brief query whether device has screw-terminal capability
        ///
        /// \return 0 if no screw terminals; otherwise number of 4-pin screw terminals
        ///     in standard Catena configuration.
        ///
        virtual uint8_t has_screwTerminals() const = 0;

        ///
        /// \brief get label of screw terminal
        ///
        /// \return nullptr if iTerminal is out of range, otherwise label (like "JP4").
        virtual const char *get_screwTerminalLabel(uint8_t iTerminal) const = 0;

        ///
        /// \brief get Arduino pin for screw terminal
        ///
        virtual uint8_t get_screwTerminalPin2(uint8_t iTerminal) const = 0;

        ///
        /// \brief get Arduino pin for screw terminal
        ///
        virtual uint8_t get_screwTerminalPin3(uint8_t iTerminal) const = 0;

        ///
        /// \brief enable power for screw terminal
        ///
        /// \return delay time in millis() until power will be ready.
        ///
        virtual uint32_t enable_screwTerminalVdd(uint8_t iTerminal, bool fState) = 0;

        ///
        /// \brief query logical power enable for screw terminal
        ///
        /// \return requested state
        ///
        virtual bool get_screwTerminalVddRequest(uint8_t iTerminal) = 0;

        ///
        /// \brief query whether there's power control for screw terminal.
        ///
        virtual bool has_screwTerminalVddControl(uint8_t iTerminal) = 0;

        ///     \}

        //********************************************************************
        ///
        /// \name Internal I2C
        ///
        /// \details
        ///     Most Catenas have an internal I2C bus; some have power
        ///     control for those busses. These APIs control the bus.
        ///
        //********************************************************************

        ///     \{

        ///
        /// \brief enable/disable power for I2C bus and associated peripherals
        ///
        ///     Since the I2C bus has pullups, the entire bus has to be powered up or
        ///     down as a unit. If the pullups are not switchable on the target board,
        ///     the I2C should be brought up and left up during begin(), and this function
        ///     should be a no-op.
        ///
        ///     The implementation should allow for multiple requests for the same power
        ///     supply, because typical boards share this supply with the flash chip.
        ///
        /// \return delay time until power is ready.
        ///
        virtual uint32_t enable_i2cVdd(bool) = 0;

        ///
        /// \brief query whether there's power control for i2c
        ///
        virtual bool has_i2cVddControl() const = 0;

        ///
        /// \brief query whether i2c Vdd is currently requested
        ///
        virtual bool get_i2cVddRequest() const = 0;

        ///     \}

        // ********************************************************************
        ///
        /// \name Flash
        ///
        /// \details
        ///     Many Catenas have an onboard flash. These APIs
        ///     control the power and abstract access.
        ///
        // ********************************************************************

        ///     \{

        virtual bool has_flash() const = 0;

        virtual cFlash *get_flash() const = 0;

        virtual SPIClass *get_flashBus() const = 0;

        virtual uint32_t enable_flashVdd(bool) = 0;
        virtual bool has_flashVddControl() const = 0;
        virtual bool get_flashVddRequest() const = 0;

        /// \brief determine whether flash was successfully probed
        bool get_flashFound() const
                {
                return this->m_flashFound;
                }

        /// \brief record whether flash was successfully probed.
        void set_flashFound(bool fFound)
                {
                this->m_flashFound = fFound;
                }

        ///     \}

        //********************************************************************
        ///
        /// \name FRAM
        ///
        /// \details
        ///     Most Catenas have an onboard FRAM. These APIs
        ///     control the power and abstract access.
        ///
        //********************************************************************

        ///     \{
        virtual bool has_fram() const = 0;
        virtual uint32_t get_framSize() const = 0;

        virtual ::TwoWire *get_framBus() const = 0;

        virtual uint32_t enable_framVdd(bool) = 0;
        virtual bool has_framVddControl() const = 0;
        virtual bool get_framVddRequest() const = 0;

        ///     \}

        //********************************************************************
        ///
        /// \name External I2C
        ///
        /// \details
        ///     Catenas separate the external I2C from
        ///     the internal I2C via a powered mux. These APIs control the
        ///     external I2C, if it's present.
        ///
        //********************************************************************

        ///     \{

        /// \brief query whether there's an external i2c
        virtual bool has_externalI2cBus() const
                { return false; }

        ///
        /// \brief turn on power and access to to the external I2C
        ///
        /// \return number of millis() to wait for bus to be ready.
        ///
        virtual uint32_t enable_externalI2cBridgeVdd(bool fStatus)
                { return 0; }

        ///
        /// \brief query state of request for external I2C bus.
        ///
        virtual bool get_externalI2cBridgeVddRequest() const
                { return false; }

        /// \brief query whether we can control the external I2C bus.
        virtual bool has_externalI2cBridgeVddControl() const
                { return false; }

        ///     \}

        //********************************************************************
        ///
        /// \name TCXO
        ///
        /// \details
        ///     Some LPWAN Catenas have a TCXO (temperature-controlled crystal
        ///     oscillator). These APIs give
        ///     access. By default, the TCXO is not present and these APIs
        ///     do nothing.
        ///
        //********************************************************************

        ///     \{
        /// \brief power up the TCXO; return number of milliseconds to delay.
        virtual uint32_t enable_tcxo(bool fStatus)
                { return 0; }

        ///
        /// \brief query TCXO request state
        virtual bool get_tcxoRequest() const
                { return false; }

        /// \brief query whether TCXO control is part of the design
        virtual bool has_tcxoControl() const
                { return false; }

        ///     \}

        //********************************************************************
        ///
        /// \name On-board Sensors
        ///
        /// \details
        ///     Catenas have a variety of integrated sensors.
        ///     These APIs replace the platform flags used in order versions
        ///     of this library
        ///
        //********************************************************************

        ///     \{

        /// \brief query whether there's a BME280
        virtual bool has_BME280() const
                { return false; }

        /// \brief query whether there's a BME680
        virtual bool has_BME680() const
                { return false; }

        /// \brief query whether there's an HS3001
        virtual bool has_HS3001() const
                { return false; }

        /// \name SHT3x
        /// \details These APIs relate to control of the SHT31/SHT35
        ///             \{

        /// \brief query whether there's an SHT31/35
        virtual bool has_SHT3x() const
                { return false; }

        /// \brief enable power to the SHT31/35
        virtual uint32_t enable_SHT3x(bool fStatus)
                { return 0; }

        /// \brief query whether the SHT31/35 power is requested
        virtual bool get_SHT3xRequest() const
                { return false; }

        /// \brief query whether the SHT31/35 power can be controlled
        virtual bool has_SHT3xPowerControl() const
                { return false; }

        ///             \}      SHT3x

        /// \name SGPC3
        /// \details These APIs relate to control of the SGPC3
        ///             \{

        /// \brief query whether there's an SGPC3
        virtual bool has_SGPC3() const
                { return false; }

        /// \brief enable power to the SHT31/35
        virtual uint32_t enable_SGPC3(bool)
                { return 0; }

        /// \brief query whether the SHT31/35 power is requested
        virtual bool get_SGPC3Request() const
                { return false; }

        /// \brief query whether the SHT31/35 power can be controlled
        virtual bool has_SGPC3PowerControl() const
                { return false; }

        ///             \}      SGPC3

        /// \name PMS7003
        /// \details These APIs relate to control of the PMS7003
        ///             \{

        /// \brief query whether there's the interface for a PMS7003
        virtual bool has_PMS7003() const
                { return false; }

        /// \brief enable power to the PMS7003
        ///
        /// \return delay (in milllis) before proceeding.
        ///
        virtual uint32_t enable_PMS7003(bool)
                { return 0; }

        /// \brief query whether the PMS7003 power is requested
        virtual bool get_PMS7003Request() const
                { return false; }

        virtual bool has_PMS7003Control() const
                { return false; }

        ///             \} PMS7003

        /// \name Light Sensors
        ///
        /// \details
        ///     These APIs relate to light sensors.
        ///             \{
        /// \brief query whether there's an Si1133 light sensor
        virtual bool has_Si1133() const
                { return false; }
        /// \brief query whether design includes a BH1750 light sensor
        virtual bool has_BH1750() const
                { return false; }

        ///             \} light
        ///     \} sensors

        //********************************************************************
        ///
        /// \name USB
        ///
        /// \details
        ///     Catenas can have USB device ports. These APIs
        ///     query the configuration.
        ///
        //********************************************************************

        ///     \{

        ///
        /// \brief query whether platform has USB port. Note that USB might
        ///     not be configured in this build.
        ///
        virtual bool has_usbPort() const
                { return false; }

        ///
        /// \brief query configuration to see whether the console is configured
        ///     to use the USB port
        ///
        virtual bool get_consoleIsUsb() const
                { return false; }

        ///
        /// \brief query whether Vbus can be measured.
        ///
        ///     Catenas designed prior to 2018 or so didn't consistently make
        ///     Vbus measurable. This API returns whether the measurement is a
        ///     standard feature.
        ///
        virtual bool has_usbVbusMeasurementStandard() const
                { return false; }

        ///
        /// \brief query the Vbus measurement pin.
        ///
        ///     Even if Vbus measurement is not standard, Catenas with USB
        ///     often have pre-assigned pins where Vbus would be measured if
        ///     the appropriate resistor is stuffed. This API returns that pin,
        ///     or zero.
        ///
        virtual uint8_t get_usbVbusMeasurementPin() const
                { return 0; }

        ///     \} usb

        /// \}  setup
        ///
        /// \brief return a pointer to string containing the name of this Catena board
        ///
        virtual const char *CatenaName(void) const = 0; // requires that an override be provided.

        ///
        /// \brief put the system to sleep for a specified number of seconds.
        ///
        /// \details This sleeps the system for a given number of seconds, but it
        ///     doesn't of itself stop peripherals or make the system ready to sleep.
        ///     That's unfortunately very situational.
        ///
        virtual void Sleep(uint32_t howLongInSeconds) = 0; // require a concrete method

        ///
        /// \brief return a pointer to the FRAM object.
        ///
        /// \details The base class returns \c nullptr; it is expected that boards
        /// that provide an FRAM will provide an override that returns the actual
        /// pointer.
        ///
        virtual McciCatena::cFram *getFram(void)
                {
                return nullptr;
                }

        /// \brief poll all the pollable objects linked to the Catena object.
        void poll(void);

        /// \brief register a pollable object to be polled by this Catena object.
        void registerObject(McciCatena::cPollableObject *pObject);

        /// \brief register a command dispatch table with the command processor.
        void addCommands(McciCatena::cCommandStream::cDispatch &, void *);

        ///
        /// \brief get the LoRaWAN provisioning style
        ///
        /// \return \c Arduino_LoRaWAN::ProvisioningStyle::kNone if LoRaWAN
        ///     is not supported.
        ///
        /// \todo (tmm@mcci.com) this should be virtual.
        ///
        Arduino_LoRaWAN::ProvisioningStyle GetProvisioningStyle(void);

        ///
        /// \brief get the LoRaWAN ABP provisioning info
        ///
        /// \return \c false if LoRaWAN is not supported.
        ///
        /// \todo (tmm@mcci.com) this should be virtual.
        ///
        bool GetAbpProvisioningInfo(Arduino_LoRaWAN::AbpProvisioningInfo *);

        ///
        /// \brief get the LoRaWAN OTAA provisioning info
        ///
        /// \return \c false if LoRaWAN is not supported.
        ///
        /// \todo (tmm@mcci.com) this should be virtual.
        ///
        bool GetOtaaProvisioningInfo(Arduino_LoRaWAN::OtaaProvisioningInfo *);

        ///
        /// \brief save LoRaWAN session info
        ///
        /// \todo (tmm@mcci.com) this should be virtual.
        ///
        void NetSaveSessionInfo(
                const Arduino_LoRaWAN::SessionInfo &Info,
                const uint8_t *pExtraInfo,
                size_t nExtraInfo
                );

        ///
        /// \brief save LoRaWAN session state
        ///
        /// \todo (tmm@mcci.com) this should be virtual.
        ///
        void NetSaveSessionState(
                const Arduino_LoRaWAN::SessionState &State
                );

        ///
        /// \brief get LoRaWAN session state
        ///
        /// \return \c false if LoRaWAN is not supported, or if
        ///     session state could not be found.
        ///
        /// \todo (tmm@mcci.com) this should be virtual.
        ///
        bool NetGetSessionState(
                Arduino_LoRaWAN::SessionState &State
                );

        ///
        /// \brief add the well known LoRaWAN commands to the command table.
        ///
        bool addLoRaWanCommands(void);

        /// \brief calibrate the system clock, if possible
        virtual uint32_t CalibrateSystemClock(void)
                {
                // if it's not possible; do nothing and return 0.
                return 0;
                }

        /// \brief return the command processor object
        cCommandStream *getCommandStream()
                {
                return &this->m_CommandStream;
                }

        /// \brief return the command line collector
        cStreamLineCollector *getCommandCollector()
                {
                return &this->m_Collector;
                }

        ///
        /// \brief the global CatenaBase pointer.
        ///
        /// Code can't readily reference a global (like "gCatena") that is of
        /// a more specialized type than the one they know. So instead, we have
        /// a global pointer to the Catena, taken as a pointer to the most
        /// general object.  It's initialzed by the constructor.
        ///
        static CatenaBase *pCatenaBase;

        /// \brief get the application version
        Version_t getAppVersion() const
                {
                return this->m_appVersion;
                }

        ///
        /// \brief delay n millis while polling
        ///
        void delay(uint32_t nMillis)
                {
                auto const tStart = millis();

                while (millis() - tStart < nMillis)
                        this->poll();
                }

protected:
        /// \brief set the application version
        void setAppVersion(Version_t v)
                {
                this->m_appVersion = v;
                }

        /// \brief register the well-known system commands
        virtual void registerCommands(void);

        /// subclasses override this method for getting the platform table
        virtual void getPlatformTable(
                const CATENA_PLATFORM * const * &vPlatforms,
                size_t &nvPlatforms
                )
                {
                vPlatforms = nullptr;
                nvPlatforms = 0;
                }

        /// subclasses override this method for getting the CPU ID platform table
        virtual void getCpuIdPlatformTable(
                const CPUID_PLATFORM_MAP * &vCpuIdToPlatform,
                size_t &nvCpuIdToPlatform
                )
                {
                vCpuIdToPlatform = nullptr;
                nvCpuIdToPlatform = 0;
                }

        ///
        /// \brief Helper class for the command-processing framework.
        ///
        /// \details The \c cStreamLineCollector class requires a instance of
        ///     a concrete class derived from \c cStreamLineCollector::cStreamReady
        ///     This class allows the stream line collector to see if there really
        ///     is an input device connected to the stream.
        ///
        class cSerialReady : public McciCatena::cStreamLineCollector::cStreamReady
                {
        public:
                /// \brief return true if Serial is ready. Overridden because
                /// the Arduino !!Serial() delays 10ms unconditionally!
                /// so we need special business.
                virtual bool isReady() const override;
                };

        /// \brief the callback object to use with the default command stream.
        cSerialReady    m_SerialReady;

        // data objects
        EUI64_buffer_t m_SysEUI;                        ///< the SysEUI we're using.
        McciCatena::cPollingEngine m_PollingEngine;     ///< the object polling engine.

        /// \brief the line collector
        McciCatena::cStreamLineCollector        m_Collector;

        /// \brief the command processor
        McciCatena::cCommandStream              m_CommandStream;

        bool                    m_flashFound = false;   ///< set true if flash was probed.

private:
        uint32_t                m_OperatingFlags;       ///< the operating flags
        const CATENA_PLATFORM * m_pPlatform;            ///< the platform pointer
        Version_t               m_appVersion {0};       ///< the application version

        // internal methods

        /// \brief save the platform once it's determined.
        void savePlatform(
                const CATENA_PLATFORM &Platform,
                const EUI64_buffer_t *pSysEUI,
                const uint32_t *pOperatingFlags
                );

        /// \brief search for a platform based on the CPU ID -- for short production
        /// run devices where we can make one image with all the CPU IDs in advance.
        const CATENA_PLATFORM *getPlatformForCpuId(
                const UniqueID_buffer_t *pIdBuffer,
                EUI64_buffer_t *pSysEUI,
                uint32_t *pOperatingFlags
                );
        };

///
/// \brief The platform description object represents common info about any Catena variant
///
/// \details
///     Every Catena model is represented by a CATENA_PLATFORM instance.
///     This instance respresents common information about all Catenas of
///     that kind.
///
///     The platforms are organized as a tree; each node has a pointer to
///     a parent node which is a more general version of the same platform.
///

struct CATENA_PLATFORM
        {
        MCCIADK_GUID_WIRE       Guid;                   ///< the platform GUID
        const CATENA_PLATFORM   *pParent;               ///< the parent (more general) platform, or NULL if
                                                        ///  this is the root for this family of models.
        uint32_t                PlatformFlags;          ///< the flags representing the capabilities of this platform,
                                                        ///  or'ed together from CatenaBase::PLATFORM_FLAGS.
        uint32_t                OperatingFlags;         ///< the default operating flags for this platform.
        };

// implementation of GetPlatformFlags -- here so that it's after the declaration of CATENA_PLATFORM.
inline uint32_t CatenaBase::GetPlatformFlags(void)
        {
        const CATENA_PLATFORM * const pPlatform = this->m_pPlatform;

        if (pPlatform != nullptr)
                return pPlatform->PlatformFlags;
        else
                return 0;
        }

///
/// \brief Simple structure to map a CPU ID to a Platform code.
///
/// \details
///     Many Catenas don't have NVRAM, and need to have information about
///     how the SAMD21 CPU is wired up and connected to the world. However,
///     all SAMD21s have a unique CPU ID. We take advantage of this, and the
///     relatively large Flash memory of the SAMD21, to map the unique CPU ID
///     onto a platform pointer. Unfortunately this requires manually adding
///     CPU IDs to the flash table, so this is only suitable for small volume;
///     but it works well if the number of CPUs is less than a hundred or so.
///
///     The file gk_WellKnownCpuBindings.cpp instantiates an array,
///     `gk_WellKnownCpuBindings[]`, containing CPU ID => platform mappings.
///
///     For convenience, we also allow you to override operating flags on
///     a CPU-ID-by-CPU-ID basis.
///
///     Catenas with NVRAM use a similar concept, but the data in the NVRAM
///     provides a platform GUID, and the SysEUI, rather than mapping the
///     CPUID to a platform pointer. The platform GUID is used to find
///     a CATENA_PLATFORM, and the SysEUI provides the identity.
///
/// \note \parblock
///     The following bash macro is generally used to generate the first
///     two lines of initiailization given the output from an MCCI test
///     program:
///
///     \code{.sh}
///       function _cpuid {
///               echo "$1" |
///               sed -e 's/^/0x/' -e 's/-/, 0x/g' |
///               awk '{
///                       $1 = "        { CpuID:  { " $1;
///                       $8 = $8 "\n\t\t   ";
///                       $16 = $16 " },";
///                       print }' ;
///       }
///     \endcode
///
///     With this macro in scope, you can write (e.g.):
///
///     \code{.sh}
///       _cpuid 05-25-dc-ef-54-53-4b-50-4a-31-2e-39-36-1a-07-ff
///     \endcode
///
///     and you'll get the first two lines of a suitable table entry.
/// \endparblock
///
/// \see ::gk_WellKnownCpuBindings[], CATENA_PLATFORM
///

struct CatenaBase::CPUID_PLATFORM_MAP
        {
        CatenaBase::UniqueID_buffer_t   CpuID;                  ///< the CPU ID to match

        const CATENA_PLATFORM           *pPlatform;             ///< the platform to use
        CatenaBase::EUI64_buffer_t      SysEUI;                 ///< the SysEUI to use
        uint32_t                        OperatingFlagsClear;    ///< mask of operating flags to be cleared
        uint32_t                        OperatingFlagsSet;      ///< mask of operating flags to be set
        };

}; // end namespace McciCatena


/**** end of CatenaBase.h ****/
#endif /* _CATENABASE_H_ */
