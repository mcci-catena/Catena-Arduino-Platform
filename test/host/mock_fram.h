/*

Module:	mock_fram.h

Function:
	cMockFram: a cFram backed by a RAM array, for host tests.

Copyright notice:
	See accompanying LICENSE file.

Author:
	Terry Moore, MCCI Corporation	October 2026

*/

#pragma once

#include "Catena_Fram.h"

#include <cstring>

class cMockFram : public McciCatena::cFram
	{
public:
	static constexpr size_t kSize = 8192;

	// fresh media reads as 0xFF, like erased storage that has never been
	// formatted.
	cMockFram(uint8_t *pMem) : m_pMem(pMem) {}

	static void erase(uint8_t *pMem)
		{
		std::memset(pMem, 0xFF, kSize);
		}

	virtual size_t read(
		McciCatena::cFramStorage::Offset uOffset, uint8_t *pBuffer, size_t nBuffer
		) override
		{
		if (uOffset > kSize || nBuffer > kSize - uOffset)
			return 0;
		std::memcpy(pBuffer, m_pMem + uOffset, nBuffer);
		++m_nReads;
		return nBuffer;
		}

	virtual bool write(
		McciCatena::cFramStorage::Offset uOffset, const uint8_t *pBuffer, size_t nBuffer
		) override
		{
		if (uOffset > kSize || nBuffer > kSize - uOffset)
			return false;
		std::memcpy(m_pMem + uOffset, pBuffer, nBuffer);
		++m_nWrites;
		return true;
		}

	virtual McciCatena::cFramStorage::Offset getsize() const override
		{
		return kSize;
		}

	McciCatena::cFramStorage::Offset getEndOffset() const
		{
		return this->m_endOffset;
		}

	unsigned m_nReads = 0;
	unsigned m_nWrites = 0;

private:
	uint8_t *m_pMem;
	};
