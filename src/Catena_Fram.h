/* Catena_Fram.h	Fri Mar 24 2017 14:53:52 tmm */

/*

Module:  Catena_Fram.h

Function:
	FRAM of various sizes and connectivities.

Version:
	V0.5.0	Fri Mar 24 2017 14:53:52 tmm	Edit level 1

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
   0.5.0  Fri Mar 24 2017 14:53:52  tmm
	Module created.

*/

#ifndef _CATENA_FRAM_H_		/* prevent multiple includes */
#define _CATENA_FRAM_H_

#pragma once

#ifndef _CATENA_COMMANDSTREAM_H_
# include "Catena_CommandStream.h"
#endif

#ifndef _CATENA_FRAMSTORAGE_H_
# include "Catena_FramStorage.h"
#endif

#ifndef _CATENA_PERSISTENTSTORAGE_H_
# include "Catena_PersistentStorage.h"
#endif

namespace McciCatena {

/*

Name:	McciCatena::cFramItemTable

Function:
	A table of FRAM items stored under one GUID.

Description:
	Each object in the FRAM carries a GUID and a one-byte key. The
	platform's standard items (cFramStorage::vItemDefs) form one table,
	under cFramStorage::skFramGuid. An application can describe its own
	items in a table under its own GUID, and register it with
	cFram::registerItemTable(); cFram::Cursor then reads and writes
	those items as it does standard ones.

	Items are found by key, not by position in the table, so keys need
	not be consecutive. The table also holds the caches for its items
	(the offset of each object and its current replicant), which the
	caller provides, sized for the table; cFramAppItemTable<N> provides
	them for N items.

*/

class cFramItemTable
	{
public:
	cFramItemTable(
		const MCCIADK_GUID_WIRE &guid,
		const cFramStorage::StandardItem *pItems,
		unsigned nItems,
		cFramStorage::Offset *pOffsetCache,
		uint8_t *pVerCache
		)
		: m_pGuid(&guid)
		, m_pItems(pItems)
		, m_nItems(nItems)
		, m_pOffsetCache(pOffsetCache)
		, m_pVerCache(pVerCache)
		{}

	// neither copyable nor movable: the FRAM keeps a pointer to it.
	cFramItemTable(const cFramItemTable&) = delete;
	cFramItemTable& operator=(const cFramItemTable&) = delete;
	cFramItemTable(const cFramItemTable&&) = delete;
	cFramItemTable& operator=(const cFramItemTable&&) = delete;

	const MCCIADK_GUID_WIRE &getGuid() const
		{
		return *this->m_pGuid;
		}

	unsigned getCount() const
		{
		return this->m_nItems;
		}

	// the index of the item with key uKey, or -1 if there is none.
	int findKey(uint8_t uKey) const
		{
		if (uKey < this->m_nItems && this->m_pItems[uKey].getKey() == uKey)
			return uKey;

		for (unsigned i = 0; i < this->m_nItems; ++i)
			if (this->m_pItems[i].getKey() == uKey)
				return int(i);

		return -1;
		}

	const cFramStorage::StandardItem &getItem(unsigned i) const
		{
		return this->m_pItems[i];
		}

	cFramStorage::Offset getOffset(unsigned i) const
		{
		return this->m_pOffsetCache[i];
		}

	uint8_t getVer(unsigned i) const
		{
		return this->m_pVerCache[i];
		}

	void setEntry(unsigned i, cFramStorage::Offset offset, uint8_t uVer)
		{
		this->m_pOffsetCache[i] = offset;
		this->m_pVerCache[i] = uVer;
		}

	void setVer(unsigned i, uint8_t uVer)
		{
		this->m_pVerCache[i] = uVer;
		}

	// forget every cached object.
	void resetCache()
		{
		for (unsigned i = 0; i < this->m_nItems; ++i)
			this->setEntry(i, cFramStorage::kInvalidOffset, 0);
		}

private:
	friend class cFram;

	const MCCIADK_GUID_WIRE *m_pGuid;
	const cFramStorage::StandardItem *m_pItems;
	unsigned m_nItems;
	cFramStorage::Offset *m_pOffsetCache;
	uint8_t *m_pVerCache;
	cFramItemTable *m_pNext = nullptr;	// in the FRAM's list of tables
	};

// an application's item table, with caches for its N items.
template <unsigned N>
class cFramAppItemTable : public cFramItemTable
	{
public:
	cFramAppItemTable(
		const MCCIADK_GUID_WIRE &guid,
		const cFramStorage::StandardItem (&items)[N]
		)
		: cFramItemTable(guid, items, N, m_offsetCache, m_verCache)
		{
		this->resetCache();
		}

private:
	cFramStorage::Offset m_offsetCache[N];
	uint8_t m_verCache[N];
	};

// this is a dummy layer for refactoring later
class cFram : public cPersistentStorage
	{
protected:
	using Super = cPersistentStorage;

public:
        virtual bool begin() override
                {
                this->resetCache();
                this->m_endOffset = 0;
                this->m_fReady = false;
                return true;
                }

        // initialize the store
        virtual bool initialize() override;

        // check the store
        virtual bool isValid() override;

        // reset the store
        virtual bool reset() override;

        // read from the store.
	virtual size_t read(
		cFramStorage::Offset uOffset, uint8_t *pBuffer, size_t nBuffer
		) = 0;

        // write to the store
	virtual bool write(
		cFramStorage::Offset uOffset, const uint8_t *pBuffer, size_t nBuffer
		) = 0;

        // return the unformatted size of the store.
        virtual cFramStorage::Offset getsize() const = 0;

        // return state of the FRAM system
        virtual bool isReady() const override { return this->m_fReady; }

        virtual void invalidate() override { this->m_fReady = false; }

	class Cursor;

	using ItemTable = cFramItemTable;
	template <unsigned N> using AppItemTable = cFramAppItemTable<N>;

	// register an application's item table. If the store is valid, scan
	// it now for the table's objects; otherwise the scan happens when the
	// store becomes valid. Registering a table again does nothing. The
	// table must outlive the FRAM object, or at least its use.
	bool registerItemTable(ItemTable &table);

	// the table of standard items.
	ItemTable &getStandardTable()
		{
		return this->m_standardTable;
		}

	// initialize the commands
	bool addCommands();

        // implement the dump command
        static cCommandStream::CommandStatus doDump(
                        cCommandStream *pThis,
                        void *pContext,
                        int argc,
                        char **argv
                        );

        // implement the reset command
        static cCommandStream::CommandStatus doReset(
                        cCommandStream *pThis,
                        void *pContext,
                        int argc,
                        char **argv
                        );

        void saveField(
                cFramStorage::StandardKeys uKey,
                const uint8_t *pValue,
                size_t nValue
                );

        template <typename T>
        void saveField(
                cFramStorage::StandardKeys uKey,
                const T &field
                )
                {
                this->saveField(uKey, (const uint8_t *)&field, sizeof(field));
                };

        bool getField(
                cFramStorage::StandardKeys uKey,
                uint8_t *pValue,
                size_t nValue
                );

        template <typename T>
        bool getField(
                cFramStorage::StandardKeys uKey,
                T &field
                )
                {
                return this->getField(uKey, (uint8_t *)&field, sizeof(field));
                };


protected:
        // record the "ready status"
        bool                    m_fReady;

        // to save storage, we make these parallel arrays

        // saved offset of objects. If not valid, will be kInvalidOffset.
        cFramStorage::Offset m_offsetCache[cFramStorage::kMAX];

        // saved versions of objects.
        uint8_t m_uVerCache[cFramStorage::kMAX];

        // offset of first free byte in FRAM
        cFramStorage::Offset m_endOffset;

        // the standard items, using the caches above; first in the list
        // of tables.
        ItemTable m_standardTable
                {
                cFramStorage::skFramGuid,
                cFramStorage::vItemDefs,
                cFramStorage::kMAX,
                m_offsetCache,
                m_uVerCache
                };

        // the registered tables, starting with the standard one.
        ItemTable *m_pTables = &m_standardTable;

        // load the cache from the media.
        void loadCache(void);

        // scan the media and fill the caches: of table pOnly, or of every
        // registered table if pOnly is null.
        void scanObjects(ItemTable *pOnly);

        // forget the cache contents.
        void resetCache(void);

        // set a specific entry.
        void setCacheEntry(cFramStorage::StandardKeys, cFramStorage::Offset, uint8_t uVer);

        cFramStorage::Offset allocate(size_t nBytes);

        bool writeItemData(
                cFramStorage::StandardItem item,
                const uint8_t *pBuffer,
                size_t nBuffer
                );

        // write the data of item iItem of table.
        bool writeItemData(
                ItemTable &table,
                unsigned iItem,
                const uint8_t *pBuffer,
                size_t nBuffer
                );

	};


class cFram::Cursor
	{
public:
        // the basic constructor just fills things in.
	Cursor(cFram *pFram)
                : m_pFram(pFram),
                  m_uKey(cFramStorage::StandardKeys::kMAX),
                  m_offset(cFramStorage::kInvalidOffset)
                {};

        // construct and locate an item of an item table by key
        Cursor(cFram *pFram, ItemTable &table, uint8_t uKey)
                : Cursor{pFram}
                {
                (void) this->locate(table, uKey);
                }

        // construct and locate
        Cursor(cFram *pFram, cFramStorage::StandardItem item);

        // construct and locate using key
        Cursor(cFram *pFram, cFramStorage::StandardKeys uKey)
                : Cursor{pFram, cFramStorage::vItemDefs[uKey]} {};

        // take a located cursor and create an object if needed
        bool create(void);

        // test whether a cursor is bound to an item
        bool isbound() const
                {
                return this->m_pTable != nullptr;
                }

        // test whether a cursor was located
        bool islocated() const
                {
                return this->m_offset != cFramStorage::kInvalidOffset;
                }

        // set up a cursor to match a standard item
	bool locate(const cFramStorage::StandardItem);

        bool locate(cFramStorage::StandardKeys uKey)
                {
                return this->locate(cFramStorage::vItemDefs[uKey]);
                }

        // set up a cursor for the item with key uKey in table. The cursor
        // is bound if the table has that key, and located if the object
        // exists.
        bool locate(ItemTable &table, uint8_t uKey);

        // return the standard item associated with this cursor
        bool getitem(cFramStorage::StandardItem &) const;

        // get the item size associated with this cursor
        size_t getitemsize() const;

        // get a buffer
	bool get(uint8_t *pBuffer, size_t nBuffer);

        // get a uint32_t.
	bool getuint32(uint32_t &v)
		{
		return this->get((uint8_t *)&v, sizeof(v));
		}

        // put a buffer
	bool put(const uint8_t *pBuffer, size_t nBuffer);

        // put a uint32_t
	bool putuint32(uint32_t &v)
		{
		return this->put((const uint8_t *)&v, sizeof(v));
		}

        // parse a value
        bool parsevalue(
                const char *pValue,
                uint8_t *pData,
                size_t nData
                ) const;

        // parse a field
        static bool parsefield(
                const char *pValue,
                uint8_t *pData,
                size_t nData,
                bool isNumber
                );

        size_t formatvalue(
                char *pBuffer,
                size_t nBuffer,
                size_t iBuffer,
                const uint8_t *pData,
                size_t nData
                ) const;

        static size_t formatfield(
                char *pBuffer,
                size_t nBuffer,
                size_t iBuffer,
                const uint8_t *pData,
                size_t nData,
                bool isNum
                );

        cFramStorage::StandardKeys getKey() const
                {
                return cFramStorage::StandardKeys(this->m_uKey);
                }

        /// \brief return cursor's internal value of object size.
	uint16_t getSavedSize() const { return this->m_uSize; }
        /// \brief return cursor's internal value of object key.
	uint8_t getSavedKey() const { return this->m_uKey; }
        /// \brief return cursor's internal value of object version
	uint8_t getSavedVer() const { return this->m_uVer; }
        /// \brief return cursor's internal value of object location
	uint32_t getSavedOffset() const { return this->m_offset; }

private:
        cFramStorage::StandardItem getItem()
                {
                return this->m_pTable->getItem(this->m_iItem);
                };

protected:
	cFram *m_pFram;
	ItemTable *m_pTable = nullptr;	// null: not bound
	uint16_t m_uSize;
	uint8_t m_uKey;
	uint8_t m_uVer;
	uint8_t m_iItem = 0;		// index of the item in m_pTable
	uint32_t m_offset;
	};

}; /* namespace McciCatena */

/**** end of Catena_Fram.h ****/
#endif /* _CATENA_FRAM_H_ */
