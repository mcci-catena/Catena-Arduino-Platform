/* CatenaBase_GetPlatformForID.cpp	Mon Dec 03 2018 16:43:25 chwon */

/*

Module:  CatenaBase_GetPlatformForID.cpp

Function:
	CatenaBase::GetPlatformForID()

Version:
	V0.12.0	Mon Dec 03 2018 16:43:25 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	December 2018

Revision history:
   0.12.0  Mon Dec 03 2018 16:43:25  chwon
	Module created.

*/

#include "CatenaBase.h"

#include "Catena_Log.h"
#include "Catena_Platforms.h"

using namespace McciCatena;

/*

Name:	CatenaBase::GetPlatformForID()

Function:
	Get a platform given a CPU ID and FRAM.

Definition:
	public: virtual
		const CATENA_PLATFORM *
			CatenaBase::GetPlatformForID(
				const UniqueID_buffer_t *pIdBuffer,
				EUI64_buffer_t *pSysEui,
				uint32_t *pOperatingFlags
				) override;

Description:
	This override for GetPlatformForID() looks at the FRAM, and
	picks any information availble there. If no FRAM or none is available,
	it scans by CPU ID.

	Because this implementation needs the FRAM system to be available,
	and it's called early, the Catena begin routine has to initialize
	the FRAM handler before doing the adaptive portion of initialization.

Returns:
	Pointer to platform, or NULL. *pSysEUI is alway set to some value,
	possibly zero if the routine fails. If non-NULL, pOperatingFlags is
	set to the operating flags, possibly zero if the routine fails.

*/

const CATENA_PLATFORM *
CatenaBase::GetPlatformForID(
	const UniqueID_buffer_t *pIdBuffer,
	EUI64_buffer_t *pSysEui,
	uint32_t *pOperatingFlags
	)
	{
	/* we ignore the CPUID unless we can't get a GUID */
	MCCIADK_GUID_WIRE PlatformGuid;
	auto const pFram = this->getFram();

	gLog.printf(gLog.kTrace, "+CatenaBase::GetPlatformForID:\n");

	if (pFram == nullptr)
		{
		return this->getPlatformForCpuId(
				pIdBuffer,
				pSysEui,
				pOperatingFlags
				);
		}

	// set up the SysEUI
	if (!pFram->getField(
		cFramStorage::StandardKeys::kSysEUI,
		*pSysEui
		))
		{
		gLog.printf(
			gLog.kError,
			"Didn't find SysEUI entry; zero local copy\n"
			);
		memset(pSysEui->b, 0, sizeof(pSysEui->b));
		}

	if (!pFram->getField(
			cFramStorage::StandardKeys::kPlatformGuid,
			PlatformGuid
			))
		{
		const CATENA_PLATFORM *pRomPlatform;
		uint32_t OperatingFlags;

		gLog.printf(
			gLog.kError,
			"Didn't find FRAM entry; search well-known\n"
			);

		pRomPlatform = this->getPlatformForCpuId(
				pIdBuffer,
				pSysEui,
				&OperatingFlags
				);

		if (pRomPlatform != nullptr)
			{
			this->savePlatform(
				*pRomPlatform,
				pSysEui,
				&OperatingFlags
				);
			}

		if (pOperatingFlags)
			*pOperatingFlags = OperatingFlags;

		return pRomPlatform;
		}

	// set up the operating flags
	uint32_t SavedFlags;
	bool fHaveFlags = pFram->getField(
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
				pFram->saveField(
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
CatenaBase::savePlatform(
	const CATENA_PLATFORM &Platform,
	const EUI64_buffer_t *pSysEUI,
	const uint32_t *pOperatingFlags
	)
	{
	auto const pFram = this->getFram();

	if (pFram != nullptr)
		{
		pFram->saveField(
			cFramStorage::StandardKeys::kPlatformGuid,
			Platform.Guid
			);

		if (pSysEUI != nullptr)
			{
			pFram->saveField(
				cFramStorage::StandardKeys::kSysEUI,
				*pSysEUI
				);
			}
		if (pOperatingFlags != nullptr)
			{
			pFram->saveField(
				cFramStorage::StandardKeys::kOperatingFlags,
				*pOperatingFlags
				);
			}
		}
	}

const CATENA_PLATFORM *
CatenaBase::getPlatformForCpuId(
	const UniqueID_buffer_t *pIdBuffer,
	EUI64_buffer_t *pSysEui,
	uint32_t *pOperatingFlags
	)
	{
	gLog.printf(gLog.kTrace, "CatenaBase::GetPlatformForID\n");

	// search for a matching CPU ID
	const CPUID_PLATFORM_MAP * vCpuIdToPlatform;
	size_t nvCpuIdToPlatform;

	this->getCpuIdPlatformTable(vCpuIdToPlatform, nvCpuIdToPlatform);
	const CPUID_PLATFORM_MAP *pMap;

	for (size_t i = 0; i < nvCpuIdToPlatform; ++i)
		{
		pMap = &vCpuIdToPlatform[i];

		if (memcmp(pMap->CpuID.b, pIdBuffer->b,
			   sizeof(pMap->CpuID.b)) == 0)
			{
			const CATENA_PLATFORM *pPlatform;

			memcpy(pSysEui->b, pMap->SysEUI.b,
				sizeof(pMap->SysEUI.b));

			pPlatform = pMap->pPlatform;
			if (pOperatingFlags)
				{
				uint32_t Flags;

				Flags = pPlatform->OperatingFlags;
				Flags &= ~pMap->OperatingFlagsClear;
				Flags |= pMap->OperatingFlagsSet;

				*pOperatingFlags = Flags;
				}

			return pPlatform;
			}
		}

	memset(pSysEui->b, 0, sizeof(pSysEui->b));
	if (pOperatingFlags)
		*pOperatingFlags = 0;
	return (CATENA_PLATFORM *) NULL;
	}

/**** end of CatenaBase_GetPlatformForID.cpp ****/
