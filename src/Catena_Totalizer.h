/* Catena_Totalizer.h	Mon Mar 27 2017 00:18:49 tmm */

/*

Module:  Catena_Totalizer.h

Function:
	class McciCatena:cTotalizer.

Version:
	V0.5.0	Mon Mar 27 2017 00:18:49 tmm	Edit level 1

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
   0.5.0  Mon Mar 27 2017 00:18:49  tmm
	Module created.

*/

#ifndef _CATENA_TOTALIZER_H_		/* prevent multiple includes */
#define _CATENA_TOTALIZER_H_

#pragma once

#include <cstdint>

#ifndef _CATENA_POLLABLEINTERFACE_H_
# include "Catena_PollableInterface.h"
#endif

namespace McciCatena {

class cTotalizer : public cPollableObject
	{
public:
	enum : uint8_t
		{
		kPinUndefined = 0xFFu,
		};

	cTotalizer(uint8_t pin = kPinUndefined) : m_pin(pin) {};

	// setup, passing in a pin
	bool begin(uint8_t pin = kPinUndefined);

	// totalize
	virtual void poll() override;

	// get the running total
	uint32_t getcurrent() const
		{ return this->m_total; };
        bool getDeltaCountAndTime(uint32_t &outCount, uint32_t &outDelta);
        void setReference();

private:
        // total number of counts seen
	uint32_t m_total;
        // time of last change (from millis())
        uint32_t m_tEdge;
        // time of last measurement
        uint32_t m_tLastMeasured;
        uint32_t m_nLastMeasured;
        // last bit value observed.
	bool m_last;
        // last debounced bit value observed
        bool m_lastStable;
        // value has stabilized
        bool m_fIsStable;
        // true if we have a valid previous measurement
        bool m_fHaveDelta;
        // counter is initialized
	bool m_initialized = false;
        // Arduino digital pin to be monitored.
	uint8_t m_pin;
	};

}; // namespace McciCatena

/**** end of Catena_Totalizer.h ****/
#endif /* _CATENA_TOTALIZER_H_ */
