/* Catena4450.h	Sun Mar 19 2017 15:50:15 tmm */

/*

Module:  Catena4450.h

Function:
	class Catena4450: CatenaBase Platform to represent a Catena 4450

Version:
	V0.5.0	Sun Mar 19 2017 15:50:15 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2017 by

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

protected:
        // we are required to provide a table of platforms
        virtual void getPlatformTable(
                const CATENA_PLATFORM * const * &vPlatforms,
                size_t &nvPlatforms
                ) override;

private:
        // the known platforms
        static const CATENA_PLATFORM (* const vPlatforms[]);
        static const size_t nvPlatforms;
	};

} /* namespace McciCatena */

/**** end of Catena4450.h ****/
#endif /* _CATENA4450_H_ */
