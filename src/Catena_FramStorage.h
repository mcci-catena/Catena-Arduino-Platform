/* Catena_FramStorage.h	Sun Mar 19 2017 17:34:08 tmm */

/*

Module:  Catena_FramStorage.h

Function:
	class McciCatena::FramStorage

Version:
	V0.5.0	Sun Mar 19 2017 17:34:08 tmm	Edit level 1

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
   0.5.0  Sun Mar 19 2017 17:34:08  tmm
	Module created.

*/

#ifndef _CATENA_FRAMSTORAGE_H_		/* prevent multiple includes */
#define _CATENA_FRAMSTORAGE_H_

#include <stdint.h>

#ifndef _MCCIADK_GUID_H_
# include "mcciadk_guid.h"
#endif

namespace McciCatena {

class FramStorage
	{
        struct ObjectRaw
                {
                // the size and discriminator
                uint32_t uSizeKey;

                // the GUID for this object.  The first object is always the
                // "header GUID"
                MCCIADK_GUID_WIRE	Guid;

                // a discriminator, allowing for 256 different subvalues
                uint8_t		uKey;

                // a trick used for 30 years for storing data reliably: three values
                // that (in this case) select one location or the other. Written to
                // the other value sequentially, and when read, if v1 == v2, we
                // believe it, otherwise we think that either v1 or v2 was corrupted
                // during write, and instead use v3.

                uint8_t		uVer1;	// first image of discriminator
                uint8_t		uVer2;	// second image of discriminator
                uint8_t		uVer3;	// third image of discriminator
                };
        class Object;
	static constexpr size_t kObjectQuantum = sizeof(uint32_t);
	static constexpr size_t kObjectStandardSize = sizeof(ObjectRaw);
	static constexpr size_t kObjectStandardClicks =
				(kObjectStandardSize + kObjectQuantum - 1) /
					kObjectQuantum;
	};


struct FramStorage::Object : public FramStorage::ObjectRaw
	{
	enum : uint32_t 
		{
		SIZE_MASK = 0xFFFFu,
		NONSTD_MASK = 0x80000000u,
		DATASIZE_MASK = 0x7FFF0000u,
		};

	static uint32_t constexpr getField(uint32_t v, uint32_t mask)
		{
		return (v & mask) / (mask & (~mask + 1u));
		}
	static uint32_t constexpr setField(
		uint32_t oldv, uint32_t fv, uint32_t mask
		)
		{
		return (oldv & ~mask) +
		       (fv * (mask & (~mask + 1u)));
		}

	static uint32_t constexpr uSizeKey_GetSize(uint32_t uSizeKey)
		{
		return (uSizeKey & SIZE_MASK) * kObjectQuantum;
		}

	static size_t constexpr getClicks(size_t nBytes)
		{
		return (nBytes + kObjectQuantum - 1) / kObjectQuantum;
		}
        static constexpr bool uSizeKey_SetClicks(uint32_t uSizeKey, uint32_t v)
                {
                return (1 <= v && v <= SIZE_MASK) 
                        ? (uSizeKey & ~SIZE_MASK) | v
                        : 0;
                }
	static uint32_t constexpr uSizeKey_SetSize(
		uint32_t uSizeKey,
		size_t sizeInBytes
		)
		{
                return uSizeKey_SetClicks(
                        uSizeKey, getClicks(sizeInBytes)
                        );
		}

	bool getObjectSize(void) const
		{
		return uSizeKey_GetSize(this->uSizeKey);
		}
	bool getObjectClicks(void) const
		{
		return this->uSizeKey & SIZE_MASK;
		}
	bool setObjectSize(uint32_t sizeInBytes)
		{
		uint32_t const uSizeKey = uSizeKey_SetSize(
						this->uSizeKey,
						sizeInBytes
						);
		if (uSizeKey != 0)
			{
			this->uSizeKey = uSizeKey;
			return true;
			}
		else
			{
			return false;
			}
		}
	bool isStandard() const
		{
		return (this->uSizeKey & NONSTD_MASK) == 0;
		}
	uint16_t getDataSize() const
		{
		if (! this->isStandard())
			return 0;

		return (uint16_t) getField(this->uSizeKey, DATASIZE_MASK);
		}

	uint16_t static neededClicks(size_t nBytes, bool fReplicated)
		{
		size_t nClicks;
		
		nClicks = getClicks(nBytes);
		if (fReplicated)
			nClicks *= 2;

		if (nClicks > SIZE_MASK - kObjectStandardClicks)
			return 0;
		else
			return (uint16_t) nClicks + kObjectStandardClicks;
		}

	bool validDataSize(size_t nBytes)
		{
		if (! this->isStandard())
			return false;
		
		uint16_t const n = neededClicks(nBytes, false);
		return (n <= this->getObjectClicks());
		}
	
	bool isReplicated() const
		{
		if (! this->isStandard())
			return false;

		const uint16_t dataSize = this->getDataSize();
		const uint16_t dataClicks = (uint16_t) getClicks(dataSize);

		return (2 * dataClicks + kObjectStandardClicks <=
				this->getObjectClicks());
		}
	unsigned nReplicants() const
		{
		if (! this->isStandard())
			return 0;

		const uint16_t dataSize = this->getDataSize();
		const uint16_t dataClicks = (uint16_t) getClicks(dataSize);

		if (dataClicks == 0)
			return 0;

		return (this->getObjectClicks() - kObjectStandardClicks) /
				dataClicks;
		}

	// offset to the i-th repliant from the base of this object.
	// zero means error.
	unsigned offsetToReplicant(unsigned i) const
		{
		if (! this->isStandard())
			return 0;

		const uint16_t dataSize = this->getDataSize();
		const uint16_t dataClicks = (uint16_t) getClicks(dataSize);

		return sizeof(this) + i * dataClicks * kObjectQuantum;
		}


	// get current discriminator; use voting algorigthm
	uint8_t getCurrent() const
		{
		uint8_t const v1 = this->uVer1;

		if (v1 == this->uVer2)
			return v1;
		else
			return this->uVer3;
		}

	// set discriminator (only in local image, of cousre)
	uint8_t getCurrent(uint8_t v)
		{
		this->uVer1 = v;
		this->uVer2 = v;
		this->uVer3 = v;
		}
	};

}; // namespace McciCatena

/**** end of Catena_FramStorage.h ****/
#endif /* _CATENA_FRAMSTORAGE_H_ */
