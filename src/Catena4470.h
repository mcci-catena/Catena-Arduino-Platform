/* Catena4470.h	Wed Nov 21 2018 13:45:22 chwon */

/*

Module:  Catena4470.h

Function:
	class Catena4470: CatenaBase Platform to represent a Catena 4470

Version:
	V0.11.0	Wed Nov 21 2018 13:45:23 chwon	Edit level 2

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

   0.11.0  Wed Nov 21 2018 13:45:23  chwon
	Add CatenaName() method.

*/

#ifndef _CATENA4470_H_		/* prevent multiple includes */
#define _CATENA4470_H_

#pragma once

#ifndef _CATENAWINGFRAM2K_H_
# include "CatenaWingFram2k.h"
#endif

namespace McciCatena {

class Catena4470 : public CatenaWingFram2k
	{
public:
        using Super = CatenaWingFram2k;

        // no specific constructor.
        Catena4470() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4470(const Catena4470&) = delete;
	Catena4470& operator=(const Catena4470&) = delete;
	Catena4470(const Catena4470&&) = delete;
	Catena4470& operator=(const Catena4470&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4470"; };

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

/**** end of Catena4470.h ****/
#endif /* _CATENA4470_H_ */
