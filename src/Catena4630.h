/*

Module:  Catena4630.h

Function:
        class Catena4630: CatenaBase Platform to represent a Catena 4630.

Copyright notice:
        See accompanying LICENSE file.

Author:
        Dhinesh Kumar Pitchai, MCCI Corporation	April 2019

*/

#ifndef _Catena4630_H_	/* prevent multiple includes */
#define _Catena4630_H_

#pragma once

#ifndef _CATENA463X_H_
# include "Catena463x.h"
#endif

namespace McciCatena {

class Catena4630 : public Catena463x
	{
public:
        using Super = Catena463x;

        // no specific constructor.
        Catena4630() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4630(const Catena4630&) = delete;
	Catena4630& operator=(const Catena4630&) = delete;
	Catena4630(const Catena4630&&) = delete;
	Catena4630& operator=(const Catena4630&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4630"; };
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

/**** end of Catena4630.h ****/
#endif /* _Catena4630_H_ */
