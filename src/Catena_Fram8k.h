/* Catena_Fram8k.h	Fri Oct 13 2017 15:19:30 chwon */

/*

Module:  Catena_Fram8k.h

Function:
	class McciCatena::cFram8k

Version:
	V0.6.0	Fri Oct 13 2017 15:19:30 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2017 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	ChaeHee Won, MCCI Corporation	October 2017

Revision history:
   0.6.0  Fri Oct 13 2017 15:19:30  chwon
	Module created.

*/

#ifndef _CATENA_FRAM2K_H_		/* prevent multiple includes */
#define _CATENA_FRAM2K_H_

#pragma once

#ifndef _CATENA_FRAM_H_
# include "Catena_Fram.h"
#endif

#ifndef _CATENA_MB85RC64TA_H_
# include "Catena_Mb85rc64ta.h"
#endif

/****************************************************************************\
|
|	The contents
|
\****************************************************************************/


namespace McciCatena {


class cFram8k : public cFram
	{
protected:
	using Super = cFram;

public:
	cFram8k() { this->m_fReady = false; };
	virtual ~cFram8k() {};

        // begin working with the FRAM
	virtual bool begin() override;

        // read from the store
        virtual size_t read(
                cFramStorage::Offset uOffset, uint8_t *pBuffer, size_t nBuffer
                ) override;

        // write to the store
        virtual bool write(
                cFramStorage::Offset uOffset, const uint8_t *pBuffer, size_t nBuffer
                ) override;

        virtual cFramStorage::Offset getsize() const override
                {
                return 8 * 1024;
                };


protected:
private:
	Catena_Mb85rc64ta	m_hw;
	};

}; // namespace McciCatena

/**** end of Catena_Fram8k.h ****/
#endif /* _CATENA_FRAM2K_H_ */
