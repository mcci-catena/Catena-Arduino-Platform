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
	cLog() {};

	// neither copyable nor movable.
	cLog(const cLog&) = delete;
	cLog& operator=(const cLog&) = delete;
	cLog(const cLog&&) = delete;
	cLog& operator=(const cLog&&) = delete;

	enum DebugFlags : uint32_t
		{
		kAlways = 0,
		};

	// initialize
	bool begin(DebugFlags uDebugFlags = DebugFlags(0))
		{
		this->m_uDebugFlags = uDebugFlags;
		}

	// log	
	void printf(
		DebugFlags uDebugFlags,
		const char *pFmt,
		...
		) __attribute__((__format__(__printf__, 3, 4)));
		/* format counts start with 2 for non-static C++ member fns */

private:
	DebugFlags m_uDebugFlags = DebugFlags(0);
	};

extern cLog Log;

}; // namespace McciCatena

/**** end of Catena_Log.h ****/
#endif /* _CATENA_LOG_H_ */
