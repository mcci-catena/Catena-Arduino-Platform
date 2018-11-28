/* Catena4801.h	Thu Nov 15 2018 15:07:23 chwon */

/*

Module:  Catena4801.h

Function:
	class Catena4801: CatenaBase Platform to represent a Catena 4801

Version:
	V0.11.0	Thu Nov 15 2018 15:07:23 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2018 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	November 2018

Revision history:
   0.11.0  Thu Nov 15 2018 15:07:23 chwon
	Module created.

*/

#ifndef _Catena4801_H_	/* prevent multiple includes */
#define _Catena4801_H_

#pragma once

#ifndef _CATENA480X_H_
# include "Catena480x.h"
#endif

namespace McciCatena {

class Catena4801 : public Catena480x
	{
public:
        using Super = Catena480x;

        // no specific constructor.
        Catena4801() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4801(const Catena4801&) = delete;
	Catena4801& operator=(const Catena4801&) = delete;
	Catena4801(const Catena4801&&) = delete;
	Catena4801& operator=(const Catena4801&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4801"; };

protected:
	// we are required to provide a table of platforms
	virtual void getPlatformTable(
		const CATENA_PLATFORM * const * &vPlatforms,
		size_t &nvPlatforms
		) override;

private:
	// the known platforms
	static const CATENA_PLATFORM(* const vPlatforms[]);
	static const size_t nvPlatforms;
	};

} // namespace McciCatena

/**** end of Catena4801.h ****/
#endif /* _Catena4801_H_ */
