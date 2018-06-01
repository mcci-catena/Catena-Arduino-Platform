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

#include <cstring>

#include "Catena_Log.h"
#include "mcciadk_baselib.h"

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

	this->resetCache();

        if (! this->isReady())
                {
                gLog.printf(
                        gLog.kError,
                        "%s: fram not ready\n",
                        __func__
                        );
                return;
                }

        cFramStorage::Offset offset;

        for (offset = 0; 
             offset < this->m_endOffset; 
             offset += object.nextObjectOffset())
                {
                if (! this->read(offset, (uint8_t *)&object, sizeof(object)))
                        {
                        gLog.printf(
                                gLog.kError,
                                "%s: read error at offset 0x%x\n",
                                __func__,
                                offset
                                );
                        return;
                        }

                if (! object.isValid())
                        {
                        gLog.printf(
                                gLog.kError,
                                "%s: invalid object at offset 0x%x\n",
                                __func__,
                                offset
                                );
                        continue;
                        }

                const uint8_t uThisKey = object.getKey();

                // TODO(tmm@mcci.com) handle GUIDs other than skFramGuid
                if (! (object.matchesGuid(cFramStorage::skFramGuid) &&
                       uThisKey < cFramStorage::StandardKeys::kMAX))
                        {
                        gLog.printf(
                                gLog.kError,
                                "%s: mismatch guid/key at offset 0x%x\n",
                                __func__,
                                offset
                                );
                        continue;
                        }

                // make sure things match definition
                cFramStorage::StandardItem const item =
                        cFramStorage::vItemDefs[uThisKey];

                if (object.isReplicated() != item.isReplicated())
                        {
                        gLog.printf(
                                gLog.kError,
                                "%s: replication mispmatch at offset 0x%x\n",
                                __func__,
                                offset
                                );
                        continue;
                        }

                if (object.getDataSize() != item.getSize())
                        {
                        gLog.printf(
                                gLog.kError,
                                "%s: mismatch size at offset 0x%x, got %u, expected %u\n",
                                __func__,
                                offset,
                                object.getDataSize(),
                                item.getSize()
                                );
                        continue;
                        }

                // take the first one.
                if (this->m_offsetCache[uThisKey] == cFramStorage::kInvalidOffset)
                        {
                        gLog.printf(
                                gLog.kTrace,
                                "%s: cache key 0x%x at offset 0x%x\n",
                                __func__,
                                uThisKey,
                                offset
                                );
                        this->m_offsetCache[uThisKey] = offset;
                        this->m_uVerCache[uThisKey] = object.getCurrent();
                        }
                else
                        {
                        gLog.printf(
                                gLog.kError,
                                "%s: key %u at offset 0x%x duplicate of offset 0x%x\n",
                                __func__,
                                uThisKey,
                                offset,
                                this->m_offsetCache[uThisKey]
                                );
                        }
                }
        gLog.printf(
                gLog.kTrace,
                "%s: cache loaded through offset 0x%x (end offset 0x%x)\n",
                __func__,
                offset,
                this->m_endOffset
                );
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
		gLog.printf(
			gLog.kError,
			"read failed; expected 0x%x, got 0x%x\n",
			object.getBufferSize(),
			nRead
			);
			
		return false;
		}

	// first object must be standard
	if (! object.isStandard())
		{
		gLog.printf(
			gLog.kError,
			"first object not standard: %08x\n",
			object.uSizeKey
			);

		return false;
		}

	if (! object.hasValidSize() ||
            object.getDataSize() != sizeof(uint32_t))
		{
		gLog.printf(
			gLog.kError,
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
		gLog.printf(
			gLog.kError,
			"guid doesn't match FRAM guid\n"
			);
		return false;
		}
	if (object.getKey() != cFramStorage::kHeader)
		{
		gLog.printf(
			gLog.kError,
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
		gLog.printf(
			gLog.kError,
			"couldn't find end pointer: uVer[%u %u %u]\n",
                        object.uVer[0], object.uVer[1], object.uVer[2]
			);
                return false;
                }

        uint32_t endPointer;
        nRead = this->read(offsetOfEndPointer, (uint8_t *)&endPointer, sizeof(endPointer));
        if (nRead != sizeof(endPointer))
                {
                gLog.printf(
                        gLog.kError,
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
                gLog.printf(
                        gLog.kError,
                        "invalid end pointer %u\n",
                        endPointer
                        );
                return false;
                }

        // cache the relevant offsets and variant selectors.
	this->m_fReady = true;
	this->m_endOffset = endPointer;
        gLog.printf(
                gLog.kAlways,
                "%s: end pointer set to 0x%x\n",
                __func__,
                endPointer
                );

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

        if (offset == cFramStorage::kInvalidOffset)
                {
                gLog.printf(
                        gLog.kBug,
                        "%s: offset for uKey(%u) invalid!\n",
                        __FUNCTION__,
                        uKey
                        );

                return false;
                }

	if (nBuffer != item.getSize())
		{
		gLog.printf(
			gLog.kBug,
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
                gLog.printf(
                        gLog.kTrace, 
                        "key(0x%x) write new version(%u): base(%u) offset(%u) size(%u)\n",
                        uKey,
                        newVer,
                        offset,
                        offset + cFramStorage::Object::offsetOfDiscriminator(),
                        sizeof(uVers[newVer])
                        );

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

void McciCatena::cFram::saveField(
        cFramStorage::StandardKeys uKey,
        const uint8_t *pValue,
        size_t nValue
        )
        {
        cFram::Cursor cursor(this, uKey);

        if (! cursor.create())
                {
                gLog.printf(gLog.kError,
                        "%s: can't save uKey(0x%x) %u bytes\n",
                        __FUNCTION__, uKey, nValue
                        );
                }

        cursor.put(pValue, nValue);
        }

bool McciCatena::cFram::getField(
        cFramStorage::StandardKeys uKey,
        uint8_t *pValue,
        size_t nValue
        )
        {
        cFram::Cursor cursor(this, uKey);

        return cursor.get(pValue, nValue);
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
		gLog.printf(
			gLog.kError,
			"%s: can't allocate 0x%#x bytes\n",
			object.getObjectSize()
			);

		return false;
		}

	// write object and the default value
        cFramStorage::Offset const objOffset = offset;
	pFram->write(offset, (uint8_t *) &object, sizeof(object));
	
	offset += sizeof(object);
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
                pFram->m_offsetCache[this->m_uKey] = objOffset;
                this->m_offset = objOffset;
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
        static const char FUNCTION[] = "cFram::Cursor::locate";
        uint8_t const uKey = item.getKey();

	// don't crash on invalid param.
	if (uKey >= cFramStorage::StandardKeys::kMAX)
                {
                gLog.printf(
                        gLog.kBug,
                        "%s: invalid item key(0x%x)\n",
                        FUNCTION,
                        uKey
                        );

		return false;
                }

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
        static const char FUNCTION[] = "cFram::Cursor::put";

	if (! this->islocated() ||
	    ! this->isbound())
                {
                gLog.printf(
                        gLog.kBug,
                        "%s: can't put to un%s cursor: "
                        "uSize(0x%x) uKey(0x%x) uVer(%u) offset(0x%x)\n",
                        FUNCTION,
                        (! this->islocated()) ? "located" : "bound",
                        this->m_uSize,
                        this->m_uKey,
                        this->m_uVer,
                        this->m_offset
                        );
		return false;
                }

	return this->m_pFram->writeItemData(
		cFramStorage::vItemDefs[this->m_uKey],
		pBuffer,
		nBuffer
		);
	}

bool
McciCatena::cFram::Cursor::getitem(
        cFramStorage::StandardItem &item
        ) const
        {
        if (! this->isbound())
                return false;

        item = cFramStorage::vItemDefs[this->m_uKey];
        return true;
        }

size_t
McciCatena::cFram::Cursor::getitemsize() const
        {
        cFramStorage::StandardItem item;

        if (! this->getitem(item))
                return 0;

        return item.getSize();
        }

static int getnibble(const char * &pValue)
        {
        char c;

        c = *pValue;
        if ('0' <= c && c <= '9')
                {
                ++pValue;
                return c - '0';
                }
        else if ('A' <= c && c <= 'F')
                {
                ++pValue;
                return c - 'A' + 0xA;
                }
        else if ('a' <= c && c <= 'f')
                {
                ++pValue;
                return c - 'a' + 0xA;
                }
        else
                return -1;
        }

static int getbyte(const char * &pValue, const char * &pTerm)
        {
        int v;

        if (pTerm == nullptr || pTerm == pValue)
                {
                pTerm = std::strchr(pValue, '-');
                if (pTerm == nullptr)
                        pTerm = std::strchr(pValue, '\0');
                }

        bool fHavePair = ((pTerm - pValue) & 1) == 0;

        v = getnibble(pValue);
        if (fHavePair && v >= 0)
                {
                int v2;
                v2 = getnibble(pValue);
                if (v2 >= 0)
                        return v * 16 + v2;
                else
                        return v;
                }
        else
                return v;
        }

bool 
McciCatena::cFram::Cursor::parsevalue(
        const char *pValue,
        uint8_t *pData,
        size_t nData
        ) const
        {
        // we don't write the data, but we use the cursor to
        // help decode it.
        cFramStorage::StandardItem item;

        if (! this->getitem(item))
                return false;
        if (nData != item.getSize())
                return false;

        // value is hex, and may have embedded '-' to separate
        // bytes. At the end, if it's a number, we have to reverse 
        // it.... for sanity, start with zeros.  As a further
        // headache, if we have an odd-number of consecutive numeric
        // nibbles, we need to right justify, not left.
        std::memset(pData, 0, nData);

        char c;
        size_t i;

        i = 0;

        const char *pTerm = nullptr;

        for (i = 0; i < nData; )
                {
                // find the end of this string of chars
                int v = getbyte(pValue, pTerm);
                if (v < 0)
                        {
                        if (pValue[0] == '\0')
                                break;
                        else
                                return false;
                        }

                pData[i++] = uint8_t(v);
                if (pValue[0] == '-' && pValue[1] != '\0')
                        {
                        // skip the separator
                        ++pValue;

                        // reset the scanner:
                        pTerm = nullptr;
                        }
                }

        // ok, we might have fewer bytes than specified. in that case, right justify.
        if (i < nData)
                {
                for (size_t j = nData; j > 0; --j)
                        {
                        if (i > 0)
                                {
                                pData[j - 1] = pData[i - 1];
                                --i;
                                }
                        else
                                pData[j - 1] = 0;
                        }
                }

        // if it's a number, we now have to byte-reverse
        if (item.isNumber())
                {
                size_t nData2 = nData/2;
                size_t i, j;
                for (i = 0, j = nData-1; i < nData/2; ++i, --j)
                        std::swap(pData[i], pData[j]);
                }

        return true;
        }

size_t 
McciCatena::cFram::Cursor::formatvalue(
        char *pBuffer,
        size_t nBuffer,
        size_t iBuffer,
        const uint8_t *pData,
        size_t nData
        ) const
        {
        // data has been fetched, but we need to use the 
        // cursor info to format it.
        cFramStorage::StandardItem item;

        if (iBuffer < nBuffer)
                pBuffer[iBuffer] = '\0';

        if (! this->getitem(item))
                return iBuffer;

        // if it's a number, we need to reverse it.
        // TODO(tmm@mcci.com): better formatting for GUIDs
        for (size_t i = 0; i < nData; ++i)
                {
                size_t j;
                const char *pSep;
                pSep = "-";
                if (item.isNumber())
                        {
                        j = nData - i - 1;
                        if (nData <= 4 || j == 0)
                                pSep = "";
                        }
                else
                        {
                        j = i;
                        if (i == nData - 1)
                                pSep = "";
                        }

                iBuffer += McciAdkLib_Snprintf(
                                pBuffer,
                                nBuffer,
                                iBuffer,
                                "%02x%s",
                                pData[j],
                                pSep
                                );
                }

        return iBuffer;
        }
