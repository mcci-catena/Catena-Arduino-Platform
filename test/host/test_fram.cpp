/*

Module:	test_fram.cpp

Function:
	Host tests for cFram and cFramStorage, against a mock FRAM backed
	by a RAM array.

Copyright notice:
	See accompanying LICENSE file.

Author:
	Terry Moore, MCCI Corporation	October 2026

*/

#include "mock_fram.h"

#include <cstdio>
#include <cstring>

using namespace McciCatena;

static unsigned gnFail;
static unsigned gnCheck;

#define CHECK(e)							\
	do	{							\
		++gnCheck;						\
		if (! (e))						\
			{					\
			++gnFail;					\
			std::printf("%s:%d: CHECK(%s) failed\n", __FILE__, __LINE__, #e); \
			}						\
		} while (0)

static uint8_t gMem[cMockFram::kSize];

/****************************************************************************\
|
|	Standard items (today's behavior)
|
\****************************************************************************/

static void testFormat()
	{
	cMockFram::erase(gMem);
	cMockFram fram(gMem);

	CHECK(fram.begin());
	CHECK(! fram.isValid());
	CHECK(fram.initialize());
	CHECK(fram.isValid() && fram.isReady());

	// the header holds the end pointer, twice, after a 24-byte object header.
	CHECK(fram.getEndOffset() == 24 + 2 * 4);

	// a second instance on the same media finds it valid.
	cMockFram fram2(gMem);
	CHECK(fram2.begin());
	CHECK(fram2.isValid());
	CHECK(fram2.getEndOffset() == fram.getEndOffset());
	}

static void testStandardItems()
	{
	cMockFram::erase(gMem);
	cMockFram fram(gMem);

	CHECK(fram.begin() && fram.initialize());

	// not there yet
	cFram::Cursor c1(&fram, cFramStorage::kBootCount);
	CHECK(c1.isbound() == false || c1.islocated() == false);
	uint32_t v = 0x12345678;
	CHECK(! c1.getuint32(v));

	// create, put, get
	cFram::Cursor c2(&fram, cFramStorage::kBootCount);
	CHECK(c2.create());
	CHECK(c2.islocated());
	uint32_t bootCount = 0;
	CHECK(c2.getuint32(bootCount) && bootCount == 0);
	bootCount = 41;
	CHECK(c2.putuint32(bootCount));
	bootCount = 42;
	CHECK(c2.putuint32(bootCount));

	// 24 + 2 x 4 for the header, then 24 + 2 x 4 for the boot count.
	CHECK(fram.getEndOffset() == 64);

	// another instance sees the item and the last value.
	cMockFram fram2(gMem);
	CHECK(fram2.begin() && fram2.isValid());
	uint32_t got = 0;
	cFram::Cursor c3(&fram2, cFramStorage::kBootCount);
	CHECK(c3.islocated() && c3.getuint32(got) && got == 42);

	// saveField and getField
	uint32_t flags = 0x80000001;
	fram2.saveField(cFramStorage::kOperatingFlags, flags);
	uint32_t flags2 = 0;
	CHECK(fram2.getField(cFramStorage::kOperatingFlags, flags2) && flags2 == 0x80000001);
	}

static void testReset()
	{
	cMockFram::erase(gMem);
	cMockFram fram(gMem);

	CHECK(fram.begin() && fram.initialize());
	uint32_t v = 7;
	fram.saveField(cFramStorage::kBootCount, v);
	CHECK(fram.reset());
	uint32_t got = 0;
	CHECK(! fram.getField(cFramStorage::kBootCount, got));
	CHECK(fram.getEndOffset() == 32);
	}

int main()
	{
	testFormat();
	testStandardItems();
	testReset();

	std::printf("%u checks, %u failed\n", gnCheck, gnFail);
	return gnFail == 0 ? 0 : 1;
	}
