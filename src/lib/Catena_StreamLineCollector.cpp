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
	this->m_inputColumn.reset(this->m_outputColumn.getColumn());
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

			if (c == kEol || c == kCr)
				{
				this->m_fLastWasCr = (c == kCr);
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

                        // activate on cr or lf;
                        // silently ignore lf immediately after cr, so cr/lf
                        // is still effectively only one line ending.
                        if (c == kCr)
                                {
				this->inputEdit(kEol);
                                this->m_fLastWasCr = true;
                                status = ErrorCode::kSuccess;
                                }
                        else if (c == kEol)
                                {
                                if (! this->m_fLastWasCr)
					{
					this->inputEdit(c);
                                        status = ErrorCode::kSuccess;
					}

                                this->m_fLastWasCr = false;
                                }
                        else
				{
				this->inputEdit(c);
                                this->m_fLastWasCr = false;
				}
                        }

                // exited loop; complete if possible.
                if (status != ErrorCode::kBusy)
			this->readComplete(status);
		}
	}

void
McciCatena::cStreamLineCollector::inputEdit(
	std::uint8_t c
	)
	{
	switch (c)
		{
	case kBackspace:
	case kDel:
		/*
		|| "backspace" erases the current character
		*/
		this->doInputDelete();
		break;

	case kCancel:
		/*
		|| "control-X" erases the current line; the character
		|| is discarded.
		*/
		this->doInputCancel();
		break;

	case kRetype:
		/*
		|| "control-R" retypes the current line.
		*/
		this->doInputRetype();
		break;

	case '\0':
		/* just discard */
		break;

	default:
		this->doEcho(c);

		this->doInput(c);
		break;
		}
	}

void
McciCatena::cStreamLineCollector::doEcho(std::uint8_t c)
	{
	// don't echo if echo is off.
	if (this->m_fNoEcho)
		return;

	if (c == kCr || c == kLf || c == kTab)
		this->write(c);
	else if (0 <= c && c <= 0x1f)
		{
		this->echoControl(c);
		}
	else if (c == 0x7F)
		{
		this->echoControl(c);
		}
	else
		this->write(c);
	}

void
McciCatena::cStreamLineCollector::doInputDelete()
	{
	if (this->m_pInsert > this->m_pBuffer)
		{
		Columnator t = this->m_inputColumn;

		--this->m_pInsert;

		// adjust the column.
		for (auto p = this->m_pBuffer; p < this->m_pInsert; ++p)
			{
			t.adjustColumn(p[0], true);
			}

		this->realign(t);
		}
	}

void
McciCatena::cStreamLineCollector::doInputCancel()
	{
	this->m_pInsert = this->m_pBuffer;
	this->echoControl(kCancel); this->doEcho('\n');
	this->realign(this->m_inputColumn);
	}

void
McciCatena::cStreamLineCollector::doInputRetype()
	{
	this->echoControl(kRetype); this->doEcho('\n');
	this->realign(this->m_inputColumn);
	for (auto p = this->m_pBuffer; p < this->m_pInsert; ++p)
		this->doEcho(*p);
	}

void
McciCatena::cStreamLineCollector::realign(
	cStreamLineCollector::Columnator &t
	)
	{
	if (this->m_fNoEcho)
		return;

	while (t.getColumn() < this->m_outputColumn.getColumn())
		{
		this->write(kBackspace);
		this->write(' ');
		this->write(kBackspace);
		}
	while (t.getColumn() > this->m_outputColumn.getColumn())
		{
		this->write(' ');
		}
	}

void
McciCatena::cStreamLineCollector::doInput(
	std::uint8_t c
	)
	{
	*this->m_pInsert++ = c;
	}

void
McciCatena::cStreamLineCollector::echoControl(
	std::uint8_t c
	)
	{
	this->doEcho('^');
	this->doEcho(c ^ 0x40);
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
McciCatena::cStreamLineCollector::printf(
        const char *pFmt, ...
        )
        {
        std::va_list ap;

        va_start(ap, pFmt);
        (void)this->vprintf(pFmt, ap);
        va_end(ap);
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
	//this->m_outputColumn.adjust(buf);
	for (auto p = buf; ; ++p)
		{
		const std::uint8_t c = std::uint8_t(*p);
		if (c == '\0')
			break;

		this->write(c);
		}
        }

void
McciCatena::cStreamLineCollector::write(std::uint8_t c)
	{
	if (c == kLf && this->m_outputColumn.getColumn() != 0)
		this->write(kCr);

	// do tabs by adjusting column and aligning. This
	// has nicer boundary conditions.
	if (c == kTab)
		{
		auto t = this->m_outputColumn;
		t.adjustColumn(c, false);
		this->realign(t);
		return;
		}

	// normal path
	this->m_outputColumn.adjustColumn(c, false);
	this->m_pStream->print(char(c));
	}

McciCatena::cStreamLineCollector::ColumnNumber_t
McciCatena::cStreamLineCollector::Columnator::adjust(const char *pString)
	{
	for (;;)
		{
		const std::uint8_t c = std::uint8_t(*pString++);

		if (c == '\0')
			return this->getColumn();

		this->adjustColumn(c, false);
		}
	}

void
McciCatena::cStreamLineCollector::Columnator::adjustColumn(std::uint8_t c, bool fInputMode)
	{
	switch (this->m_state)
		{
	case EncodingState::Transparent:
		break;

	case EncodingState::Esc1:
		if (c == '[')
			{
			this->m_state = EncodingState::CSI1;
			return;
			}
		else if (c == ']')
			{
			this->m_state = EncodingState::EscOsc;
			return;
			}
		// in all other cases, consume the next charater and return,.
		this->m_state = EncodingState::Normal;
		return;

	case EncodingState::EscOsc:
		if (0x20 <= c && c <= 0x7e)
			return;
		this->m_state = EncodingState::Normal;
		goto normal;

	case EncodingState::CSI1:
		if (0x30 <= c && c <= 0x3F)
			// parameter byte.
			return;

	case EncodingState::CSI2:
		if (0x20 <= c && c <= 0x2F)
			{
			this->m_state = EncodingState::CSI2;
			return;
			}

		if (0x40 <= c && c <= 0x7E)
			{
			this->m_state = EncodingState::Normal;
			return;
			}
		this->m_state = EncodingState::Normal;
		goto normal;

	case EncodingState::UTF8:
		if (0xC0 <= c & c <= 0xFF)
			return;
		this->m_state = EncodingState::Normal;
		goto normal;

	default:
	case EncodingState::Normal:
	normal:
		if ((0x20 <= c && c <= 0x7E) || (0x80 <= c && c < 0xC0))
			{
			if (this->m_column < kColumnMax)
				++this->m_column;
			break;
			}
		else if (0xC0 <= c && c <= 0xFF)
			{
			this->m_state = EncodingState::UTF8;
			break;
			}

		// non-simple characters
		switch (c)
			{
		case kCr:
			this->m_column = 0;
			break;
		case kLf:
			this->m_column = 0;
			break;
		case kBackspace:
			if (this->m_column > 0)
				--this->m_column;
			break;
		case kTab:
			{
			ColumnNumber_t delta = 8 - (this->m_column & 7);
			if (this->m_column > kColumnMax - delta)
				this->m_column = kColumnMax;
			else
				this->m_column += delta;
			}
			break;
		case kEsc:
			if (! fInputMode)
				this->m_state = EncodingState::Esc1;
			break;

		default:
			// in input mode, this will be ^<something> (2 ch)
			// in normal mode, leave column number unchanged.
			if (fInputMode)
				{
				constexpr ColumnNumber_t nThresh = kColumnMax - 2;

				if (this->m_column <= nThresh)
					this->m_column += 2;
				else
					this->m_column = kColumnMax;
				}
			break;
			}
		break;
		}
	}