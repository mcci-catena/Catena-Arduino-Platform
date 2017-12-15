/* Catena.h	Wed Dec 13 2017 17:29:35 tmm */

/*

Module:  Catena.h

Function:
	class Catena: virtual CatenaBase derivative representing
        the target Catena for this build.

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

#ifndef _CATENA_H_		/* prevent multiple includes */
#define _CATENA_H_

#pragma once

#ifdef ARDUINO_ARCH_SAMD
# include "Catena4450.h"
# define CATENA_H_SUPER_  McciCatena::Catena4450
#elif defined(ARDUINO_ARCH_STM32)
# include "Catena4551.h"
# define CATENA_H_SUPER_  McciCatena::Catena4551
#else
# define CATENA_H_SUPER_  /* nothing, for intellisense */
# error Architecture not supported
#endif

namespace McciCatena {

class Catena : public CATENA_H_SUPER_
	{
public:
        using Super = CATENA_H_SUPER_;

        // no specific constructor.
        Catena() {};

	// uses default destructor

	// neither copyable nor movable
	Catena(const Catena&) = delete;
	Catena& operator=(const Catena&) = delete;
	Catena(const Catena&&) = delete;
	Catena& operator=(const Catena&&) = delete;

protected:

private:
	};

} // namespace McciCatena

#undef CATENA_H_SUPER_

/**** end of Catena.h ****/
#endif /* _CATENA_H_ */
