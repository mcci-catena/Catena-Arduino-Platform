/* Catena_Totalizer.cpp	Mon Mar 27 2017 00:27:22 tmm */

/*

Module:  Catena_Totalizer.cpp

Function:
	class McciCatena::Totalizer implemenation.

Version:
	V0.5.0	Mon Mar 27 2017 00:27:22 tmm	Edit level 1

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
   0.5.0  Mon Mar 27 2017 00:27:22  tmm
	Module created.

*/

#include "Catena_Totalizer.h"

#include "CatenaBase.h"

using namespace McciCatena;

/****************************************************************************\
|
|	Method functions
|
\****************************************************************************/

bool
McciCatena::cTotalizer::begin(
	uint8_t pin
	)
	{
	if (this->m_pin == kPinUndefined &&
	    pin == kPinUndefined)
		{
		return false;
		}

	if (this->m_initialized)
		return true;

	if (this->m_pin == kPinUndefined)
		this->m_pin = pin;

	// set up the input
	pinMode(this->m_pin, INPUT);
	this->m_last = digitalRead(this->m_pin);
	this->m_lastStable = this->m_last;
	this->m_initialized = true;
	this->m_fIsStable = true;
        this->m_total = 0;
        this->m_tEdge = 0;
        this->m_tLastMeasured = 0;
        this->m_nLastMeasured = 0;
        this->m_tEdge = 0;
        this->m_fHaveDelta = false;

	// register with the polling engine
	CatenaBase::pCatenaBase->registerObject(this);

	return true;
	}

// public: virtual override
void 
McciCatena::cTotalizer::poll()
	{
	bool fCurrent = digitalRead(this->m_pin);
	uint32_t now = millis();

	if (fCurrent != this->m_last)
		{
		this->m_fIsStable = false;
		this->m_tEdge = now;
		this->m_last = fCurrent;
		}
	// current == last
	else if (! this->m_fIsStable)
		{
		// 50 millisecond debounce?
		if ((now - this->m_tEdge) > 50)
			{
			this->m_fIsStable = true;
			if (fCurrent != this->m_lastStable)
				{
				// count on falling edges
				if (! fCurrent)
					++this->m_total;

				this->m_lastStable = fCurrent;
				}
			}
		}
	}

bool 
McciCatena::cTotalizer::getDeltaCountAndTime(
        uint32_t &outCount, 
        uint32_t &outDelta
        )
        {
        if (this->m_fHaveDelta) 
                {
                outCount = this->m_total - this->m_nLastMeasured;
                outDelta = this->m_tEdge - this->m_tLastMeasured;
                }
        else
                outCount = outDelta = 0;

        return this->m_fHaveDelta;
        }

void 
McciCatena::cTotalizer::setReference()
        {
        this->m_tLastMeasured = this->m_tEdge;
        this->m_nLastMeasured = this->m_total;
        this->m_fHaveDelta = true;
        }
