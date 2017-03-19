/* Catena_CommandStream.h	Sun Mar 19 2017 00:19:22 tmm */

/*

Module:  Catena_CommandStream.h

Function:
	class McciCatena::cCommandStream

Version:
	V0.5.0	Sun Mar 19 2017 00:19:22 tmm	Edit level 1

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
   0.5.0  Sun Mar 19 2017 00:19:22  tmm
	Module created.

*/

#ifndef _CATENA_COMMANDSTREAM_H_		/* prevent multiple includes */
#define _CATENA_COMMANDSTREAM_H_

#ifndef _CATENA_POLLABLEINTERFACE_H_
# include "Catena_PollableInterface.h"
#endif

#ifndef _CATENA_STREAMLINECOLLECTOR_H_
# include "Catena_StreamLineCollector.h"
#endif

#ifndef _CATENABASE_H_
# include "CatenaBase.h"
#endif

namespace McciCatena {

class cCommandStream;	// forward

class cCommandStream : public cPollableObject
	{
public:
	cCommandStream() {};
	virtual ~cCommandStream() {};

	// neither copyable nor movable.
	cCommandStream(const cCommandStream&) = delete;
	cCommandStream& operator=(const cCommandStream&) = delete;
	cCommandStream(const cCommandStream&&) = delete;
	cCommandStream& operator=(const cCommandStream&&) = delete;

	bool begin(
		cStreamLineCollector *pCollector, // the command line reader
		CatenaBase *pCatena		  // the owning Catena object
		);

	virtual void poll(void);

protected:

private:
	// launch the read request
	void launchRead(void);

	// a simple callback that calls the method.
	static void readCompleteCb(
		void *pCtx,
		cStreamLineCollector::ErrorCode uStatus,
		uint8_t *pBuffer,
		size_t nBuffer
		)
		{
		cCommandStream * const pThis =
			static_cast<cCommandStream *>(pCtx);

		pThis->readComplete(uStatus, pBuffer, nBuffer);
		}

	// indicate that a line has arrived from the lower level.
	void readComplete(
		cStreamLineCollector::ErrorCode uStatus,
		uint8_t *pBuffer,
		size_t nBuffer
		);

	// the command buffer
	uint8_t m_buffer[128];

	// the status of the operation
	
	size_t m_nRead;			// how many bytes were read
	bool m_fReadPending = false;	// is a read pending?
	bool m_fReadComplete = false;	// is a read complete?
	uint8_t m_ReadStatus;		// status of last read.

	// the collector
	cStreamLineCollector *m_pCollector = nullptr;

	// pointer to the owning Catena
	CatenaBase *m_pCatena = nullptr;
	};

}; // namespace McciCatena

/**** end of Catena_CommandStream.h ****/
#endif /* _CATENA_COMMANDSTREAM_H_ */
