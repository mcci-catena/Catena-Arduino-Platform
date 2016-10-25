/* CatenaSamd21.h	Tue Oct 25 2016 02:00:28 tmm */

/*

Module:  CatenaSamd21.h

Function:
	class CatenaSamd21

Version:
	V0.1.0	Tue Oct 25 2016 02:00:28 tmm	Edit level 1

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

*/

#ifndef _CATENASAMD21_H_		/* prevent multiple includes */
#define _CATENASAMD21_H_

#ifndef _CATENABASE_H_
# include "CatenaBase.h"
#endif

#include "mcciadk_guid.h"

typedef struct CATENA_PLATFORM_s  CATENA_PLATFORM;
typedef struct CATENA_CPUID_TO_PLATFORM_s CATENA_CPUID_TO_PLATFORM;

class CatenaSamd21 : public CatenaBase
	{
public:	
	/*
	|| Types
	*/
	typedef	uint8_t	UniqueID_buffer_t[128/8];

	/* a buffer big enough to hold a stringified UniqueID_buffer_t */
	typedef char UniqueID_string_t[
			sizeof(UniqueID_buffer_t) * 3 + 1
			];

	/*
	|| Methods
	*/
	bool begin(void);
	bool begin(uint32_t uOverrideMask);
	bool begin(uint32_t uClearMask, uint32_t uSetMask);

	const CATENA_PLATFORM *GetPlatformForID(
		const UniqueID_buffer_t pIdBuffer,
		EUI64_buffer_t pSysEUI
		);

	const CATENA_PLATFORM *GetPlatformForID(
		const UniqueID_buffer_t pIdBuffer,
		EUI64_buffer_t pSysEUI,
		uint32_t *pOperatingFlags
		);

	void GetUniqueID(
		UniqueID_buffer_t pIdBuffer
		);

	char *GetUniqueIDstring(
		UniqueID_string_t pIdStringBuffer
		);

	const inline CATENA_PLATFORM *GetPlatform(void)
		{
		return this->m_pPlatform;
		}
	const inline uint32_t GetPlatformFlags(void)
		{
		return this->m_OperatingFlags;
		}

	/*
	|| Class-level information
	*/
private:
	static const CATENA_CPUID_TO_PLATFORM vCpuIdToPlatform[];
	static const size_t nvCpuIdToPlatform;
	uint32_t		m_OperatingFlags;
	const CATENA_PLATFORM	*m_pPlatform;
	};

struct CATENA_PLATFORM_s
	{
	MCCIADK_GUID		Guid;
	const CATENA_PLATFORM	*pParent;
	uint32_t		PlatformFlags;
	uint32_t		OperatingFlags;
	};

struct CATENA_CPUID_TO_PLATFORM_s
	{
	CatenaSamd21::UniqueID_buffer_t	CpuID;

	const CATENA_PLATFORM		*pPlatform;
	CatenaBase::EUI64_buffer_t	SysEUI;
	uint32_t			OperatingFlagsClear;
	uint32_t			OperatingFlagsSet;
	};

/**** end of CatenaSamd21.h ****/
#endif /* _CATENASAMD21_H_ */
