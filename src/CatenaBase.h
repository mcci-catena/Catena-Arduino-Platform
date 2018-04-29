/* CatenaBase.h	Sat Apr 28 2018 16:52:31 tmm */

/*

Module:  CatenaBase.h

Function:
        class CatenaBase interfaces.

Version:
        V0.9.1	Sat Apr 28 2018 16:52:31 tmm	Edit level 4

Copyright notice:
        This file copyright (C) 2016-2018 by

                MCCI Corporation
                3520 Krums Corners Road
                Ithaca, NY  14850

        An unpublished work.  All rights reserved.
        
        This file is released under terms of the accompanying LICENSE file.

Author:
        Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Tue Oct 25 2016 01:58:25  tmm
        Module created.

   0.3.0  Thu Oct 27 2016 22:46:30  tmm
        Change buffer types away from array, types are just not intuitive.

   0.5.0  Sun Mar 19 2017 15:00:21  tmm
        Major update for comamand support, etc.

   0.9.0  Sat Mar 31 2018 19:28:30  tmm
	Add fHasBme680.

   0.9.1  Sat Apr 28 2018 16:52:31  tmm
	Add support for RS485 (for Catena 4470).

*/

#ifndef _CATENABASE_H_		/* prevent multiple includes */
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

namespace McciCatena {

/* forward references */
struct CATENA_PLATFORM;

class CatenaBase
        {
public:
        CatenaBase();
        virtual ~CatenaBase() {};

        /* an EUI64 */
        struct EUI64_buffer_t {
                uint8_t	b[64/8];
                };
        struct EUI64_string_t
                {
                char	c[sizeof(EUI64_buffer_t) * 3 + 1];
                };

        enum OPERATING_FLAGS : uint32_t
                {
                fUnattended = 1 << 0,
                fManufacturingTest = 1 << 1,
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

        virtual const EUI64_buffer_t *GetSysEUI(void)
                {
                return &this->m_SysEUI;
                }

        virtual bool begin();

        // poll the engine
        void poll(void);
        void registerObject(McciCatena::cPollableObject *pObject);

        // command handling
        void addCommands(McciCatena::cCommandStream::cDispatch &, void *);

/****************************************************************************\
|
|	The global CatenaBase pointer.
|
|	Code can't readily reference a global (like "gCatena") that is of
|	a more specialized type than the one they know. So instead, we have
|	a global pointer to the Catena, taken as a pointer to the most
|	general object.  It's initialzed by the constructor.
|
\****************************************************************************/

        static CatenaBase *pCatenaBase;

protected:
        virtual void registerCommands(void);

        // data objects
protected:
        EUI64_buffer_t m_SysEUI;
        McciCatena::cPollingEngine m_PollingEngine;

        // the line collector
        McciCatena::cStreamLineCollector	m_Collector;

        // the command processor
        McciCatena::cCommandStream              m_CommandStream;
        };


/*

Type:	CATENA_PLATFORM

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
	MCCIADK_GUID_WIRE	Guid;
	const CATENA_PLATFORM	*pParent;
	uint32_t		PlatformFlags;
	uint32_t		OperatingFlags;
	};



/*

Name:	CatenaBase::PlatformFlags_GetModNumber()

Index:	Function:	CatenaBase::PlatformFlags_IsModded();

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

// global for finding the base from anywhere.
extern CatenaBase *gpCatenaBase;

}; // end namespace McciCatena


/**** end of CatenaBase.h ****/
#endif /* _CATENABASE_H_ */
