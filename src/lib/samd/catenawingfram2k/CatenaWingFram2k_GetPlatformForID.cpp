/* CatenaWingFram2k_GetPlatformForID.cpp	Wed Apr 12 2017 13:26:14 tmm */

/*

Module:  CatenaWingFram2k_GetPlatformForID.cpp

Function:
	CatenaWingFram2k::GetPlatformForID()

Version:
	V0.5.0	Wed Apr 12 2017 13:26:14 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	April 2017

Revision history:
   0.5.0  Wed Apr 12 2017 13:26:14  tmm
	Module created.

*/

#ifdef ARDUINO_ARCH_SAMD

#include "CatenaWingFram2k.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"

using namespace McciCatena;

/*

Name:	CatenaWingFram2k::GetPlatformForID()

Function:
	Get a platform given a CPU ID and FRAM.

Definition:
	public: virtual 
		const CATENA_PLATFORM * 
			CatenaWingFram2k::GetPlatformForID(
				const UniqueID_buffer_t *pIdBuffer,
				EUI64_buffer_t *pSysEui,
				uint32_t *pOperatingFlags
				) override;

Description:
	This override for GetPlatformForID() looks at the FRAM, and
	picks any information availble there. If none is available, it falls
	back to the generic SAM21D method, which scans by CPU ID.

        Because this implementation needs the FRAM system to be available,
        and it's called early, the Catena begin routine has to initialize
        the FRAM handler before doing the adaptive portion of initialization.

Returns:
	Pointer to platform, or NULL. *pSysEUI is alway set to some value,
	possibly zero if the routine fails. If non-NULL, pOperatingFlags is
	set to the operating flags, possibly zero if the routine fails.

*/

/* public virtual override */
const CATENA_PLATFORM * 
CatenaWingFram2k::GetPlatformForID(
	const UniqueID_buffer_t *pIdBuffer,
	EUI64_buffer_t *pSysEui,
	uint32_t *pOperatingFlags
	)
	{
	/* we ignore the CPUID unless we can't get a GUID */
	MCCIADK_GUID_WIRE PlatformGuid;

        gLog.printf(gLog.kAlways, "CatenaWingFram2k::GetPlatformForID entered\n");

	// set up the SysEUI
	if (!this->m_Fram.getField(
		cFramStorage::StandardKeys::kSysEUI,
		*pSysEui
		))
		{
                gLog.printf(cLog::DebugFlags(gLog.kInfo | gLog.kError), "Didn't find SysEUI entry; zero local copy\n");
		memset(pSysEui->b, 0, sizeof(pSysEui->b));
		}

	if (!this->m_Fram.getField(cFramStorage::StandardKeys::kPlatformGuid,
			PlatformGuid
			))
		{
		const CATENA_PLATFORM *pRomPlatform;
		uint32_t OperatingFlags;

                gLog.printf(cLog::DebugFlags(gLog.kInfo | gLog.kError), "Didn't find FRAM entry; search well-known\n");

		pRomPlatform = this->Super::GetPlatformForID(
			pIdBuffer, pSysEui, &OperatingFlags
			);

		if (pRomPlatform != nullptr)
			{
			this->savePlatform(*pRomPlatform, pSysEui, &OperatingFlags);
			}

		if (pOperatingFlags)
			*pOperatingFlags = OperatingFlags;

		return pRomPlatform;
		}

	// set up the operating flags
	uint32_t SavedFlags;
	bool fHaveFlags = this->m_Fram.getField(
				cFramStorage::StandardKeys::kOperatingFlags,
				SavedFlags
				);
	if (! fHaveFlags)
		{
		SavedFlags = 0;
		}

	// search for a matching GUID
        const CATENA_PLATFORM * const * vPlatforms;
        size_t nvPlatforms;

        this->getPlatformTable(vPlatforms, nvPlatforms);
        const CATENA_PLATFORM *pPlatform;

	for (size_t i = 0; i < nvPlatforms; ++i)
		{
                pPlatform = vPlatforms[i];

		if (memcmp(&vPlatforms[i]->Guid, &PlatformGuid,
			   sizeof(PlatformGuid)) == 0)
			{
			uint32_t Flags;

			Flags = pPlatform->OperatingFlags;

			// try to get the operating flags. If not found,
			// use the value from *pPlatform as the default, and
			// intialize the FRAM.
			if (! fHaveFlags)
				{
                                this->m_Fram.saveField(
					cFramStorage::StandardKeys::kOperatingFlags,
					Flags
					);
				SavedFlags = Flags;
				}

			// give the operating flags back to the client.
			if (pOperatingFlags != nullptr)
				*pOperatingFlags = SavedFlags;

			return pPlatform;
			}
		}

	/* 
	|| no platforms matched... set things up in a default-ish way.
	*/
	pPlatform = vPlatforms[0];
	if (! fHaveFlags)
		SavedFlags = pPlatform->OperatingFlags;

	this->savePlatform(*pPlatform, nullptr, &SavedFlags);

	return nullptr;
	}

void
CatenaWingFram2k::savePlatform(
	const CATENA_PLATFORM &Platform,
	const EUI64_buffer_t *pSysEUI,
	const uint32_t *pOperatingFlags
	)
	{
        this->m_Fram.saveField(
		cFramStorage::StandardKeys::kPlatformGuid,
		Platform.Guid
		);

	if (pSysEUI != nullptr)
		{
                this->m_Fram.saveField(
			cFramStorage::StandardKeys::kSysEUI,
			*pSysEUI
			);
		}
	if (pOperatingFlags != nullptr)
		{
                this->m_Fram.saveField(
			cFramStorage::StandardKeys::kOperatingFlags,
			*pOperatingFlags
			);
		}
	}

#endif // ARDUINO_ARCH_SAMD
