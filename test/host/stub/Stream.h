/*

Module:	Stream.h

Function:
	Host-test stand-in for the Arduino Stream class.

Copyright notice:
	See accompanying LICENSE file.

Author:
	Terry Moore, MCCI Corporation	October 2026

*/

#pragma once

#include <cstddef>
#include <cstdint>

class Stream
	{
public:
	virtual ~Stream() {}
	virtual int available() { return 0; }
	virtual int read() { return -1; }
	virtual size_t write(uint8_t) { return 1; }
	virtual size_t write(const uint8_t *, size_t n) { return n; }
	};
