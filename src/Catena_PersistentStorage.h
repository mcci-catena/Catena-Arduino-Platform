/* Catena_PersistentStorage.h	Sun Mar 12 2017 17:49:22 tmm */

/*

Module:  Catena_PersistentStorage.h

Function:
	Framework for persistent storage on Arduino Catenas.

Version:
	V0.5.0	Sun Mar 12 2017 17:49:22 tmm	Edit level 1

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
   0.5.0  Sun Mar 12 2017 17:49:22  tmm
	Module created.

*/

#ifndef _CATENA_PERSISTENTSTORAGE_H_		/* prevent multiple includes */
#define _CATENA_PERSISTENTSTORAGE_H_

#pragma once

#ifndef _MCCIADK_GUID_H_
# include "mcciadk_guid.h"
#endif

namespace McciCatena {

class cPersistentStorage
	{
public:
	// constructor
	cPersistentStorage() {};
	// destructor
	virtual ~cPersistentStorage() {};

	// neither copyable nor movable
	cPersistentStorage(const cPersistentStorage&) = delete;
	cPersistentStorage& operator=(const cPersistentStorage&) = delete;
	cPersistentStorage(const cPersistentStorage&&) = delete;
	cPersistentStorage& operator=(const cPersistentStorage&&) = delete;

	// start storage operations
	virtual bool begin() = 0;

	// initialize the store (gently) - do not overwite store if valid
	virtual bool initialize() = 0;

        // initialize the store (forcibly).
        virtual bool reset() = 0;

        // return the state of the store.
        virtual bool isValid() = 0;

        // query whether the FRAM is valid and ready
        virtual bool isReady() const = 0;

        // invalidate any cached ready state
        virtual void invalidate() = 0;

private:
	};

} // namespace McciCatena

/**** end of Catena_PersistentStorage.h ****/
#endif /* _CATENA_OBJECT_H_ */
