/* Catena4610.h	Fri Dec 28 2018 13:58:16 chwon */

/*

Module:  Catena4610.h

Function:
	class Catena4610: CatenaBase Platform to represent a Catena 4610

Version:
	V0.13.0	Fri Dec 28 2018 13:58:16 chwon	Edit level 2

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

   0.13.0  Fri Dec 28 2018 13:58:16  chwon
	Add ReadVbat() and ReadVbus() override.

*/

#ifndef _Catena4610_H_	/* prevent multiple includes */
#define _Catena4610_H_

#pragma once

#ifndef _CATENA461X_H_
# include "Catena461x.h"
#endif

namespace McciCatena {

class Catena4610 : public Catena461x
	{
public:
        using Super = Catena461x;

        // no specific constructor.
        Catena4610() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4610(const Catena4610&) = delete;
	Catena4610& operator=(const Catena4610&) = delete;
	Catena4610(const Catena4610&&) = delete;
	Catena4610& operator=(const Catena4610&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4610"; };
	virtual float ReadVbat(void) const override;
	virtual float ReadVbus(void) const override;

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

/**** end of Catena4610.h ****/
#endif /* _Catena4610_H_ */
