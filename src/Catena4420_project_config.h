/* Catena4420_project_config.h	Tue Nov  1 2016 11:59:47 tmm */

/*

Module:  Catena4420_project_config.h

Function:
	Bridge from anywhere to the lmic project_config

Version:
	V0.3.0	Tue Nov  1 2016 11:59:47 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	November 2016

Revision history:
   0.3.0  Tue Nov  1 2016 11:59:47  tmm
	Module created.

*/

#ifndef _CATENA4420_PROJECT_CONFIG_H_		/* prevent multiple includes */
#define _CATENA4420_PROJECT_CONFIG_H_

#include "../../arduino-lmic/src/lmic/lmic_config_preconditions.h"

/* define the following non-zero for using a static key file */
#ifndef CATENA4420_USE_STATIC_KEYS /* PARAM */
# define CATENA4420_USE_STATIC_KEYS 0
#endif

/**** end of Catena4420_project_config.h ****/
#endif /* _CATENA4420_PROJECT_CONFIG_H_ */
