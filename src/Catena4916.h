/*

Module:  Catena4916.h

Function:
        class Catena4916: CatenaBase Platform to represent a Catena 4916

Copyright notice:
        See accompanying LICENSE file.

Author:
        Dhinesh Kumar Pitchai, MCCI Corporation	November 2022

*/

#ifndef _Catena4916_H_	/* prevent multiple includes */
#define _Catena4916_H_

#pragma once

#ifndef _CATENA491x_H_
# include "Catena491x.h"
#endif

namespace McciCatena {

class Catena4916 : public Catena491x
	{
public:
        using Super = Catena4916;

        // no specific constructor.
        Catena4916() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4916(const Catena4916&) = delete;
	Catena4916& operator=(const Catena4916&) = delete;
	Catena4916(const Catena4916&&) = delete;
	Catena4916& operator=(const Catena4916&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4916"; };
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

/**** end of Catena4916.h ****/
#endif /* _Catena4916_H_ */