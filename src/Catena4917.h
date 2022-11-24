/*

Module:  Catena4917.h

Function:
        class Catena4917: CatenaBase Platform to represent a Catena 4917

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau R, MCCI Corporation	November 2022

*/

#ifndef _Catena4917_H_	/* prevent multiple includes */
#define _Catena4917_H_

#pragma once

#ifndef _CATENA491x_H_
# include "Catena491x.h"
#endif

namespace McciCatena {

class Catena4917 : public Catena491x
	{
public:
        using Super = Catena4917;

        // no specific constructor.
        Catena4917() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4917(const Catena4917&) = delete;
	Catena4917& operator=(const Catena4917&) = delete;
	Catena4917(const Catena4917&&) = delete;
	Catena4917& operator=(const Catena4917&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4917"; };
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

/**** end of Catena4917.h ****/
#endif /* _Catena4917_H_ */