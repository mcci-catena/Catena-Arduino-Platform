/* Catena_StreamLineCollector.h	Sat Mar 18 2017 15:15:53 tmm */

/*

Module:  Catena_StreamLineCollector.h

Function:
	McciCatena::cStreamLineCollector

Version:
	V0.5.0	Sat Mar 18 2017 15:15:53 tmm	Edit level 1

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
   0.5.0  Sat Mar 18 2017 15:15:53  tmm
	Module created.

*/

#ifndef _CATENA_STREAMLINECOLLECTOR_H_		/* prevent multiple includes */
#define _CATENA_STREAMLINECOLLECTOR_H_

#pragma once

// this is a wrapper for <functional>
#ifndef _CATENA_FUNCTIONAL_H_
# include <Catena_functional.h>
#endif

#ifndef _CATENA_POLLABLEINTERFACE_H_
# include <Catena_PollableInterface.h>
#endif

#include <cstdarg>

// now, back to reality
#include "Stream.h"

/****************************************************************************\
|
|	The definitions
|
\****************************************************************************/

namespace McciCatena {

class cStreamLineCollector : public cPollableObject
	{
public:
	cStreamLineCollector() {};
	virtual ~cStreamLineCollector() {};

	// neither copyable nor movable
	cStreamLineCollector(const cStreamLineCollector&) = delete;
	cStreamLineCollector& operator=(const cStreamLineCollector&) = delete;
	cStreamLineCollector(const cStreamLineCollector&&) = delete;
	cStreamLineCollector& operator=(const cStreamLineCollector&&) = delete;

	enum : uint8_t
		{
		kEol = '\n',
                kCr = '\r'
		};

	enum ErrorCode : uint32_t
		{
		kSuccess = 0,
		kOverrun = 1,
		kBusy = 2,
                kIoError = 3,
		};

	class cStreamReady
		{
	public:
		cStreamReady() {};
		virtual bool isReady() const
			{
			return false;
			};
		};

	// the callback function:
	typedef void (ReadCompleteCbFn)(
				void* pCtx, 
				ErrorCode uStatus, 
				uint8_t *pBuffer,
				size_t nBuffer
				);
	
public:
	// the poll method (makes this pollable)
	virtual void poll(void) override;
	
	// the setup function. The enabler function is used for streams like
	// USB Serial, which hang if you talk to them when they're not plugged
	// in...
	void begin(
		Stream *pStream,			// the stream to poll.
		cStreamReady *pReady = nullptr		// optional enabler.
		);

	void readAsync(
		ReadCompleteCbFn *pFn,	// the function to call on completion,
		void *pDoneCtx,		// context to pass to the function
		uint8_t *pBuffer,	// the buffer to fill
		size_t nBuffer		// the buffer size
		);

        void vprintf(const char *pFmt, std::va_list ap);

private:
	// the stream we're working with
	Stream			*m_pStream = nullptr;
        bool                    m_fLastWasCr = false;
	cStreamReady		*m_pStreamReady = nullptr;
	
	// the callback function
	ReadCompleteCbFn	*m_pReadCompleteCbFn = nullptr;
	// context for callback.
	void			*m_pReadCompleteCbCtx = nullptr;

	// the buffer
	uint8_t			*m_pBuffer = nullptr;
	// the buffer size
	size_t			m_nBuffer = 0;

	// the current position in the buffer
	uint8_t			*m_pInsert = nullptr;

	void readComplete(ErrorCode uStatus);
	bool isBusy(void) const
		{
		return this->m_pReadCompleteCbFn != nullptr;
		}
	};

}; // namespace McciCatena

/**** end of Catena_StreamLineCollector.h ****/
#endif /* _CATENA_STREAMLINECOLLECTOR_H_ */
