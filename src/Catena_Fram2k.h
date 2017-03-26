/* Catena_Fram2k.h	Sun Mar 12 2017 17:47:52 tmm */

/*

Module:  Catena_Fram2k.h

Function:
	class McciCatena::cFram2k

Version:
	V0.5.0	Sun Mar 12 2017 17:47:52 tmm	Edit level 1

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
   0.5.0  Sun Mar 12 2017 17:47:52  tmm
	Module created.

*/

#ifndef _CATENA_FRAM2K_H_		/* prevent multiple includes */
#define _CATENA_FRAM2K_H_

#pragma once

#ifndef _CATENA_FRAM_H_
# include "Catena_Fram.h"
#endif

#ifndef _ADAFRUIT_FRAM_I2C_H_
# include <Adafruit_FRAM_I2C.h>
#endif

/****************************************************************************\
|
|	The contents
|
\****************************************************************************/


namespace McciCatena {


class cFram2k : public cFram
	{
protected:
	using Super = cFram;

public:
	cFram2k() { this->m_fReady = false; };
	virtual ~cFram2k() {};

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
                return 2 * 1024;
                };


protected:
private:
	Adafruit_FRAM_I2C	m_hw;
	};

}; // namespace McciCatena

/**** end of Catena_Fram2k.h ****/
#endif /* _CATENA_FRAM2K_H_ */
