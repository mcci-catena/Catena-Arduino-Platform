/* CatenaStm32.h	Thu Dec 14 2017 14:55:06 tmm */

/*

Module:  CatenaStm32.h

Function:
	class CatenaStm32

Version:
	V0.7.0	Thu Dec 14 2017 14:55:06 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

Author:
	ChaeHee Won, MCCI Corporation	October 2017

Revision history:
   0.6.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

   0.7.0  Thu Dec 14 2017 14:55:06  tmm
	Refactor common code.

*/

#ifndef _CATENASTM32_H_		/* prevent multiple includes */
#define _CATENASTM32_H_

#pragma once

#ifndef _CATENABASE_H_
# include "CatenaBase.h"
#endif

namespace McciCatena {

/* the class for Stm32-based Catenas */
class CatenaStm32 : public CatenaBase
	{
public:
        using Super = CatenaBase;

	/*
	|| Types
        ||
        || STM32's UniqeuID is 12 bytes long.
        || Presently we use a 16-byte buffer.
        || TODO: change.
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

        /* forward references */
        struct CPUID_PLATFORM_MAP;

	/*
	|| Methods
	*/

        // start the Stm32 level
	virtual bool begin(void);
	bool begin(uint32_t uOverrideMask);
	bool begin(uint32_t uClearMask, uint32_t uSetMask);

	const CATENA_PLATFORM *GetPlatformForID(
		const UniqueID_buffer_t *pIdBuffer,
		EUI64_buffer_t *pSysEUI
		);

	virtual const CATENA_PLATFORM *GetPlatformForID(
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

protected:
	virtual void registerCommands(void);

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
        cSerialReady                            m_SerialReady;

	/*
	|| Class-level information
	*/
private:
	static const CPUID_PLATFORM_MAP vCpuIdToPlatform[];
	static const size_t nvCpuIdToPlatform;
	uint32_t		m_OperatingFlags;
	const CATENA_PLATFORM	*m_pPlatform;
	};


inline uint32_t CatenaStm32::GetPlatformFlags(void)
	{
	const CATENA_PLATFORM * const pPlatform = this->m_pPlatform;

	if (pPlatform != NULL)
		return pPlatform->PlatformFlags;
	else
		return 0;
	}

/*

Type:	CatenaStm32::CPUID_PLATFORM_MAP

Function:
	Simple structure to map a CPU ID to a Platform code.

Description:
	Many Catenas don't have NVRAM, and need to have information about
	how the STM32 CPU is wired up and connected to the world. However,
	all STM32s have a unique CPU ID. We take advantage of this, and the
	relatively large Flash memory of the STM32, to map the unique CPU ID
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
	CatenaStm32::UniqueID_buffer_t	CpuID;
		The Unique CPU ID.

	const CATENA_PLATFORM *pPlatform;
		A pointer to the platform that defines this assembly.

	CatenaBase::EUI64_buffer_t	SysEUI;
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

.	function _cpuid {
.	        echo "$1" |
.	        sed -e 's/^/0x/' -e 's/-/, 0x/g' |
.		awk '{
.	                $1 = "        { CpuID:  { " $1;
.		        $8 = $8 "\n\t\t   ";
.			$16 = $16 " },";
.	                print }' ;
.	}

	With this macro in scope, you can write (e.g.):

.	_cpuid 05-25-dc-ef-54-53-4b-50-4a-31-2e-39-36-1a-07-ff

	and you'll get the first two lines of a suitable table entry.

See Also:
	gk_WellKnownCpuBindings[], CATENA_PLATFORM

*/

struct CatenaStm32::CPUID_PLATFORM_MAP
	{
	CatenaStm32::UniqueID_buffer_t	CpuID;

	const CATENA_PLATFORM *		pPlatform;
	CatenaBase::EUI64_buffer_t	SysEUI;
	uint32_t			OperatingFlagsClear;
	uint32_t			OperatingFlagsSet;
	};

} // namespace McciCatena

/**** end of CatenaStm32.h ****/
#endif /* _CATENASTM32_H_ */
