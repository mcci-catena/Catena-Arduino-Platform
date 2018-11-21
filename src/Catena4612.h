/* Catena4612.h	Thu Nov 15 2018 15:07:23 chwon */

/*

Module:  Catena4612.h

Function:
	class Catena4612: CatenaBase Platform to represent a Catena 4612

Version:
	V0.11.0	Thu Nov 15 2018 15:07:23 chwon	Edit level 1

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

*/

#ifndef _Catena4612_H_	/* prevent multiple includes */
#define _Catena4612_H_

#pragma once

#ifndef _CATENA461X_H_
# include "Catena461x.h"
#endif

namespace McciCatena {

class Catena4612 : public Catena461x
	{
public:
        using Super = Catena461x;

        // no specific constructor.
        Catena4612() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4612(const Catena4612&) = delete;
	Catena4612& operator=(const Catena4612&) = delete;
	Catena4612(const Catena4612&&) = delete;
	Catena4612& operator=(const Catena4612&&) = delete;

	virtual const char *CatenaName() const override { return "Catena 4612"; };

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena4612.h ****/
#endif /* _Catena4612_H_ */
