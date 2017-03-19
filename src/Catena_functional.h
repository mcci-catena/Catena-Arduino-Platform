/* Catena_functional.h	Sat Mar 18 2017 16:06:51 tmm */

/*

Module:  Catena_functional.h

Function:
	Wrapper for <functional> when working with Arduino code.

Version:
	V0.5.0	Sat Mar 18 2017 16:06:51 tmm	Edit level 1

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
   0.5.0  Sat Mar 18 2017 16:06:51  tmm
	Module created.

*/

#ifndef _CATENA_FUNCTIONAL_H_		/* prevent multiple includes */
#define _CATENA_FUNCTIONAL_H_

#pragma once

// this is ugly. Arduino.h defines macros min() and max().... which clobber
// functional. So we do some dark magic.
#if defined(Arduino_h)
# define CATENA_FUNCTIONAL_min_defined 1
# define CATENA_FUNCTIONAL_max_defined 1
#else
# if defined(min)
#  define CATENA_FUNCTIONAL_min_defined 1
# else
#  define CATENA_FUNCTIONAL_min_defined 0
# endif
# if defined(max)
#  define CATENA_FUNCTIONAL_max_defined 1
# else
#  define CATENA_FUNCTIONAL_max_defined 0
# endif
#endif

#if CATENA_FUNCTIONAL_min_defined
# undef min
#endif
#if CATENA_FUNCTIONAL_max_defined
# undef max
#endif

#include <functional>

/*
|| Now, we reinstate the Arduino.h values... I said it was ugly.
|| In an ideal world we'd be able to save and restore the below. Bear
|| in mind that there may be other words that Arduino.h steps on,
|| breaking C++ header files: it defines a number of lower-case macros with
|| commonly-used words. Since there are no namespaces for macros, it's
|| an unpleasant situation.
*/
#if CATENA_FUNCTIONAL_min_defined
# define min(a,b) ((a)<(b)?(a):(b))
#endif

#if CATENA_FUNCTIONAL_max_defined
# define max(a,b) ((a)>(b)?(a):(b))
#endif

/**** end of Catena_functional.h ****/
#endif /* _CATENA_FUNCTIONAL_H_ */
