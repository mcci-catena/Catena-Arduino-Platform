/* Catena4450.h	Wed Dec 05 2018 14:22:06 chwon */

/*

Module:  Catena4450.h

Function:
	class Catena4450: CatenaBase Platform to represent a Catena 4450

Version:
	V0.12.0	Wed Dec 05 2018 14:22:06 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2017-2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	March 2017

Revision history:
   0.5.0  Sun Mar 12 2017 18:16:07  tmm
	Module created.

   0.11.0  Wed Nov 21 2018 13:44:27  chwon
	Add CatenaName() method.

   0.12.0  Wed Dec 05 2018 14:22:06  chwon
	Add getCpuIdPlatformTable() method override.

*/

#ifndef _CATENA4450_H_		/* prevent multiple includes */
#define _CATENA4450_H_

#pragma once

#ifndef _CATENAWINGFRAM2K_H_
# include "CatenaWingFram2k.h"
#endif

namespace McciCatena {

class Catena4450 : public CatenaWingFram2k
	{
public:
        using Super = CatenaWingFram2k;

        // no specific constructor.
        Catena4450() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4450(const Catena4450&) = delete;
	Catena4450& operator=(const Catena4450&) = delete;
	Catena4450(const Catena4450&&) = delete;
	Catena4450& operator=(const Catena4450&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4450"; };

protected:
        // we are required to provide a table of platforms
        virtual void getPlatformTable(
                const CATENA_PLATFORM * const * &vPlatforms,
                size_t &nvPlatforms
                ) override;

	virtual void getCpuIdPlatformTable(
		const CPUID_PLATFORM_MAP * &vCpuIdToPlatform,
		size_t &nvCpuIdToPlatform
		) override;

private:
        // the known platforms
        static const CATENA_PLATFORM (* const vPlatforms[]);
        static const size_t nvPlatforms;

	static const CPUID_PLATFORM_MAP vCpuIdToPlatform[];
	static const size_t nvCpuIdToPlatform;
	};

} /* namespace McciCatena */

/**** end of Catena4450.h ****/
#endif /* _CATENA4450_H_ */
