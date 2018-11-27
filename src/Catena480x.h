/* Catena480x.h	Mon Nov 26 2018 15:50:44 chwon */

/*

Module:  Catena480x.h

Function:
	class Catena480x: CatenaBase Platform to represent a Catena 480x

Version:
	V0.12.0	Mon Nov 26 2018 15:50:44 chwon	Edit level 2

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

   0.12.0  Mon Nov 26 2018 15:50:44  chwon
	Change from CatenaStm32L0LoRa to CatenaStm32L0 class.

*/

#ifndef _CATENA480X_H_	/* prevent multiple includes */
#define _CATENA480X_H_

#pragma once

#ifndef _CATENASTM32L0_H_
# include "CatenaStm32L0.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

namespace McciCatena {

class Catena480x : public CatenaStm32L0
	{
public:
        using Super = CatenaStm32L0;

        // no specific constructor.
        Catena480x() {};

	// uses default destructor

	// neither copyable nor movable
	Catena480x(const Catena480x&) = delete;
	Catena480x& operator=(const Catena480x&) = delete;
	Catena480x(const Catena480x&&) = delete;
	Catena480x& operator=(const Catena480x&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	// methods
	virtual bool begin() override;
	virtual float ReadVbat(void) const override;
	virtual float ReadVbus(void) const override;

protected:

private:
	};

/*
|| The LoRaWAN class for the Catena 455x. Assumes The Things Network
*/
class Catena480x::LoRaWAN : public CatenaStm32L0::LoRaWAN
	{
public:
	using Super = CatenaStm32L0::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the owrk.
	*/
	LoRaWAN() {};

	bool begin(Catena480x *pParent);

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena480x.h ****/
#endif /* _CATENA480X_H_ */
