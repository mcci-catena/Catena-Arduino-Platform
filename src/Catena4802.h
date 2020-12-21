/*

Module:  Catena4802.h

Function:
        class Catena4802: CatenaBase Platform to represent a Catena 4802.

Copyright notice:
        See accompanying LICENSE file.

Author:
        Dhinesh Kumar Pitchai, MCCI Corporation	November 2020

*/

#ifndef _Catena4802_H_	/* prevent multiple includes */
#define _Catena4802_H_

#pragma once

#ifndef _CATENA480X_H_
# include "Catena480x.h"
#endif

namespace McciCatena {

class Catena4802 : public Catena480x
	{
public:
        using Super = Catena480x;

        // no specific constructor.
        Catena4802() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4802(const Catena4802&) = delete;
	Catena4802& operator=(const Catena4802&) = delete;
	Catena4802(const Catena4802&&) = delete;
	Catena4802& operator=(const Catena4802&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4802"; };
	float ReadVin(void) const;

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

/**** end of Catena4802.h ****/
#endif /* _Catena4802_H_ */
