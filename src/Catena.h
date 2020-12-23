/*

Module:  Catena.h

Function:
        class Catena: virtual CatenaBase derivative representing
        the target Catena for this build.

Copyright notice:
        See accompanying LICENSE file.

Author:
        ChaeHee Won, MCCI Corporation	October 2017

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
#elif defined(ARDUINO_MCCI_CATENA_4630)
# include "Catena4630.h"
# define CATENA_H_SUPER_  McciCatena::Catena4630
#elif defined(ARDUINO_MCCI_CATENA_4617)
# include "Catena4617.h"
# define CATENA_H_SUPER_  McciCatena::Catena4617
#elif defined(ARDUINO_MCCI_CATENA_4618)
# include "Catena4618.h"
# define CATENA_H_SUPER_  McciCatena::Catena4618
#elif defined(ARDUINO_MCCI_CATENA_4801) || defined(ARDUINO_CATENA_4801)
# include "Catena4801.h"
# define CATENA_H_SUPER_  McciCatena::Catena4801
#elif defined(ARDUINO_MCCI_CATENA_4802)
# include "Catena4802.h"
# define CATENA_H_SUPER_  McciCatena::Catena4802
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
