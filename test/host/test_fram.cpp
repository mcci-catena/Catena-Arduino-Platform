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

	// the same cursor reads back what it last wrote (before #379, get()
	// after put() read the other replicant).
	uint32_t again = 0;
	CHECK(c2.getuint32(again) && again == 42);

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

/****************************************************************************\
|
|	Application item tables (#379)
|
\****************************************************************************/

// two application GUIDs
static const MCCIADK_GUID_WIRE kAppGuid =
	MCCIADK_GUID_GEN_INIT(WIRE, 0x0b6c3e4a, 0x5d21, 0x4f0e, 0x9a, 0x61, 0x2c, 0x7d, 0x13, 0xe8, 0x40, 0x55);
static const MCCIADK_GUID_WIRE kOtherGuid =
	MCCIADK_GUID_GEN_INIT(WIRE, 0x7f1d2b90, 0x0c3a, 0x4e6b, 0x8d, 0x24, 0x95, 0x0a, 0x6e, 0x31, 0xb7, 0xc2);

struct Params_t
	{
	uint8_t	b[61];
	};

// key 0: a 61-byte record, replicated; key 1: a counter, not replicated;
// key 15: same number as the standard kOperatingFlags.
static const cFramStorage::StandardItem kAppItems[] =
	{
	cFramStorage::StandardItem(0, sizeof(Params_t), /* number */ false),
	cFramStorage::StandardItem(1, sizeof(uint32_t), /* number */ true, /* replicated */ false),
	cFramStorage::StandardItem(15, sizeof(uint32_t), /* number */ true),
	};

static const cFramStorage::StandardItem kOtherItems[] =
	{
	cFramStorage::StandardItem(0, sizeof(uint32_t), /* number */ true),
	};

static void testAppTableCreate()
	{
	cMockFram::erase(gMem);
	cMockFram fram(gMem);
	cFram::AppItemTable<3> table(kAppGuid, kAppItems);

	CHECK(fram.begin());
	// register before the store is valid: nothing to scan yet
	CHECK(fram.registerItemTable(table));
	CHECK(fram.initialize());

	cFram::Cursor c(&fram, table, 0);
	CHECK(c.isbound() && ! c.islocated());
	CHECK(c.create() && c.islocated());

	Params_t p;
	for (unsigned i = 0; i < sizeof(p.b); ++i)
		p.b[i] = uint8_t(i + 1);
	CHECK(c.put(p.b, sizeof(p.b)));
	Params_t q {};
	CHECK(c.get(q.b, sizeof(q.b)) && std::memcmp(p.b, q.b, sizeof(p.b)) == 0);

	// a second put goes to the other replicant; get sees the new value
	p.b[0] = 0xAA;
	CHECK(c.put(p.b, sizeof(p.b)));
	CHECK(c.get(q.b, sizeof(q.b)) && q.b[0] == 0xAA);

	// the wrong size is refused
	CHECK(! c.get(q.b, 4));

	// a key outside the table doesn't bind
	cFram::Cursor bad(&fram, table, 3);
	CHECK(! bad.isbound() && ! bad.create());

	// registering twice is harmless
	CHECK(fram.registerItemTable(table));
	cFram::Cursor c2(&fram, table, 0);
	CHECK(c2.islocated() && c2.get(q.b, sizeof(q.b)) && q.b[0] == 0xAA);
	}

static void testAppTableRescan()
	{
	cMockFram::erase(gMem);
	{
	cMockFram fram(gMem);
	cFram::AppItemTable<3> table(kAppGuid, kAppItems);
	CHECK(fram.begin() && fram.initialize() && fram.registerItemTable(table));

	cFram::Cursor c0(&fram, table, 0);
	Params_t p {};
	p.b[60] = 0x5A;
	CHECK(c0.create() && c0.put(p.b, sizeof(p.b)));

	cFram::Cursor c1(&fram, table, 1);
	uint32_t n = 1234;
	CHECK(c1.create() && c1.putuint32(n));

	// same key number as a standard item, different GUID: no collision
	cFram::Cursor c15(&fram, table, 15);
	uint32_t appFlags = 0xA5A5A5A5;
	CHECK(c15.create() && c15.putuint32(appFlags));
	uint32_t stdFlags = 0x00000001;
	fram.saveField(cFramStorage::kOperatingFlags, stdFlags);
	}

	// a new instance, as after a reset: register after the store is
	// valid, and the scan finds the items.
	cMockFram fram(gMem);
	CHECK(fram.begin() && fram.isValid());
	cFram::AppItemTable<3> table(kAppGuid, kAppItems);
	cFram::Cursor before(&fram, table, 0);
	CHECK(! before.islocated());		// not registered yet
	CHECK(fram.registerItemTable(table));

	cFram::Cursor c0(&fram, table, 0);
	Params_t q {};
	CHECK(c0.islocated() && c0.get(q.b, sizeof(q.b)) && q.b[60] == 0x5A);
	cFram::Cursor c1(&fram, table, 1);
	uint32_t n = 0;
	CHECK(c1.islocated() && c1.getuint32(n) && n == 1234);
	cFram::Cursor c15(&fram, table, 15);
	uint32_t appFlags = 0;
	CHECK(c15.getuint32(appFlags) && appFlags == 0xA5A5A5A5);
	uint32_t stdFlags = 0;
	CHECK(fram.getField(cFramStorage::kOperatingFlags, stdFlags) && stdFlags == 1);

	// another application's table sees none of these
	cFram::AppItemTable<1> other(kOtherGuid, kOtherItems);
	CHECK(fram.registerItemTable(other));
	cFram::Cursor o(&fram, other, 0);
	CHECK(o.isbound() && ! o.islocated());
	CHECK(o.create());
	uint32_t ov = 99;
	CHECK(o.putuint32(ov));
	uint32_t got = 0;
	CHECK(c1.getuint32(got) && got == 1234);
	}

static void testAppTableUnregistered()
	{
	// application objects in the store don't disturb an instance that
	// never registers their table.
	cMockFram::erase(gMem);
	{
	cMockFram fram(gMem);
	cFram::AppItemTable<3> table(kAppGuid, kAppItems);
	CHECK(fram.begin() && fram.initialize() && fram.registerItemTable(table));
	cFram::Cursor c(&fram, table, 1);
	uint32_t n = 5;
	CHECK(c.create() && c.putuint32(n));
	uint32_t bc = 77;
	fram.saveField(cFramStorage::kBootCount, bc);
	}
	cMockFram fram(gMem);
	CHECK(fram.begin() && fram.isValid());
	uint32_t bc = 0;
	CHECK(fram.getField(cFramStorage::kBootCount, bc) && bc == 77);
	// new standard items still append after the application objects
	uint32_t flags = 3;
	fram.saveField(cFramStorage::kOperatingFlags, flags);
	uint32_t got = 0;
	CHECK(fram.getField(cFramStorage::kOperatingFlags, got) && got == 3);
	}

static void testAppTableSizeChange()
	{
	// an item whose stored size differs from the table's definition is
	// not used; create() appends a new object of the right size.
	cMockFram::erase(gMem);
	{
	cMockFram fram(gMem);
	cFram::AppItemTable<1> other(kOtherGuid, kOtherItems);	// key 0: 4 bytes
	CHECK(fram.begin() && fram.initialize() && fram.registerItemTable(other));
	cFram::Cursor c(&fram, other, 0);
	uint32_t v = 0x11223344;
	CHECK(c.create() && c.putuint32(v));
	}
	static const cFramStorage::StandardItem kBigger[] =
		{
		cFramStorage::StandardItem(0, 8, /* number */ false),
		};
	cMockFram fram(gMem);
	cFram::AppItemTable<1> bigger(kOtherGuid, kBigger);
	CHECK(fram.begin() && fram.isValid() && fram.registerItemTable(bigger));
	cFram::Cursor c(&fram, bigger, 0);
	CHECK(! c.islocated());
	const auto end = fram.getEndOffset();
	CHECK(c.create() && c.islocated() && fram.getEndOffset() > end);
	uint8_t b[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	CHECK(c.put(b, sizeof(b)));
	uint8_t r[8] {};
	CHECK(c.get(r, sizeof(r)) && r[7] == 8);
	}

static void testAppTableReset()
	{
	cMockFram::erase(gMem);
	cMockFram fram(gMem);
	cFram::AppItemTable<3> table(kAppGuid, kAppItems);
	CHECK(fram.begin() && fram.initialize() && fram.registerItemTable(table));
	cFram::Cursor c(&fram, table, 1);
	uint32_t n = 9;
	CHECK(c.create() && c.putuint32(n));
	CHECK(fram.reset());
	// the table stays registered, but its items are gone
	cFram::Cursor c2(&fram, table, 1);
	CHECK(c2.isbound() && ! c2.islocated());
	CHECK(c2.create());
	uint32_t got = 1;
	CHECK(c2.getuint32(got) && got == 0);
	}

int main()
	{
	testFormat();
	testStandardItems();
	testReset();
	testAppTableCreate();
	testAppTableRescan();
	testAppTableUnregistered();
	testAppTableSizeChange();
	testAppTableReset();

	std::printf("%u checks, %u failed\n", gnCheck, gnFail);
	return gnFail == 0 ? 0 : 1;
	}
