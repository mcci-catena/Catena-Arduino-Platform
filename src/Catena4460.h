/* Catnea4460.h	Sat Mar 31 2018 19:28:30 tmm */

/*

Module:  Catena4460.h

Function:
	class Catena4460: CatenaBase Platform to represent a Catena 4460

Version:
	V0.9.0	Sat Mar 31 2018 19:28:30 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2018

Revision history:
   0.9.0  Sat Mar 31 2018 19:28:30  tmm
	Module created.

*/

#ifndef _CATENA4460_H_		/* prevent multiple includes */
#define _CATENA4460_H_

#pragma once

#ifndef _CATENAWINGFRAM2K_H_
# include "CatenaWingFram2k.h"
#endif

namespace McciCatena {

class Catena4460 : public CatenaWingFram2k
	{
public:
        using Super = CatenaWingFram2k;

        // no specific constructor.
        Catena4460() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4460(const Catena4460&) = delete;
	Catena4460& operator=(const Catena4460&) = delete;
	Catena4460(const Catena4460&&) = delete;
	Catena4460& operator=(const Catena4460&&) = delete;

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

/**** end of Catena4460.h ****/
#endif /* _CATENA4460_H_ */
