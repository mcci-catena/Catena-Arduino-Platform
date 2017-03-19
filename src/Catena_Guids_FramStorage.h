/* Catena_Guids_FramStorage.h	Sun Mar 19 2017 18:28:38 tmm */

/*

Module:  Catena_Guids_FramStorage.h

Function:
	Guids for FRAM storage

Version:
	V0.5.0	Sun Mar 19 2017 18:28:38 tmm	Edit level 1

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
   0.5.0  Sun Mar 19 2017 18:28:38  tmm
	Module created.

*/

#ifndef _CATENA_GUIDS_FRAMSTORAGE_H_		/* prevent multiple includes */
#define _CATENA_GUIDS_FRAMSTORAGE_H_

#ifndef _MCCIADK_GUID_H_
# include "mcciadk_guid.h"
#endif

/****************************************************************************\
|
|	The storage format GUID
|
\****************************************************************************/

// {1DE7CDCD-0647-4B3C-A18D-8138A3D9613F}
#define GUID_FRAM_CATENA_V1(f)	\
	MCCIADK_GUID_GEN_INIT(f, 0x1de7cdcd, 0x647, 0x4b3c, 0xa1, 0x8d, 0x81, 0x38, 0xa3, 0xd9, 0x61, 0x3f)

/**** end of Catena_Guids_FramStorage.h ****/
#endif /* _CATENA_GUIDS_FRAMSTORAGE_H_ */
