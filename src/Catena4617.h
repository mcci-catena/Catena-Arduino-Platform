/*

Module:  Catena4617.h

Function:
        class Catena4617: CatenaBase Platform to represent a Catena 4617

Copyright notice:
        See accompanying LICENSE file.

Author:
        Lakshmi Priya Natarajan, MCCI Corporation	June 2019

Revision history:
        See https://github.com/mcci-catena/Catena-Arduino-Platform

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
