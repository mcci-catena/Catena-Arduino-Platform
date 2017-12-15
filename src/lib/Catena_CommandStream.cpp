/* Catena_CommandStream.cpp	Sun Mar 19 2017 00:54:43 tmm */

/*

Module:  Catena_CommandStream.cpp

Function:
	class McciCatena::cCommandStream implementation.

Version:
	V0.5.0	Sun Mar 19 2017 00:54:43 tmm	Edit level 1

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
   0.5.0  Sun Mar 19 2017 00:54:43  tmm
	Module created.

*/

#include "Catena_CommandStream.h"

#include "CatenaBase.h"
#include "Catena_Log.h"
#include <cstring>
#include <cstdarg>
#include "mcciadk_baselib.h"

using namespace McciCatena;

/****************************************************************************\
|
|	The command table
|
\****************************************************************************/

static cCommandStream::CommandFn doEcho;

static const cCommandStream::cEntry sDefaultEntries[] =
	{
	{ "echo", doEcho },
	};

static cCommandStream::cDispatch sDefault(
		sDefaultEntries, sizeof(sDefaultEntries)
		);

static cCommandStream::CommandStatus
doEcho(cCommandStream *pThis, void *pContext, int argc, char**argv)
        {
        for (int i = 1; i < argc; ++i)
                {
                pThis->printf(
                        "%s%c", 
                        argv[i], 
                        (i < argc - 1) ? ' ' : '\n'
                        );
                }
        return cCommandStream::kSuccess;
        }

bool 
McciCatena::cCommandStream::begin(
	cStreamLineCollector *pCollector, // the command line reader
	CatenaBase *pCatena		  // the owning Catena object
	)
	{
	this->m_pCollector = pCollector;
	this->m_pCatena = pCatena;
	this->m_pCatena->registerObject(this);

	this->registerCommands(&sDefault, nullptr);

	// launch a read
	this->launchRead();

	return true;
	}

void 
McciCatena::cCommandStream::readComplete(
	cStreamLineCollector::ErrorCode uStatus,
	uint8_t *pBuffer,
	size_t nBuffer
	)
	{
	this->m_fReadPending = false;
	this->m_fReadComplete = true;
	this->m_ReadStatus = uStatus;

	if (uStatus == cStreamLineCollector::kSuccess)
		{
		this->m_nRead = nBuffer;
		}
	else
		{
		this->m_nRead = 0;
		}
	}

void 
McciCatena::cCommandStream::poll(void)
	{
	if (this->m_fReadComplete)
		{
		this->m_fReadComplete = false;

		if (this->m_ReadStatus == cStreamLineCollector::kSuccess)
			{
			this->m_buffer[this->m_nRead - 1] = '\0';

			// Log.printf(
			//	Log.kAlways,
			//	"received: %s\n",
			//	this->m_buffer
			//	);

			int status = this->parseAndDispatch();
                        if (status == 0)
                                this->printf("\rOK\n");
                        else
                                this->printf("\r?%d\n", status);
			}
		else
			{
			this->printf(
				"?*** error %u ***\n",
				this->m_ReadStatus
				);
			}
		}

	if (! this->m_fReadPending)
		this->launchRead();
	}

int
McciCatena::cCommandStream::parseAndDispatch()
	{
	int argc;
	uint8_t *pBuffer;
	uint8_t *pEndBuffer;

	pBuffer = this->m_buffer;
	pEndBuffer = pBuffer + this->m_nRead;

	for (argc = 0; 
             argc < sizeof(this->m_argv)/sizeof(this->m_argv[0]) - 1; 
             ++argc)
		{
		uint8_t c;

		/* skip leading blanks */
		for (; pBuffer < pEndBuffer && (c = *pBuffer) <= 0x20; )
			*pBuffer++ = 0;

		if (pBuffer == pEndBuffer)
			break;

		this->m_argv[argc] = (char *)pBuffer;

		/* skip chars */
		while (pBuffer < pEndBuffer && (c = *pBuffer) > 0x20)
			++pBuffer;
		}

	// if too many arguments, just give up.
	if (pBuffer != pEndBuffer)
		return -1;

	this->m_argv[argc] = nullptr;

	return this->dispatch(argc, this->m_argv);
	}

void
McciCatena::cCommandStream::registerCommands(
	cDispatch *pObject,
        void *pContext
	)
	{
        pObject->m_pContext = pContext;

	auto const pHead = this->m_pHead;
	if (pHead == nullptr)
		{
		pObject->m_pNext = pObject;
		pObject->m_pLast = pObject;
		this->m_pHead = pObject;
		}
	else
		{
		auto const pLast = pHead->m_pLast;
		pObject->m_pNext = pHead;
		pObject->m_pLast = pLast;
		pLast->m_pNext = pObject;
		pHead->m_pLast = pObject;

                // this object becomes the new head
                this->m_pHead = pObject;
		}
	}

int
McciCatena::cCommandStream::dispatch(
	int argc,
	char **argv
	)
	{
	if (argc == 0)
		return 0;

	const char * const pCommand = argv[0];
        // Log.printf(Log.kAlways, "dispatch: %s\n", pCommand);

        if (McciAdkLib_StringCompareCaseInsensitive(pCommand, "help") == 0)
                {
                this->dispatchHelp(argc, argv);
                return 0;
                }

	auto const pHead = this->m_pHead;
	if (pHead != nullptr)
		{
		auto pThis = pHead;

		// iterate over all command tables.		
		do
			{
			const char * const pGroupName = pThis->m_pGroupName;
			CommandFn *pCommandFn;
			const cEntry *pEntry;

			if (pGroupName == nullptr)
				{
				pEntry = pThis->search(pCommand);
				}
			else
				{
                                // Log.printf(Log.kAlways, "check group for: %s\n", pGroupName);

                                // need two words, and need to match first word.
				if (argc < 2 ||
				    McciAdkLib_StringCompareCaseInsensitive(pCommand, pGroupName) != 0
				    )
					// no luck.
					pEntry = nullptr;
				else
					{
					// first word matched, try second
					pEntry = pThis->search(argv[1]);

					// if it matches, consume first word
					if (pEntry != nullptr)
						++argv, --argc;
					}
				}

			if (pEntry != nullptr)
				{
				CommandStatus status;
                                // Log.printf(Log.kAlways, "dispatch %s\n", argv[0]);
                                status = pEntry->pDispatch(
						this,
						pThis->m_pContext,
						argc, 
						argv
						);

				return (status >= 0) ? status : kError;
				}

			pThis = pThis->m_pNext;
			} while (pThis != pHead);
		}

	// if we get here, the command wasn't matched
	return -1;
	}

int
McciCatena::cCommandStream::dispatchHelp(
	int argc,
	char **argv
	)
        {
        // walk all the tables and print simple help.
	auto const pHead = this->m_pHead;
	if (pHead != nullptr)
		{
		auto pThis = pHead;

		// iterate over all command tables.		
		do
			{
			const char * const pGroupName = pThis->m_pGroupName;

                        auto pEntry = pThis->m_pEntries;
                        for (unsigned i = 0;
                                i < pThis->m_nEntries;
                                ++i, ++pEntry)
                                {
                                if (pGroupName != nullptr)
                                        this->printf("%s %s\n", pGroupName, pEntry->pName);
                                else
                                        this->printf("%s\n", pEntry->pName);
                                }

			pThis = pThis->m_pNext;
			} while (pThis != pHead);
		}

        return 0;
        }

const cCommandStream::cEntry * 
McciCatena::cCommandStream::cDispatch::search(const char *pCommand) const
	{
	auto pEntry = this->m_pEntries;
	for (unsigned i = 0; 
	     i < this->m_nEntries; 
	     ++i, ++pEntry)
		{
		if (std::strcmp(pEntry->pName, pCommand) == 0)
			{
			return pEntry;
			}
		}

	return nullptr;
	}

void
McciCatena::cCommandStream::launchRead(void)
	{
	this->m_fReadPending = true;
	this->m_fReadComplete = false;

	this->m_pCollector->readAsync(
		readCompleteCb,
		static_cast<void *>(this),
		this->m_buffer,
		sizeof(this->m_buffer)
		);
	}

void
McciCatena::cCommandStream::printf(
        const char *pFmt, ...
        )
        {
        std::va_list ap;

        va_start(ap, pFmt);
        (void)this->m_pCollector->vprintf(pFmt, ap);
        va_end(ap);
        }
