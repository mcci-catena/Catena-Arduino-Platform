/* Catena_Log.h	Sun Mar 19 2017 01:40:10 tmm */

/*

Module:  Catena_Log.h

Function:
	McciCatena::cLog and associated functions.

Version:
	V0.5.0	Sun Mar 19 2017 01:40:10 tmm	Edit level 1

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
   0.5.0  Sun Mar 19 2017 01:40:10  tmm
	Module created.

*/

#ifndef _CATENA_LOG_H_		/* prevent multiple includes */
#define _CATENA_LOG_H_

#include <cstdint>

#pragma once

namespace McciCatena {

class cLog
	{
public:
	enum DebugFlags : uint32_t
		{
		kAlways		= 0u,
		kFatal		= 0xFFFFFFFFu,

                kBug            = 0x00000001u,
		kError		= 0x00000002u,
		kWarning	= 0x00000004u,
		kTrace		= 0x00000008u,
		kInfo		= 0x00000010u,
		};

	cLog(DebugFlags flags = DebugFlags(0)) 
                : m_uDebugFlags(flags) 
                {};

	// neither copyable nor movable.
	cLog(const cLog&) = delete;
	cLog& operator=(const cLog&) = delete;
	cLog(const cLog&&) = delete;
	cLog& operator=(const cLog&&) = delete;

	// initialize
	bool begin(DebugFlags uDebugFlags = DebugFlags(kError | kBug))
		{
		this->m_uDebugFlags = uDebugFlags;
		}

	// check whether flags are enabled ... inline for speed
	bool isenabled(DebugFlags uDebugFlags) const
		{
		return ((uDebugFlags & this->m_uDebugFlags) == 0 &&
		         uDebugFlags != kAlways && 
			 uDebugFlags != kFatal);
		}

	// log	
	void printf(
		DebugFlags uDebugFlags,
		const char *pFmt,
		...
		) __attribute__((__format__(__printf__, 3, 4)));
		/* format counts start with 2 for non-static C++ member fns */

private:
	DebugFlags m_uDebugFlags;
	};

extern cLog gLog;

}; // namespace McciCatena

/**** end of Catena_Log.h ****/
#endif /* _CATENA_LOG_H_ */
