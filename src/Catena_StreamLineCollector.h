/*

Module:  Catena_StreamLineCollector.h

Function:
	McciCatena::cStreamLineCollector

Copyright notice:
	This file copyright (C) 2017, 2019 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	See accompanying license file.

Author:
	Terry Moore, MCCI Corporation	March 2017

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
#include <Catena_limits.h>

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
	using ColumnNumber_t = std::uint8_t;
	static constexpr ColumnNumber_t kColumnMax = cNumericLimits<ColumnNumber_t>::numeric_limits_max();

protected:
	class Columnator
		{
	private:
		enum class EncodingState : std::uint8_t
			{
			Normal,
			Esc1,
			EscOsc,
			CSI1,
			CSI2,
			UTF8,
			Transparent,
			};

	public:
		Columnator()
			: m_column(0)
			, m_state(EncodingState::Normal)
			{}

		ColumnNumber_t adjust(const char *pString);
		void adjustColumn(std::uint8_t c, bool fEchoMode);
		void reset(ColumnNumber_t col = 0)
			{
			this->m_column = col;
			this->m_state = EncodingState::Normal;
			}
		ColumnNumber_t getColumn() const
			{ return this->m_column; }
		void setTransarent(bool fTransparent)
			{
			if (fTransparent)
				{
				this->m_state = EncodingState::Transparent;
				}
			else
				{
				this->reset();
				}
			}

	private:
		ColumnNumber_t m_column;
		EncodingState m_state;
		};

public:
	cStreamLineCollector() {};
	virtual ~cStreamLineCollector() {};

	// neither copyable nor movable
	cStreamLineCollector(const cStreamLineCollector&) = delete;
	cStreamLineCollector& operator=(const cStreamLineCollector&) = delete;
	cStreamLineCollector(const cStreamLineCollector&&) = delete;
	cStreamLineCollector& operator=(const cStreamLineCollector&&) = delete;

	enum EditChars: std::uint8_t
		{
		kEol = '\n',
                kCr = '\r',
		kLf = '\n',
		kRetype = 'R' & 0x1f,	/* ^R: retype */
		kTab = '\t',
		kSp = ' ',
		kBackspace = 'H' & 0x1f,
		kDel = 0x7F,
		kEsc = '\e',
		kCancel = 'U' & 0x1f,	/* ^U: cancel */
		kCaret = '^',
		};

	enum ErrorCode: std::uint32_t
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

	// write a single character
	void write(std::uint8_t c);

	// formatted print
	void printf(const char *pFmt, ...)
                __attribute__((__format__(__printf__, 2, 3)));
                /* format counts start with 2 for non-static C++ member fns */

	// formatted print for nested callers
        void vprintf(const char *pFmt, std::va_list ap);

	// enable or disable echo
	void setEcho(bool fEnable)
		{
		this->m_fNoEcho = ! fEnable;
		}

	// get echo-enable state
	bool getEcho() const
		{
		return ! this->m_fNoEcho;
		}

protected:
	void inputEdit(std::uint8_t c);
	void doEcho(std::uint8_t c);
	void doInput(std::uint8_t c);
	void doInputCancel();
	void doInputDelete();
	void doInputRetype();
	void realign(Columnator &t);
	void echoControl(std::uint8_t c);

private:
	// the stream we're working with
	Stream			*m_pStream = nullptr;
        bool                    m_fLastWasCr = false;
	bool			m_fNoEcho = false;
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
	Columnator		m_inputColumn;
	Columnator		m_outputColumn;
	};


}; // namespace McciCatena

/**** end of Catena_StreamLineCollector.h ****/
#endif /* _CATENA_STREAMLINECOLLECTOR_H_ */
