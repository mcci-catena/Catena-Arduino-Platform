/* Catena4618.h	Tue Jun 18 2019 15:28:10 lakshmipriyan */

/*

Module:  Catena4618.h

Function:
	class Catena4618: CatenaBase Platform to represent a Catena 4618

Version:
	V0.11.0	Tue Jun 18 2019 15:28:10 lakshmipriyan	Edit level 1

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
   0.11.0  Tue Jun 18 2019 15:28:10 lakshmipriyan
	Module created.

*/

#ifndef _Catena4618_H_	/* prevent multiple includes */
#define _Catena4618_H_

#pragma once

#ifndef _CATENA461X_H_
# include "Catena461x.h"
#endif

namespace McciCatena {

class Catena4618 : public Catena461x
	{
public:
        using Super = Catena461x;

        // no specific constructor.
        Catena4618() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4618(const Catena4618&) = delete;
	Catena4618& operator=(const Catena4618&) = delete;
	Catena4618(const Catena4618&&) = delete;
	Catena4618& operator=(const Catena4618&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4618"; };
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

/**** end of Catena4618.h ****/
#endif /* _Catena4618_H_ */
