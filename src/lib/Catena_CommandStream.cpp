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

using namespace McciCatena;

bool 
McciCatena::cCommandStream::begin(
	cStreamLineCollector *pCollector, // the command line reader
	CatenaBase *pCatena		  // the owning Catena object
	)
	{
	this->m_pCollector = pCollector;
	this->m_pCatena = pCatena;
	this->m_pCatena->registerObject(this);

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

			this->m_pCatena->SafePrintf(
				"received: %s\n",
				this->m_buffer
				);
			}
		else
			{
			this->m_pCatena->SafePrintf(
				"*** error %u ***\n",
				this->m_ReadStatus
				);
			}
		}

	if (! this->m_fReadPending)
		this->launchRead();
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
