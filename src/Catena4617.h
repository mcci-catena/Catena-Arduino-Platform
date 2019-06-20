/* Catena4617.h	Tue Jun 18 2019 13:58:19 lakshmipriyan */

/*

Module:  Catena4617.h

Function:
	class Catena4617: CatenaBase Platform to represent a Catena 4617

Version:
	V0.11.0	Tue Jun 18 2019 13:58:19 lakshmipriyan	Edit level 1

Copyright notice:
	This file copyright (C) 2019 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	Lakshmi priya Natarajan, MCCI Corporation	June 2019

Revision history:
   0.11.0  Tue Jun 18 2019 13:58:19 lakshmipriyan
	Module created.

*/

#ifndef _Catena4617_H_	/* prevent multiple includes */
#define _Catena4617_H_

#pragma once

#ifndef _CATENA461X_H_
# include "Catena461x.h"
#endif

namespace McciCatena {

class Catena4617 : public Catena461x
	{
public:
        using Super = Catena461x;

        // no specific constructor.
        Catena4617() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4617(const Catena4617&) = delete;
	Catena4617& operator=(const Catena4617&) = delete;
	Catena4617(const Catena4617&&) = delete;
	Catena4617& operator=(const Catena4617&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4617"; };
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

/**** end of Catena4617.h ****/
#endif /* _Catena4617_H_ */
