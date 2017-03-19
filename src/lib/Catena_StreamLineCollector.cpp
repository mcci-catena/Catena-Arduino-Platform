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
			[Serial&]{ return !!Serial; }

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
	}

/*

Name:	cStreamLineCollector::readAsync()

Function:
	Launch an asynchronous read.

Definition:
	typedef std::function<void(
				void *pDoneCtx,
				cStreamLineCollector::ErrorCode Status,
				uint8_t *pBuffer,
				size_t nBuffer
				)>
		cStreamLineCollector::ReadCompleteCbFn;

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
	if (pFn == nullptr || ! *pFn)
		return;

	if (this->isBusy())
		(*pFn)(pDoneCtx, ErrorCode::kBusy, pBuffer, nBuffer);

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
	// if the stream is not set, give up.
	if (this->m_pStream == nullptr)
		return;

	// if there's no request, stop.
	if (! this->isBusy())
		return;

	// if the stream is not connected, stop.
	if (this->m_pStreamReady!= nullptr &&
	    ! this->m_pStreamReady->isReady())
		return;
	
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
		size_t const nRead = this->m_pStream->readBytesUntil(
						kEol,
						this->m_pInsert,
						nRemaining
						);

		// advance pointers
		this->m_pInsert += nRead;

		if (nRead > 0 && this->m_pInsert[-1] == kEol)
			this->readComplete(ErrorCode::kSuccess);
		}
	}

void
McciCatena::cStreamLineCollector::readComplete(
	ErrorCode uStatus
	)
	{
	
	}
