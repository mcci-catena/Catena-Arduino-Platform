/* Catena_Fram2k.h	Sun Mar 12 2017 17:47:52 tmm */

/*

Module:  Catena_Fram2k.h

Function:
	Framework for Catena_Fram2k

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

#ifndef _CATENA_PERSISTENTSTORAGE_H_
# include "Catena_PersistentStorage.h"
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


class Fram2k : public PersistentStorage
	{
protected:
	using Super = PersistentStorage;

public:
	Fram2k() {};
	virtual ~Fram2k() {};

        // begin working with the FRAM
	virtual bool begin();

        // initialize the store
        virtual bool initialize();

        // check the store
        virtual bool isValid();

        // reset the store
        virtual bool reset();

protected:
private:
	Adafruit_FRAM_I2C	m_hw;
	};

}; // namespace McciCatena

/**** end of Catena_Fram2k.h ****/
#endif /* _CATENA_FRAM2K_H_ */
