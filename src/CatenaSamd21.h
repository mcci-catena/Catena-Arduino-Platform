/* CatenaSamd21.h	Sun Mar 19 2017 15:49:23 tmm */

/*

Module:  CatenaSamd21.h

Function:
	class CatenaSamd21

Version:
	V0.5.0	Sun Mar 19 2017 15:49:23 tmm	Edit level 4

Copyright notice:
	This file copyright (C) 2016-2017 by

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

   0.4.0  Wed Mar  8 2017 17:29:54  tmm
	Add flags for new well-known sensors and mod-number provisions.

   0.5.0  Sun Mar 19 2017 15:49:23  tmm
	Refactor to add support for USB command-line processing.

*/

#ifndef _CATENASAMD21_H_		/* prevent multiple includes */
#define _CATENASAMD21_H_

#pragma once

#ifndef _CATENABASE_H_
# include "CatenaBase.h"
#endif

namespace McciCatena {

/* the class for Samd21-based Catenas */
class CatenaSamd21 : public CatenaBase
	{
public:	
        using Super = CatenaBase;

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

        /* forward references */
        struct CPUID_PLATFORM_MAP;

	/*
	|| Methods
	*/

        // start the Samd21 level
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

inline uint32_t CatenaSamd21::GetPlatformFlags(void)
	{
	const CATENA_PLATFORM * const pPlatform = this->m_pPlatform;

	if (pPlatform != NULL)
		return pPlatform->PlatformFlags;
	else
		return 0;
	}


/*

Type:	CatenaSamd21::CPUID_PLATFORM_MAP

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
	CatenaSamd21::UniqueID_buffer_t	CpuID;
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

struct CatenaSamd21::CPUID_PLATFORM_MAP
	{
	CatenaSamd21::UniqueID_buffer_t	CpuID;

	const CATENA_PLATFORM		*pPlatform;
	CatenaBase::EUI64_buffer_t	SysEUI;
	uint32_t			OperatingFlagsClear;
	uint32_t			OperatingFlagsSet;
	};

} /* namespace McciCatena */

/**** end of CatenaSamd21.h ****/
#endif /* _CATENASAMD21_H_ */
