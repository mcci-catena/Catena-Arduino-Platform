/* CatenaSamd21.h	Thu Oct 27 2016 22:53:12 tmm */

/*

Module:  CatenaSamd21.h

Function:
	class CatenaSamd21

Version:
	V0.3.0	Thu Oct 27 2016 22:53:12 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Tue Oct 25 2016 02:00:28  tmm
	Module created.

   0.3.0  Thu Oct 27 2016 22:53:12  tmm
	Eliminate array types.

*/

#ifndef _CATENASAMD21_H_		/* prevent multiple includes */
#define _CATENASAMD21_H_

#ifndef _CATENABASE_H_
# include "CatenaBase.h"
#endif

#include "mcciadk_guid.h"

/* forward references */
struct CATENA_PLATFORM;
struct CATENA_CPUID_TO_PLATFORM;

/* the class for Samd21-based Catenas */
class CatenaSamd21 : public CatenaBase
	{
public:	
	/*
	|| Types
	*/
	struct	UniqueID_buffer_t
		{ 
		uint8_t b[128/8];
		};

	/* a buffer big enough to hold a stringified UniqueID_buffer_t */
	struct UniqueID_string_t
		{
		char	c[sizeof(UniqueID_buffer_t) * 3 + 1];
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
                // platform supports the Lux meter
		fHasLux = 1 << 5,
                // platform supports soil probe
		fHasSoilProbe = 1 << 6,
                // platform supports external solar panel
		fHasSolarPanel = 1 << 7,
                // platform supports one-wire temperature sensor
		fHasWaterOneWire = 1 << 8,
                // platform not only has LoRa, but it's wired per the MCCI RadioWing standard
                fHasTtnMcciLoRa = 1 << 9,
		};

	/*
	|| Methods
	*/
	bool begin(void);
	bool begin(uint32_t uOverrideMask);
	bool begin(uint32_t uClearMask, uint32_t uSetMask);

	const CATENA_PLATFORM *GetPlatformForID(
		const UniqueID_buffer_t *pIdBuffer,
		EUI64_buffer_t *pSysEUI
		);

	const CATENA_PLATFORM *GetPlatformForID(
		const UniqueID_buffer_t *pIdBuffer,
		EUI64_buffer_t *pSysEUI,
		uint32_t *pOperatingFlags
		);

	void GetUniqueID(
		UniqueID_buffer_t *pIdBuffer
		);

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
	inline uint32_t GetPlatformFlags(void);		

	/*
	|| Class-level information
	*/
private:
	static const CATENA_CPUID_TO_PLATFORM vCpuIdToPlatform[];
	static const size_t nvCpuIdToPlatform;
	uint32_t		m_OperatingFlags;
	const CATENA_PLATFORM	*m_pPlatform;
	};

struct CATENA_PLATFORM
	{
	MCCIADK_GUID		Guid;
	const CATENA_PLATFORM	*pParent;
	uint32_t		PlatformFlags;
	uint32_t		OperatingFlags;
	};

inline uint32_t CatenaSamd21::GetPlatformFlags(void)
	{
	const CATENA_PLATFORM * const pPlatform = this->m_pPlatform;

	if (pPlatform != NULL)
		return pPlatform->PlatformFlags;
	else
		return 0;
	}

struct CATENA_CPUID_TO_PLATFORM
	{
	CatenaSamd21::UniqueID_buffer_t	CpuID;

	const CATENA_PLATFORM		*pPlatform;
	CatenaBase::EUI64_buffer_t	SysEUI;
	uint32_t			OperatingFlagsClear;
	uint32_t			OperatingFlagsSet;
	};

/**** end of CatenaSamd21.h ****/
#endif /* _CATENASAMD21_H_ */
