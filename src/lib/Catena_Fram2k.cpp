/* Catena_Fram2k.cpp	Fri Mar 17 2017 19:32:14 tmm */

/*

Module:  Catena_Fram2k.cpp

Function:
	The persistent-storage provider for the Fram on Catena

Version:
	V0.5.0	Fri Mar 17 2017 19:32:14 tmm	Edit level 1

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
   0.5.0  Fri Mar 17 2017 19:32:14  tmm
	Module created.

*/

#include "Catena_Fram2k.h"

#include "Catena_FramStorage.h"
#include "Catena_Log.h"

using namespace McciCatena;

/****************************************************************************\
|
|	read-only data
|
\****************************************************************************/


/****************************************************************************\
|
|	the methods
|
\****************************************************************************/


/*

Name:	McciCatena::cFram2k::begin()

Function:
	Prepare to use the FRAM

Definition:
	public: virtual bool
		McciCatena::cFram2k::begin()
	
Description:
	Initialize an cFram2K object prior to operation.

Returns:
	true for success, false for failure.

*/

/* virtual public */
bool
McciCatena::cFram2k::begin()
	{
        if (! this->Super::begin() ||
	    ! this->m_hw.begin(0, &Wire))
		return false;

	return true;
	}

/*

Name:	McciCatena::cFram2K::read()

Function:
	Read a string of bytes from the FRAM

Definition:
	public: virtual size_t 
		McciCatena::cFram2k::read(
			cFramStorage::Offset uOffset, 
			uint8_t *pBuffer, 
			size_t nBuffer
			) override;

Description:
	nBuffer bytes are read from the FRAM device, starting at uOffset.

Returns:
	number of bytes read.

*/

size_t 
McciCatena::cFram2k::read(
	cFramStorage::Offset uOffset, 
	uint8_t *pBuffer, 
	size_t nBuffer
	)
	{
	return this->m_hw.read(uOffset, pBuffer, nBuffer);
	}

/*

Name:	McciCatena::cFram2K::write()

Function:
	Write a sequence of bytes to the FRAM.

Definition:
	public: virtual bool
		McciCatena::cFram2k::write(
			cFramStorage::Offset uOffset, 
			const uint8_t *pBuffer, 
			size_t nBuffer
			) override;

Description:
	The sequence of bytes at [pBuffer, pBuffer+nBuffer) is written
	to the FRAM at offset uOffset.

Returns:
	true for success.

Notes:
        At the moment, the lower level write can't return failure status, 
        so this routine always returns true.

*/

bool
McciCatena::cFram2k::write(
	cFramStorage::Offset uOffset, 
	const uint8_t *pBuffer, 
	size_t nBuffer
	)
	{
	this->m_hw.write(uOffset, pBuffer, nBuffer);

        return true;
	}
