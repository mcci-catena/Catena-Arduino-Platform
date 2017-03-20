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
#include "Catena_Guids_FramStorage.h"
#include "Catena_Log.h"

using namespace McciCatena;

/****************************************************************************\
|
|	read-only data
|
\****************************************************************************/

static const MCCIADK_GUID_WIRE skFramGuid = GUID_FRAM_CATENA_V1(WIRE);

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
	public: virtual
	

Description:
	xxx

Returns:
	xxx

*/

/* virtual public */
bool 
McciCatena::cFram2k::begin()
	{
	if (! this->m_hw.begin(0, &Wire))
		return false;

	return true;
	}

/*

Name:	cFram2k::isValid()

Function:
	Check to see whether the FRAM looks valid

Definition:
	public: virtual bool cFram2k::isValid();

Description:
	We look for valid begin block, a series of intermediate blocks,
	and an end block. Any problems ==> give up.

Returns:
	true for validity, false otherwise.

*/

bool 
McciCatena::cFram2k::isValid()
	{
	cFramStorage::Object object;
	size_t nRead;

	nRead = this->m_hw.read(0, object.getBuffer(), object.getBufferSize());
	
	if (nRead != object.getBufferSize())
		{
		Log.printf(
			Log.kError,
			"read failed; expected 0x%x, got 0x%x\n",
			object.getBufferSize(),
			nRead
			);
			
		return false;
		}

	// first object must be standard
	if (! object.isStandard())
		{
		Log.printf(
			Log.kError,
			"first object not standard: %08x\n",
			object.uSizeKey
			);

		return true;
		}

	if (! object.hasValidSize())
		{
		Log.printf(
			Log.kError,
			"first object size not valid: %08x\n",
			object.uSizeKey
			);
		return false;
		}

	// check the guid
	if (object.matchesGuid(skFramGuid))
		{
		Log.printf(
			Log.kError,
			"guid doesn't match FRAM guid\n"
			);
		return false;
		}
	if (object.getKey() != cFramStorage::kHeader)
		{
		Log.printf(
			Log.kError,
			"key is not kHeader: %02x\n",
			object.getKey()
			);
		}
	return false;
	}

bool
McciCatena::cFram2k::initialize()
	{
	if (this->isValid())
		return true;

	return this->reset();
	}

bool
McciCatena::cFram2k::reset()
	{
	// write header
	return false;
	}

