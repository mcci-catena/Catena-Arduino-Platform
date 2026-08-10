/*

Module:  Catena5220.h

Function:
        class Catena5220: CatenaBase Platform to represent a Catena 5220
        (such as the 5220).

Copyright notice:
        See accompanying LICENSE file.

Author:
        Pranau Ravikumar, MCCI Corporation	August 2026

*/

#ifndef _Catena5220_H_	/* prevent multiple includes */
#define _Catena5220_H_

#pragma once

#ifndef _CATENA522x_H_
# include "Catena522x.h"
#endif

namespace McciCatena {

class Catena5220 : public Catena522x
	{
public:
        using Super = Catena522x;

        // no specific constructor.
        Catena5220() {};

	// uses default destructor

	// neither copyable nor movable
	Catena5220(const Catena5220&) = delete;
	Catena5220& operator=(const Catena5220&) = delete;
	Catena5220(const Catena5220&&) = delete;
	Catena5220& operator=(const Catena5220&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 5220"; };
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

/**** end of Catena5220.h ****/
#endif /* _Catena5220_H_ */
