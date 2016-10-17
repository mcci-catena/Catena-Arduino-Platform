/* mcciadk_env.h	Mon Oct 17 2016 02:21:06 tmm */

/*

Module:  mcciadk_env.h

Function:
	Environment for ADK work.

Version:
	V0.1.0	Mon Oct 17 2016 02:21:06 tmm	Edit level 1

Copyright notice:
	This file copyright (C) 2016 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	October 2016

Revision history:
   0.1.0  Mon Oct 17 2016 02:21:06  tmm
	Module created.

*/

#ifndef _MCCIADK_ENV_H_		/* prevent multiple includes */
#define _MCCIADK_ENV_H_

#define	MCCIADK_STRING(x)	#x
#define	MCCIADK_STRINGVAL(x)	MCCIADK_STRING(x)
#define MCCIADK_CONCAT(x,y)	x##y
#define	MCCIADK_CONCAT3(x,y,z)	x##y##z
#define	MCCIADK_CONCATVAL3(x,y,z) MCCIADK_CONCAT3(x,y,z)

/****************************************************************************\
|
|	Define MCCIADK_CONTAINER_OF
|
\****************************************************************************/

#ifndef MCCIADK_CONTAINER_OF
# define MCCIADK_CONTAINER_OF(p, TypeOfContainer, FieldInContainer)	\
	((TypeOfContainer *) (((char *) (p)) -				\
			      offsetof(TypeOfContainer, FieldInContainer)))
#endif

/****************************************************************************\
|
|	Define MCCIADK_MEMTAG
|
\****************************************************************************/

#ifndef MCCIADK_MEMTAG
# define MCCIADK_MEMTAG(a, b, c, d)	\
	(((d) << 24) | ((c) << 16) | ((b) << 8) | (a))
#endif


/*

Macro:	MCCIADK_C_ASSERT()

Function:
	Declaration-like macro that will cause a compile error if arg is FALSE.

Definition:
	MCCIADK_C_ASSERT(
		BOOL fErrorIfFalse
		);

Description:
	This macro, if used where an external reference declarataion is
	permitted, will either compile cleanly, or will cause a compilation
	error. The results of using this macro where a declaration is not
	permitted are unspecified.

	This is different from #if !(fErrorIfFalse) / #error in that the 
	expression is evaluated by the compiler rather than by the pre-
	processor. Therefore things like sizeof() can be used.

Returns:
	No explicit result -- either compiles cleanly or causes a compile
	error.

*/

#ifndef MCCIADK_C_ASSERT
# define MCCIADK_C_ASSERT(e)	\
 void  MCCIADK_C_ASSERT__(int MCCIADK_C_ASSERT_x[(e) ? 1: -1])
#endif

/****************************************************************************\
|
|	Define the begin/end declaration tags for C++ co-existance
|
\****************************************************************************/

#ifdef __cplusplus
# define MCCIADK_BEGIN_DECLS	extern "C" {
# define MCCIADK_END_DECLS	}
#else
# define MCCIADK_BEGIN_DECLS	/* nothing */
# define MCCIADK_END_DECLS	/* nothing */
#endif

/*

Name:	MCCIADK_UNREFERENCED_PARAMETER()

Index:	Name:	MCCIADK_API_PARAMETER()
	Name:	MCCIADK_UNREFERENCED_VARIABLE()

Function:
	Defeat compiler warnings for variables or parameters that are
	intentionally unused.

Definition:
	MCCIADK_UNREFERENCED_PARAMETER(name)	
	MCCIADK_API_PARAMETER(name)
	MCCIADK_UNREFERENCED_VARIABLE(name)

Description:
	MCCIADK_UNREFERENCED_PARAMETER() is used to mark a parameter that
	is intentionally not referenced for some reason. It might be reserved
	for future use, or coding might not be complete.

	MCCIADK_API_PARAMETER() is like MCCIADK_UNREFERENCED_PARAMETER(),
	except that it indicates that this function is providing an API;
	other implementations of the API might use the parameter.

	MCCIADK_UNREFERENCED_VARIABLE() is used to mark a variable that is
	declared but intentionally not referenced. This is useful when test-
	compiling code that is incomplete. It's also used elsewhere to define
	MCCIADK_DEBUG_VARIABLE(), which says that code only references a
	variable when compiled with MCCIADK_DEBUG declared and not zero.

	Example:

	void anyfunction(
		SOMETYPE a
		}
		{
		// declarations
		int b;

		// unreferenced parameters and variables
		MCCIADK_UNREFERENCED_PARAMETER(a);
		MCCIADK_UNREFERENCED_VARIABLE(b);

		// no code
		}

Notes:
	All of these macros should be used only after all declarations
	have been written, but before any executable code is written.

	These macros may be overridden by MCCIADK_ENV_H_LOCAL.

*/

#ifndef MCCIADK_UNREFERENCED_PARAMETER
# define MCCIADK_UNREFERENCED_PARAMETER(v)	do { (void) (v); } while (0)
#endif

/* we don't really want extra macro expansions, so we define explicitly */
#ifndef	MCCIADK_API_PARAMETER
# define MCCIADK_API_PARAMETER(v)		do { (void) (v); } while (0)
#endif

#ifndef MCCIADK_UNREFERENCED_VARIABLE
# define MCCIADK_UNREFERENCED_VARIABLE(v)	do { (void) (v); } while (0)
#endif

/*

Name:	MCCIADK_LENOF()

Function:
	Return the number of elements in an array.

Definition:
	size_t MCCIADK_LENOF(ArrayVar);

Description:
	Assuming that ArrayVar is the name of an array whose size is known
	in this compile unit, MCCIADK_LENOF() returns the number of elements
	in the array.

Returns:
	The count of elements in the array.

*/

#ifndef MCCIADK_LENOF
#  define MCCIADK_LENOF(a)      (sizeof(a) / sizeof((a)[0]))
#endif


/**** end of mcciadk_env.h ****/
#endif /* _MCCIADK_ENV_H_ */
