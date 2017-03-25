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

Name:	McciCatena::cFram2k::isValid()

Function:
	Check to see whether the FRAM looks valid

Definition:
	public: virtual bool McciCatena::cFram2k::isValid();

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

	// cache the answer
	if (this->isReady())
		return true;

	nRead = this->read(0, object.getBuffer(), object.getBufferSize());
	
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

		return false;
		}

	if (! object.hasValidSize() ||
            object.getDataSize() != sizeof(uint32_t))
		{
		Log.printf(
			Log.kError,
			"first object size not valid: %08x objectSize(%u) dataSize(%u)\n",
			object.uSizeKey,
                        object.getObjectSize(),
                        object.getDataSize()
			);
		return false;
		}

	// check the guid
	if (! object.matchesGuid(cFramStorage::skFramGuid))
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
                return false;
		}

        // get the end-of-storage pointer
        uint32_t offsetOfEndPointer = 
                object.offsetOfReplicant(
                        object.getCurrent()
                        );
        if (offsetOfEndPointer == 0)
                {
		Log.printf(
			Log.kError,
			"couldn't find end pointer: uVer[%u %u %u]\n",
                        object.uVer[0], object.uVer[1], object.uVer[2]
			);
                return false;
                }

        uint32_t endPointer;
        nRead = this->read(offsetOfEndPointer, (uint8_t *)&endPointer, sizeof(endPointer));
        if (nRead != sizeof(endPointer))
                {
                Log.printf(
                        Log.kError,
                        "error reading end pointer offset %u\n nRead %u\n",
                         (unsigned) offsetOfEndPointer,
                         (unsigned) nRead
                        );
                return false;
                }
       
       // check that endpointer is valid
       if (endPointer % cFramStorage::kObjectQuantum != 0 ||
           endPointer < object.getObjectSize())
                {
                Log.printf(
                        Log.kError,
                        "invalid end pointer %u\n",
                        endPointer
                        );
                return false;
                }

        // cache the relevant offsets and variant selectors.
	this->m_fReady = true;
        this->loadCache();
	return true;
	}

/*

Name:	McciCatena::cFram2k::initialize()

Function:
	Initialize an uninitialized FRAM

Definition:
	public: bool McciCatena::cFram2k::initialize();

Description:
	This routine establishes the invariant that the FRAM is intialized.
	It never overwrites an initialized and valid FRAM.

Returns:
	true if the FRAM is now initialized; false otherwise.

*/

bool
McciCatena::cFram2k::initialize()
	{
	if (this->isValid())
		return true;

	return this->reset();
	}

/*

Name:	McciCatena::cFram2k::reset()

Function:
	Reset the contents of the FRAM

Definition:
	public: bool McciCatena::cFram2k::reset();

Description:
	This function initailizes the header of the FRAM, and then re-reads
	the header (which initializes the rest of the internal data
	structures). Any information previously on the FRAM is lost.

Returns:
	true if all the I/O went properly, false if any failures.  After
	failures, things are in an indeterminate state.

*/

bool
McciCatena::cFram2k::reset()
	{
        cFramStorage::Object object;
        uint32_t endPointer[2];
        
        // initialize the object header
        object.initialize(
                cFramStorage::skFramGuid,
                cFramStorage::kHeader,
                sizeof(endPointer[0]),
                /* replicated */ true
                );

        // make an image of two zero endpointers
        endPointer[0] = 0;
        endPointer[1] = object.getObjectSize();

        // get the offset of the first replicant
        unsigned const offsetEndPointer = object.offsetOfReplicant(0);

        this->write(offsetEndPointer, (uint8_t *)endPointer, sizeof(endPointer));

        // now if we crash endPointer is invalid (offset is zero).
        // write fresh header
        this->write(0, object.getBuffer(), object.getBufferSize());

        // now update the replicant index
        object.setCurrent(1);
        this->write(
                0 + object.offsetOfDiscriminator(), 
                object.getDiscriminatorBuffer(), 
                object.getDiscriminatorBufferSize()
                );

        // finally, verify that the result looks good.
	return this->isValid();
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
	public: virtual void
		McciCatena::cFram2k::write(
			cFramStorage::Offset uOffset, 
			const uint8_t *pBuffer, 
			size_t nBuffer
			) override;

Description:
	The sequence of bytes at [pBuffer, pBuffer+nBuffer) is written
	to the FRAM at offset uOffset.

Returns:
	No explicit result.

*/

void
McciCatena::cFram2k::write(
	cFramStorage::Offset uOffset, 
	const uint8_t *pBuffer, 
	size_t nBuffer
	)
	{
	this->m_hw.write(uOffset, pBuffer, nBuffer);
	}
