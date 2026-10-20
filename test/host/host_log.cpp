/*

Module:	host_log.cpp

Function:
	Host-test stand-in for gLog: messages go to stderr when
	HOST_TEST_VERBOSE is set in the environment.

Copyright notice:
	See accompanying LICENSE file.

Author:
	Terry Moore, MCCI Corporation	October 2026

*/

#include "Catena_Log.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

McciCatena::cLog McciCatena::gLog;

void McciCatena::cLog::printf(DebugFlags uDebugFlags, const char *pFmt, ...)
	{
	(void) uDebugFlags;
	if (std::getenv("HOST_TEST_VERBOSE") == nullptr)
		return;

	va_list ap;
	va_start(ap, pFmt);
	std::vfprintf(stderr, pFmt, ap);
	va_end(ap);
	}
