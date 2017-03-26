/* Catena_Fram.cpp	Fri Mar 24 2017 16:50:02 tmm */

/*

Module:  Catena_Fram.cpp

Function:
	class McciCatena::cFram implementation.

Version:
	V0.5.0	Fri Mar 24 2017 16:50:02 tmm	Edit level 1

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
   0.5.0  Fri Mar 24 2017 16:50:02  tmm
	Module created.

*/

#include "Catena_Fram.h"

#include "Catena_Log.h"

using namespace McciCatena;

/****************************************************************************\
|
|	The cFram methods
|
\****************************************************************************/

/*

Name:	McciCatena::cFram::loadCache()

Function:
	Load the cache by scanning the media.

Definition:
	protected: void
		cFram::loadCache(
			void
			);

Description:
	The cache is loaded with pointers to all the objects found on the
	media.

Returns:
	No explicit result.

*/

void
McciCatena::cFram::loadCache(
	void
	)
        {
        cFramStorage::Object object;
        const MCCIADK_GUID_WIRE * const pGuid = &cFramStorage::skFramGuid;

        if (! this->isReady())
                return;

	this->resetCache();

        for (cFramStorage::Offset offset = 0; 
             offset < this->m_endOffset; 
             offset += object.getObjectSize())
                {
                if (! this->read(offset, (uint8_t *)&object, sizeof(object)))
                        return;

                if (! object.isValid())
                        continue;

                const uint8_t uThisKey = object.getKey();

                // TODO(tmm@mcci.com) handle GUIDs other than skFramGuid
                if (! (object.matchesGuid(cFramStorage::skFramGuid) &&
                       uThisKey < cFramStorage::StandardKeys::kMAX))
                        continue;

                // make sure things match definition
                cFramStorage::StandardItem const item =
                        cFramStorage::vItemDefs[uThisKey];

                if (object.isReplicated() != item.isReplicated())
                        continue;

                if (object.getDataSize() != item.getSize())
                        continue;

                // take the first one.
                if (this->m_offsetCache[uThisKey] == cFramStorage::kInvalidOffset)
                        {
                        this->m_offsetCache[uThisKey] = offset;
                        this->m_uVerCache[uThisKey] = object.getCurrent();
                        }
                }
        }

/*

Name:	McciCatena::cFram::resetCache()

Function:
	Reset the cache.

Definition:
	protected: void
		cFram::resetCache(
			void
			);

Description:
	All cache entries are reset to "invalid".

Returns:
	No explicit result.

*/

void
McciCatena::cFram::resetCache(
	void
	)
	{
	for (unsigned i = 0; i < cFramStorage::StandardKeys::kMAX; ++i)
		{
		this->m_offsetCache[i] = cFramStorage::kInvalidOffset;
		this->m_uVerCache[i] = 0;
		this->m_endOffset = 0;
		}
	}

/*

Name:	McciCatena::cFram::setCacheEntry()

Function:
	Set the data for a given key.

Definition:
	protected: void 
		McciCatena::cFram::setCacheEntry(
			cFramStorage::StandardKeys uKey,
			cFramStorage::Offset offset,
			uint8_t uVer
			);

Description:
	The specified values are stored in the cache.

Returns:
	No explicit result.

*/

void 
McciCatena::cFram::setCacheEntry(
	cFramStorage::StandardKeys uKey,
	cFramStorage::Offset offset,
	uint8_t uVer
	)
	{
	if (uKey < cFramStorage::StandardKeys::kMAX)
		{
		this->m_offsetCache[uKey] = offset;
		this->m_uVerCache[uKey] = uVer;
		}
	}

/*

Name:	McciCatena::cFram::isValid()

Function:
	Check to see whether the FRAM looks valid

Definition:
	public: virtual bool McciCatena::cFram::isValid();

Description:
	We look for valid begin block, a series of intermediate blocks,
	and an end block. Any problems ==> give up.

Returns:
	true for validity, false otherwise.

*/

bool
McciCatena::cFram::isValid()
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
	this->m_endOffset = endPointer;

        this->loadCache();
	return true;
	}

/*

Name:	McciCatena::cFram::initialize()

Function:
	Initialize an uninitialized FRAM

Definition:
	public: bool McciCatena::cFram::initialize();

Description:
	This routine establishes the invariant that the FRAM is intialized.
	It never overwrites an initialized and valid FRAM.

Returns:
	true if the FRAM is now initialized; false otherwise.

*/

bool
McciCatena::cFram::initialize()
	{
	if (this->isValid())
		return true;

	return this->reset();
	}

/*

Name:	McciCatena::cFram::reset()

Function:
	Reset the contents of the FRAM

Definition:
	public: bool McciCatena::cFram::reset();

Description:
	This function initailizes the header of the FRAM, and then re-reads
	the header (which initializes the rest of the internal data
	structures). Any information previously on the FRAM is lost.

Returns:
	true if all the I/O went properly, false if any failures.  After
	failures, things are in an indeterminate state.

*/

bool
McciCatena::cFram::reset()
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

cFramStorage::Offset
McciCatena::cFram::allocate(
	size_t nBytes
	)
	{
	cFramStorage::Offset mediaSize = this->getsize();
	cFramStorage::Offset available;

	if (! this->isReady())
		return cFramStorage::kInvalidOffset;

	available = mediaSize - this->m_endOffset;
	if (available > mediaSize)
		// endOffset is not valid!
		return cFramStorage::kInvalidOffset;

	// round nBytes.
	size_t const nAlloc = cFramStorage::getClicks(nBytes) * 
			      cFramStorage::kObjectQuantum;

	if (nAlloc < nBytes)
		// overflow!
		return cFramStorage::kInvalidOffset;

	if (available < nAlloc)
		return cFramStorage::kInvalidOffset;

	// we have that many bytes.  return the current end offset.
	// we don't commit the allocation yet!
	return this->m_endOffset;
	}


bool
McciCatena::cFram::writeItemData(
	cFramStorage::StandardItem item,
	const uint8_t *pBuffer,
	size_t nBuffer
	)
	{
	const uint8_t uKey = item.getKey();
	const cFramStorage::Offset offset = this->m_offsetCache[uKey];
	static const uint8_t uVers[2][3] =	
		{
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		};

	if (nBuffer != item.getSize())
		{
		Log.printf(
			Log.kAlways,
			"%s: nBuffer(%u) != item.getSize()(%u)\n",
			__FUNCTION__,
                        nBuffer,
                        item.getSize()
                        );
			
		return false;
		}

	if (! item.isReplicated())
		return this->write(
			offset + cFramStorage::dataOffset(
					item.getSize(),
					0
					),
			pBuffer,
			nBuffer
			);
	
	bool fResult;
	const uint8_t newVer = ! this->m_uVerCache[uKey];

	fResult = this->write(
			offset + cFramStorage::dataOffset(
					item.getSize(),
					newVer
					),
			pBuffer,
			nBuffer
			);

	if (fResult)
		{
		fResult = this->write(
			offset + offsetof(cFramStorage::ObjectRaw,
					 uVer[0]),
			uVers[newVer],
			sizeof(uVers[newVer])
			);

		if (fResult)
			this->m_uVerCache[uKey] = newVer;
		}

	return fResult;
	}

/****************************************************************************\
|
|	The Cursor methods
|
\****************************************************************************/


/*

Name:	cFram::Cursor::Cursor()

Function:
	The constructors for cFram::Cursor instances.

Definition:
	cFram::Cursor::Cursor(
		cFram *pFram
		);

	cFram::Cursor::Cursor(
		cFram *pFram,
		cFramStorage::StandardKeys uKey
		);

	cFram::Cursor::Cursor(
		cFram *pFram,
		cFramStorage::StandardItem item
		);

Description:
	The first form constructs the cursor in an uninitalized state.
	The other forms attempt to bind the cursor to a speicfic item in
	the storage, either by standard key or by standard item.

Returns:
	No explicit result.

*/

cFram::Cursor::Cursor(
	cFram *pFram,
	cFramStorage::StandardItem item
	)
	// do the basic initializatoin
	: Cursor{pFram}
	{
	// then try to locate.
	(void) this->locate(item);
	}

/*

Name:	McciCatena::cFram::Cursor::create()

Function:
	Ensure, if possible, that there's an object backing a cursor.

Definition:
	bool cFram::Cursor::create(
		void
		);

Description:
	We try to locate the object under the cursor, and if successful
	we simply return success.

	Otherwise we append a new object of the type described by the cursor
	to the FRAM with a default value, update the cache, and return
	with the object suitably located.

Returns:
	true for success, false for failure.

Notes:
	At the moment, all objects must be of the standard type.

*/

bool
McciCatena::cFram::Cursor::create(
	void
	)
	{
	static const uint8_t zeroes[cFramStorage::kObjectQuantum] {0};

	// try to locate; if success, just leave.
	if (this->islocated())
		return true;

	if (! this->isbound())
		return false;

	// make sure there's room for this object, or fail.
	cFram * const pFram = this->m_pFram;

	cFramStorage::Object object;

	object.initialize(this->getItem());

	cFramStorage::Offset offset;

	offset = pFram->allocate(object.getObjectSize());

	if (offset == cFramStorage::kInvalidOffset)
		{
		Log.printf(
			Log.kError,
			"%s: can't allocate 0x%#x bytes\n",
			object.getObjectSize()
			);

		return false;
		}

	// write object and the default value
	pFram->write(offset, (uint8_t *) &object, sizeof(object));
	
	offset += object.getObjectSize();
	for (auto nWrite = object.getObjectSize() - sizeof(object);
	     nWrite > 0;
	     nWrite -= sizeof(zeroes), offset += sizeof(zeroes))
		{
		pFram->write(offset, zeroes, sizeof(zeroes));
		}

	// set the end pointer
	// note that the header object is in the cache at slot zero.
	// so this->m_uVerCache[0] has the version of the size.

	if (pFram->writeItemData(
		cFramStorage::vItemDefs[cFramStorage::StandardKeys::kHeader],
		(const uint8_t *) &offset,
		sizeof(offset)
		))
		{
		pFram->m_endOffset = offset;
		return true;	
		}

	return false;
	}

/*

Name:	McciCatena::cFram::Cursor::locate()

Function:
	Set up a cursor for accessing an item.

Definition:
	public: bool 
		McciCatena::cFram::Cursor::locate(
			const cFramStorage::StandardItem item
			);

Description:
	The Cursor instance is bound to the first instance of the specified
	item in the FRAM.  If the FRAM is not initialized or if the item
	cannot be found, this returns false and the cursor is left unbound.

Returns:
	true if cursor is bound and ready for I/O.

*/

bool 
McciCatena::cFram::Cursor::locate(
	const cFramStorage::StandardItem item
	)
	{
        uint8_t const uKey = item.getKey();

	// don't crash on invalid param.
	if (uKey >= cFramStorage::StandardKeys::kMAX)
		return false;

	// if the FRAM isn't ready, give up.
	if (! this->m_pFram->isReady())
		return false;

	this->m_uKey = uKey;
	this->m_uSize = item.getSize();
	this->m_offset = this->m_pFram->m_offsetCache[uKey];

	// load the cache?
	if (this->m_offset == cFramStorage::kInvalidOffset)
		{
                return false;
		}

	// load the cached version toggle
	this->m_uVer = this->m_pFram->m_uVerCache[uKey];

	return true;
	}

bool 
McciCatena::cFram::Cursor::get(
	uint8_t *pBuffer, 
	size_t nBuffer
	)
	{
	if (! this->m_pFram->isReady())
		return false;
	if (this->m_offset == cFramStorage::kInvalidOffset)
		return false;
	if (nBuffer != this->m_uSize)
		return false;

	return this->m_pFram->read(
		this->m_offset + cFramStorage::dataOffset(this->m_uSize, this->m_uVer),
                pBuffer,
                nBuffer
		);

	return false;
	}

bool
McciCatena::cFram::Cursor::put(
	const uint8_t *pBuffer,
	size_t nBuffer
	)
	{
	if (! this->islocated() ||
	    ! this->isbound())
		return false;

	return this->m_pFram->writeItemData(
		cFramStorage::vItemDefs[this->m_uKey],
		pBuffer,
		nBuffer
		);
	}
