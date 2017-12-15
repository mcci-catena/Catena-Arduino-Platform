/* Catena4551.h	Wed Dec 13 2017 17:29:35 tmm */

/*

Module:  Catena4551.h

Function:
	class Catena4551: CatenaBase Platform to represent a Catena 4551

Version:
	V0.7.0	Wed Dec 13 2017 17:29:35 tmm	Edit level 2

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	ChaeHee Won, MCCI Corporation	October 2017

Revision history:
   0.6.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

   0.7.0  Wed Dec 13 2017 17:29:35  tmm
	Refactor.

*/

#ifndef _CATENA4551_H_		/* prevent multiple includes */
#define _CATENA4551_H_

#pragma once

#ifndef _CATENA455X_H_
# include "Catena455x.h"
#endif

namespace McciCatena {

class Catena4551 : public Catena455x
	{
public:
        using Super = Catena455x;

        // no specific constructor.
        Catena4551() {};

	// uses default destructor

	// neither copyable nor movable
	Catena4551(const Catena4551&) = delete;
	Catena4551& operator=(const Catena4551&) = delete;
	Catena4551(const Catena4551&&) = delete;
	Catena4551& operator=(const Catena4551&&) = delete;

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena4551.h ****/
#endif /* _CATENA4551_H_ */
