/* Catena_limits.h	Sat Mar 18 2017 16:06:51 tmm */

/*

Module:  Catena_limits.h

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

#ifndef _CATENA_LIMITS_H_		/* prevent multiple includes */
#define _CATENA_LIMITS_H_

#pragma once

// this is ugly. Arduino.h defines macros min() and max().... which clobber
// <limits>. So we do some dark magic.
#if defined(Arduino_h)
# define CATENA_LIMITS_min_defined 1
# define CATENA_LIMITS_max_defined 1
#else
# if defined(min)
#  define CATENA_LIMITS_min_defined 1
# else
#  define CATENA_LIMITS_min_defined 0
# endif
# if defined(max)
#  define CATENA_LIMITS_max_defined 1
# else
#  define CATENA_LIMITS_max_defined 0
# endif
#endif

#if CATENA_LIMITS_min_defined
# undef min
#endif
#if CATENA_LIMITS_max_defined
# undef max
#endif

#include <limits>

/*
|| Even more unfortunately -- if we re-instate min and max, we won't be
|| able to use numeric_limits<>::max and ::min.  So first we have to
|| declare templates (in McciCatena namespace) for the things we
|| need to use.
*/
namespace McciCatena {

template <typename T>
class cNumericLimits : public std::numeric_limits<T>
	{
public:
	static constexpr T numeric_limits_max() noexcept { return std::numeric_limits<T>::max(); }

	static constexpr T numeric_limits_min() noexcept { return std::numeric_limits<T>::min(); }

	// since `lowest` is mentally adjacent to `min`, we provide an alias for it, too.
	static constexpr T numeric_limits_lowest() noexcept { return std::numeric_limits<T>::lowest; }
	};

} // namespace McciCatena

/*
|| Now, we reinstate the Arduino.h values... I said it was ugly.
|| In an ideal world we'd be able to save and restore the below. Bear
|| in mind that there may be other words that Arduino.h steps on,
|| breaking C++ header files: it defines a number of lower-case macros with
|| commonly-used words. Since there are no namespaces for macros, it's
|| an unpleasant situation.
*/
#if CATENA_LIMITS_min_defined
# define min(a,b) ((a)<(b)?(a):(b))
#endif

#if CATENA_LIMITS_max_defined
# define max(a,b) ((a)>(b)?(a):(b))
#endif

/**** end of Catena_limits.h ****/
#endif /* _CATENA_LIMITS_H_ */
