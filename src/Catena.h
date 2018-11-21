/* Catena.h	Thu Nov 15 2018 15:06:41 chwon */

/*

Module:  Catena.h

Function:
	class Catena: virtual CatenaBase derivative representing
        the target Catena for this build.

Version:
	V0.11.0	Thu Nov 15 2018 15:06:42 chwon	Edit level 4

Copyright notice:
	This file copyright (C) 2017-2018 by

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

   0.9.0  Sat Mar 31 2018 19:28:30  tmm
	Add Catena 4460 support, use BSP symbols.

   0.11.0  Thu Nov 15 2018 15:06:42  chwon
	Add Catena 461x support.

*/

#ifndef _CATENA_H_		/* prevent multiple includes */
#define _CATENA_H_

#pragma once

#if defined(MCCI_CATENA_4450)
# include "Catena4450.h"
# define CATENA_H_SUPER_  McciCatena::Catena4450
#elif defined(MCCI_CATENA_4460)
# include "Catena4460.h"
# define CATENA_H_SUPER_  McciCatena::Catena4460
#elif defined(MCCI_CATENA_4470)
# include "Catena4470.h"
# define CATENA_H_SUPER_  McciCatena::Catena4470
#elif defined(MCCI_CATENA_4410)
# include "Catena4410.h"
# define CATENA_H_SUPER_  McciCatena::Catena4410
#elif defined(MCCI_CATENA_4420)
# include "Catena4420.h"
# define CATENA_H_SUPER_  McciCatena::Catena4420
#elif defined(ARDUINO_SAMD_FEATHER_M0)
# include "CatenaFeatherM0.h"
# define CATENA_H_SUPER_  McciCatena::CatenaFeatherM0
#elif defined(ARDUINO_MCCI_CATENA_4551) || defined(ARDUINO_CATENA_4551)
# include "Catena4551.h"
# define CATENA_H_SUPER_  McciCatena::Catena4551
#elif defined(ARDUINO_MCCI_CATENA_4610)
# include "Catena4610.h"
# define CATENA_H_SUPER_  McciCatena::Catena4610
#elif defined(ARDUINO_MCCI_CATENA_4611) || defined(ARDUINO_CATENA_4611)
# include "Catena4611.h"
# define CATENA_H_SUPER_  McciCatena::Catena4611
#elif defined(ARDUINO_MCCI_CATENA_4612) || defined(ARDUINO_CATENA_4612)
# include "Catena4612.h"
# define CATENA_H_SUPER_  McciCatena::Catena4612
#elif defined(ARDUINO_MCCI_CATENA_4801) || defined(ARDUINO_CATENA_4801)
# include "Catena4801.h"
# define CATENA_H_SUPER_  McciCatena::Catena4801
/* fallback in case it's SAMD but not what we expect */
#elif defined(ARDUINO_ARCH_SAMD)
# include "CatenaSamd21.h"
# define CATENA_H_SUPER_ McciCatena::CatenaSamd21
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
