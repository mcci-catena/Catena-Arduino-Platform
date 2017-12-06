/* Catena_Platforms.h	Sat Mar 11 2017 13:57:05 tmm */

/*

Module:  Catena_Platforms.h

Function:
	The global platform definitions.

Version:
	V0.5.0	Sat Mar 11 2017 13:57:05 tmm	Edit level 4

Copyright notice:
	This file copyright (C) 2016-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Mon Oct 17 2016 03:35:01  tmm
	Module created.

   0.3.0  Mon Oct 31 2016 13:52:28  tmm
	Add anatolian platform.

   0.4.0  Wed Mar  8 2017 18:11:46  tmm
	Add Catena 4450 platforms.

   0.5.0  Sat Mar 11 2017 13:57:05  tmm
	Revise to match naming standards; add McciCatena namespace.

*/

#ifndef _CATENA_PLATFORMS_H_		/* prevent multiple includes */
#define _CATENA_PLATFORMS_H_

#ifndef _CATENABASE_H_
# include "CatenaBase.h"
#endif

namespace McciCatena {

extern const CATENA_PLATFORM	gkPlatformFeatherM0;
extern const CATENA_PLATFORM	gkPlatformM0LoRa;
extern const CATENA_PLATFORM	gkPlatformM0LoRaTtnNyc;
extern const CATENA_PLATFORM	gkPlatformCatena4410;
extern const CATENA_PLATFORM	gkPlatformCatena4410_anatolian;
extern const CATENA_PLATFORM	gkPlatformCatena4410_gh;
extern const CATENA_PLATFORM	gkPlatformCatena4410_mh;
extern const CATENA_PLATFORM	gkPlatformCatena4410_swh; 
extern const CATENA_PLATFORM	gkPlatformCatena4410_pond;
extern const CATENA_PLATFORM	gkPlatformCatena4450;
extern const CATENA_PLATFORM	gkPlatformCatena4450_m101;
extern const CATENA_PLATFORM	gkPlatformCatena4450_m102;
extern const CATENA_PLATFORM    gkPlatformFeatherM0Proto;
extern const CATENA_PLATFORM    gkPlatformFeatherM0ProtoWingLora;

} /* namespace McciCatena */

/**** end of Catena_Platforms.h ****/
#endif /* _CATENA_PLATFORMS_H_ */
