/* CatenaBase.h	Sun Mar 19 2017 15:00:21 tmm */

/*

Module:  CatenaBase.h

Function:
	class CatenaBase interfaces.

Version:
	V0.5.0	Sun Mar 19 2017 15:00:21 tmm	Edit level 4

Copyright notice:
	This file copyright (C) 2016-2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Tue Oct 25 2016 01:58:25  tmm
	Module created.

   0.3.0  Thu Oct 27 2016 22:46:30  tmm
	Change buffer types away from array, types are just not intuitive.

   0.5.0  Sun Mar 19 2017 15:00:21  tmm
	Major update for comamand support, etc.

*/

#ifndef _CATENABASE_H_		/* prevent multiple includes */
#define _CATENABASE_H_

#pragma once

#ifndef _CATENA_POLLABLEINTERFACE_H_
# include <Catena_PollableInterface.h>
#endif

#include <stdint.h>
#include <Arduino.h>

#ifndef _CATENABASE_TYPES_H_
# include "CatenaBase_types.h"
#endif

#ifndef _CATENA_STREAMLINECOLLECTOR_H_
# include "Catena_StreamLineCollector.h"
#endif

#ifndef _CATENA_COMMANDSTREAM_H_
# include "Catena_CommandStream.h"
#endif

class CatenaBase
	{
public:
	CatenaBase();
        virtual ~CatenaBase() {};

	/* an EUI64 */
	struct EUI64_buffer_t {
		uint8_t	b[64/8];
		};
	struct EUI64_string_t
		{
		char	c[sizeof(EUI64_buffer_t) * 3 + 1];
		};

	enum OPERATING_FLAGS : uint32_t
		{
		fUnattended = 1 << 0,
                fManufacturingTest = 1 << 1,
		};

	void SafePrintf(
		const char *fmt, ...
		);

	virtual const EUI64_buffer_t *GetSysEUI(void)
		{
		return &this->m_SysEUI;
		}

        virtual bool begin();

        // poll the engine
        void poll(void);
        void registerObject(McciCatena::cPollableObject *pObject);

	// command handling
	void addCommands(McciCatena::cCommandStream::cDispatch &, void *);

/****************************************************************************\
|
|	The global CatenaBase pointer.
|
|	Code can't readily reference a global (like "gCatena") that is of
|	a more specialized type than the one they know. So instead, we have
|	a global pointer to the Catena, taken as a pointer to the most
|	general object.  It's initialzed by the constructor.
|
\****************************************************************************/

	static CatenaBase *pCatenaBase;

protected:
	virtual void registerCommands(void);

	// data objects
protected:
	EUI64_buffer_t m_SysEUI;
        McciCatena::cPollingEngine m_PollingEngine;

	// the line collector
	McciCatena::cStreamLineCollector	m_Collector;

        // the command processor
        McciCatena::cCommandStream              m_CommandStream;
	};


namespace McciCatena {

extern CatenaBase *gpCatenaBase;

}; // end namespace McciCatena


/**** end of CatenaBase.h ****/
#endif /* _CATENABASE_H_ */
