/* Catena461x.h	Mon Nov 26 2018 15:50:21 chwon */

/*

Module:  Catena461x.h

Function:
	class Catena461x: CatenaBase Platform to represent a Catena 461x
	(4610, 4611, 4612, etc.)

Version:
	V0.12.0	Mon Nov 26 2018 15:50:21 chwon	Edit level 2

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
   0.11.0  Tue Nov 06 2018 14:25:23 chwon
	Module created.

   0.12.0  Mon Nov 26 2018 15:50:21  chwon
	Change from CatenaStm32L0LoRa to CatenaStm32L0 class.

*/

#ifndef _CATENA461X_H_	/* prevent multiple includes */
#define _CATENA461X_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

namespace McciCatena {

class Catena461x : public CatenaStm32L0
	{
public:
        using Super = CatenaStm32L0;

        // no specific constructor.
        Catena461x() {};

	// uses default destructor

	// neither copyable nor movable
	Catena461x(const Catena461x&) = delete;
	Catena461x& operator=(const Catena461x&) = delete;
	Catena461x(const Catena461x&&) = delete;
	Catena461x& operator=(const Catena461x&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	// methods
	virtual bool begin() override;

protected:

private:
	};

/*
|| The LoRaWAN class for the Catena 455x. Assumes The Things Network
*/
class Catena461x::LoRaWAN : public CatenaStm32L0::LoRaWAN
	{
public:
	using Super = CatenaStm32L0::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the owrk.
	*/
	LoRaWAN() {};

	bool begin(Catena461x *pParent);

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena461x.h ****/
#endif /* _CATENA461X_H_ */
