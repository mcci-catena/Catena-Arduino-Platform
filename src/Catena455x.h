/* Catena455x.h	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  Catena455x.h

Function:
	class Catena455x: CatenaBase Platform to represent a Catena 455x
	(4550, 4551, etc.)

Version:
	V0.6.0	Fri Oct 13 2017 15:19:30 chwon	Edit level 1

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

*/

#ifndef _CATENA455X_H_		/* prevent multiple includes */
#define _CATENA455X_H_

#pragma once

#ifndef _CATENASTM32L0LORA_H_
# include "CatenaStm32L0LoRa.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

namespace McciCatena {

class Catena455x : public CatenaStm32L0LoRa
	{
public:
        using Super = CatenaStm32L0LoRa;

        // no specific constructor.
        Catena455x() {};

	// uses default destructor

	// neither copyable nor movable
	Catena455x(const Catena455x&) = delete;
	Catena455x& operator=(const Catena455x&) = delete;
	Catena455x(const Catena455x&&) = delete;
	Catena455x& operator=(const Catena455x&&) = delete;

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
class Catena455x::LoRaWAN : public CatenaStm32L0LoRa::LoRaWAN
	{
public:
	using Super = CatenaStm32L0LoRa::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the owrk.
	*/
	LoRaWAN() {};

	bool begin(Catena455x *pParent);

protected:

private:
	};

} // namespace McciCatena

/**** end of Catena455x.h ****/
#endif /* _CATENA455X_H_ */
