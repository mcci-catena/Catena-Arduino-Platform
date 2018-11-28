/* Catena4611.h	Thu Nov 15 2018 15:07:23 chwon */

/*

Module:  Catena4611.h

Function:
	class Catena4611: CatenaBase Platform to represent a Catena 4611

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

#ifndef _Catena4611_H_	/* prevent multiple includes */
#define _Catena4611_H_

#pragma once

#ifndef _CATENA461X_H_
# include "Catena461x.h"
#endif

namespace McciCatena {

class Catena4611 : public Catena461x
	{
public:
        using Super = Catena461x;

        // no specific constructor.
        Catena4611() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4611(const Catena4611&) = delete;
	Catena4611& operator=(const Catena4611&) = delete;
	Catena4611(const Catena4611&&) = delete;
	Catena4611& operator=(const Catena4611&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4611"; };

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

/**** end of Catena4611.h ****/
#endif /* _Catena4611_H_ */
