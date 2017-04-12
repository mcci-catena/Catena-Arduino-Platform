/* Catena_StreamLineCollector.cpp	Sat Mar 18 2017 16:51:46 tmm */

/*

Module:  Catena_StreamLineCollector.cpp

Function:
	Implementation of McciCatena::cStreamLineCollector.

Version:
	V0.5.0	Sat Mar 18 2017 16:51:46 tmm	Edit level 1

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
   0.5.0  Sat Mar 18 2017 16:51:46  tmm
	Module created.

*/

#include <Catena_StreamLineCollector.h>

#include "Catena_Log.h"
#include <cstdio>

using namespace McciCatena;

/*

Name:	cStreamLineCollector::begin()

Index:	Type:	cStreamReady::StreamReadyFn

Function:
	Initialize an instance of cStreamLineCollector.

Definition:
	typedef std::function<bool(void)> cStreamLineCollector::StreamReadyFn;

	void cStreamLineCollector::begin(
		Stream *pStream,
		cStreamLineCollector::StreamReadyFn *pReadyFn
		);

Description:
	This function should be called before starting to poll the instance.

	When setting up with Streams that support operator !, you should
	initialize like this:

		static const cStreamLineCollector::StreamReadyFn myReady =
			[Serial&]{ return Serial->dtr(); }

	Then pass a pointer to myReady as pReadyFn:
		collector.begin(&Serial, &myReady);

Returns:
	No explicit result.

*/

void
McciCatena::cStreamLineCollector::begin(
	Stream *pStream,
	cStreamReady *pStreamReady
	)
	{
	this->m_pStream = pStream;

	this->m_pStreamReady = pStreamReady;
        this->m_fLastWasCr = false;
	}

/*

Name:	cStreamLineCollector::readAsync()

Function:
	Launch an asynchronous read.

Definition:
	typedef void (cStreamLineCollector::ReadCompleteCbFn)(
				void *pDoneCtx,
				cStreamLineCollector::ErrorCode Status,
				uint8_t *pBuffer,
				size_t nBuffer
				);

	void cStreamLineCollector::readAsync(
		ReadCompleteCbFn *pFn,	// the function to call on completion,
		void *pDoneCtx,		// context to pass to the function
		uint8_t *pBuffer,	// the buffer to fill
		size_t nBuffer		// the buffer size
		);

Description:
	This call begins the process of reading a line (terminated by \n)
	from the configured stream. When the read completes, the function
	specified by pFn will be called with the indicated parameters.

Returns:
	No explicit result.

	When the callback is called, the ErrorCode will be set to one of the 
	following values:
	
		kSuccess for succes.
		kBusy if a read was issued while a read was already pending.
		kOverrun if the read completed with a buffer overrun.

Notes:
	Buffer overruns are handled by just looking for the '\n', and then
	issuing an overrun completion. The lost data is lost.

*/

void 
McciCatena::cStreamLineCollector::readAsync(
	ReadCompleteCbFn *pFn,	// the function to call on completion,
	void *pDoneCtx,		// context to pass to the function
	uint8_t *pBuffer,	// the buffer to fill
	size_t nBuffer		// the buffer size
	)
	{
        static const char FUNCTION[] = "McciCatena::cStreamLineCollector::readAsync";

	if (pFn == nullptr)
                {
                // Log.printf(Log.kAlways, "%s: pFn == null\n", FUNCTION);
		return;
                }

	if (this->isBusy())
                {
		(*pFn)(pDoneCtx, ErrorCode::kBusy, pBuffer, nBuffer);
                // Log.printf(Log.kAlways, "%s: busy\n", FUNCTION);
                }

        // Log.printf(Log.kAlways, "%s: launch read\n", FUNCTION);

	this->m_pReadCompleteCbFn = pFn;
	this->m_pReadCompleteCbCtx = pDoneCtx;
	this->m_pBuffer = pBuffer;
	this->m_nBuffer = nBuffer;
	this->m_pInsert = pBuffer;
	}

void
McciCatena::cStreamLineCollector::poll(
	void
	)
	{
        static const char FUNCTION[] = "McciCatena::cStreamLineCollector::poll";

	// if the stream is not set, give up.
	if (this->m_pStream == nullptr)
		return;

	// if there's no request, stop.
	if (! this->isBusy())
		return;

	// if the stream is not connected, stop.
	if (this->m_pStreamReady != nullptr &&
	    ! this->m_pStreamReady->isReady())
                {
                // Log.printf(Log.kAlways, "%s: stream not ready\n", FUNCTION);
		return;
                }
	
	// if there is no data available on the stream, give up
	size_t const nAvail = this->m_pStream->available();

	if (nAvail == 0)
		return;

	// consume data. If there's room in the buffer, read to a end-char.
	// if buffer is full, discard data until we get to an end-char, then
	// report an error.
	size_t nRemaining;
	nRemaining = this->m_nBuffer - (this->m_pInsert - this->m_pBuffer);

	if (nRemaining > nAvail)
		nRemaining = nAvail;

	// if nRemaining is 0, then the buffer is full...
	if (nRemaining == 0)
		{
		// skip to end-char
		while (this->m_pStream->available())
			{
			int const c = this->m_pStream->read();
			if (c < 0)
				break;

			if (c == kEol)
				{
				this->readComplete(ErrorCode::kOverrun);
				break;
				}
			}
		}
	// otherwise we can issue a ReadUntil
	else
		{
                // we can't use readBytesUntil because it doesn't return
                // the terminator. read manually instead.

                // number read
		size_t nRead;
                ErrorCode status;
                
                status = ErrorCode::kBusy;

                for (nRead = 0; status == ErrorCode::kBusy && nRead < nRemaining; ++nRead)
                        {
                        int const c = this->m_pStream->read();
                        if (c < 0)
                                {
                                status = ErrorCode::kIoError;
                                break;
                                }

                        *this->m_pInsert++ = (uint8_t) c;

                        // activate on cr or lf; 
                        // silently ignore empty lines, so cr/lf
                        // is still effectively only one line ending.
                        if (c == kCr)
                                {
                                this->m_pInsert[-1] = '\n';
                                this->m_fLastWasCr = true;
                                status = ErrorCode::kSuccess;
                                }
                        else if (c == kEol)
                                {
                                if (this->m_fLastWasCr)
                                        --this->m_pInsert;
                                else
                                        status = ErrorCode::kSuccess;

                                this->m_fLastWasCr = false;
                                }
                        else
                                this->m_fLastWasCr = false;
                        }

                // exited loop; complete if possible.
                if (status != ErrorCode::kBusy)
			this->readComplete(status);
		}
	}

void
McciCatena::cStreamLineCollector::readComplete(
	ErrorCode uStatus
	)
	{
	ReadCompleteCbFn * const pDoneFn = this->m_pReadCompleteCbFn;

        if (pDoneFn)
                {
                this->m_pReadCompleteCbFn = nullptr;

                (*pDoneFn)(
                        this->m_pReadCompleteCbCtx, 
                        uStatus, 
                        this->m_pBuffer, 
                        this->m_pInsert - this->m_pBuffer
                        );
                }
	}

void
McciCatena::cStreamLineCollector::vprintf(const char *pFmt, std::va_list ap)
        {
        char buf[128];

	if (this->m_pStreamReady != nullptr &&
	    ! this->m_pStreamReady->isReady())
                {
                // Log.printf(Log.kAlways, "%s: stream not ready\n", FUNCTION);
		return;
                }

        vsnprintf(buf, sizeof(buf) - 1, pFmt, ap);
        buf[sizeof(buf) - 1] = '\0';
        this->m_pStream->print(buf);
        }
