/* Catena4450.h	Sun Mar 12 2017 18:16:07 tmm */

/*

Module:  Catena4450.h

Function:
	class Catena4450: CatenaBase Platform to represent a Catena 4450

Version:
	V0.5.0	Sun Mar 12 2017 18:16:07 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	March 2017

Revision history:
   0.5.0  Sun Mar 12 2017 18:16:07  tmm
	Module created.

*/

#ifndef _CATENA4450_H_		/* prevent multiple includes */
#define _CATENA4450_H_

#ifndef _CATENAFEATHERM0LORA_H_
# include "CatenaFeatherM0LoRa.h"
#endif

#include <Arduino_LoRaWAN_ttn.h>

class Catena4450 : public CatenaFeatherM0LoRa
	{
public:
	Catena4450() {};
	// default destructor

	// neither copyable nor movable
	Catena4450(const Catena4450&) = delete;
	Catena4450& operator=(const Catena4450&) = delete;
	Catena4450(const Catena4450&&) = delete;
	Catena4450& operator=(const Catena4450&&) = delete;

	// LoRaWAN binding
	class LoRaWAN /* forward */;

	// methods
	bool begin();
	
protected:
	using Super = CatenaFeatherM0LoRa;
	
	};

/*
|| The LoRaWAN class for the Catena 4450. Assumes The Things Network
*/
class Catena4450::LoRaWAN : public CatenaFeatherM0LoRa::LoRaWAN
	{
public:
	using Super = CatenaFeatherM0LoRa::LoRaWAN;

	/*
	|| the constructor. We don't do anything at this level, the
	|| Super constructor does most of the owrk.
	*/
	LoRaWAN() {};
	LoRaWAN(const lmic_pinmap &pinmap) : Super(pinmap) {};

	bool begin(Catena4450 *pParent);

protected:

private:
	Catena4450		*m_pCatena;
	};

/**** end of Catena4450.h ****/
#endif /* _CATENA4450_H_ */
