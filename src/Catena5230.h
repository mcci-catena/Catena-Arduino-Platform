/*

Module:  Catena5230.h

Function:
        class Catena5230: CatenaBase Platform to represent a Catena 5230
        (such as the 5230).

Copyright notice:
        See accompanying LICENSE file.

Author:
        Murali, MCCI Corporation	Sep 2025

*/

#ifndef _Catena5230_H_	/* prevent multiple includes */
#define _Catena5230_H_

#pragma once

#ifndef _CATENA523X_H_
# include "Catena523x.h"
#endif

namespace McciCatena {

class Catena5230 : public Catena523x
	{
public:
        using Super = Catena523x;

        // no specific constructor.
        Catena5230() {};

	// uses default destructor

	// neither copyable nor movable
	Catena5230(const Catena5230&) = delete;
	Catena5230& operator=(const Catena5230&) = delete;
	Catena5230(const Catena5230&&) = delete;
	Catena5230& operator=(const Catena5230&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 5230"; };
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

/**** end of Catena5230.h ****/
#endif /* _Catena5230_H_ */
