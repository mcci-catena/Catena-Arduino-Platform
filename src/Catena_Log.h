/*

Module:  Catena_Log.h

Function:
	McciCatena::cLog and associated functions.

Copyright notice:
	See accompanying LICENSE file.

Author:
	Terry Moore, MCCI Corporation	March 2017

*/

#ifndef _CATENA_LOG_H_		/* prevent multiple includes */
#define _CATENA_LOG_H_

#include <cstdint>

#pragma once

namespace McciCatena {

class cLog
	{
public:
	enum DebugFlags : uint32_t
		{
		kAlways		= 0u,			///< this mask is always enabled
		kFatal		= 0xFFFFFFFFu,		///< this mask indicates a fatal error, always enabled

                kBug            = 0x00000001u,		///< use this for errors that indicate software bugs
		kError		= 0x00000002u,		///< use this for errors that indicate operational probs
		kWarning	= 0x00000004u,		///< use this for warnings; not an error but might need attention
		kInfo		= 0x00000008u,		///< use this for normal informational messages.
		kVerbose	= 0x00000010u,		///< use this for verbose (chatty) messages
		kRfu5		= 0x00000020u,		///< reserved for future definition in library
		kRfu6		= 0x00000040u,		///< reserved for future definition in library
		kRfu7		= 0x00000080u,		///< reserved for future definition in library
		kUser8		= 0x00000100u,		///< Reserved for use by programs and applications
		kUser9		= 0x00000200u,		///< Reserved for use by programs and applications
		kUser10		= 0x00000400u,		///< Reserved for use by programs and applications
		kUser11		= 0x00000800u,		///< Reserved for use by programs and applications
		kUser12		= 0x00001000u,		///< Reserved for use by programs and applications
		kUser13		= 0x00002000u,		///< Reserved for use by programs and applications
		kUser14		= 0x00004000u,		///< Reserved for use by programs and applications
		kUser15		= 0x00008000u,		///< Reserved for use by programs and applications
		kTrace0		= 0x00010000u,		///< "Trace": debugging bit 0
		kTrace		= kTrace0,		///< Legacy name for kTrace0
		kTrace1		= 0x00020000u,		///< Library debugging trace bit 1
		kTrace2		= 0x00040000u,		///< Library debugging trace bit 2
		kTrace3		= 0x00080000u,		///< Library debugging trace bit 3
		kTrace4		= 0x00100000u,		///< Library debugging trace bit 4
		kTrace5		= 0x00200000u,		///< Library debugging trace bit 5
		kTrace6		= 0x00400000u,		///< Library debugging trace bit 6
		kTrace7		= 0x00800000u,		///< Library debugging trace bit 7
		kUserTrace8	= 0x01000000u,		///< User debugging trace bit 8
		kUserTrace9	= 0x02000000u,		///< User debugging trace bit 9
		kUserTrace10	= 0x04000000u,		///< User debugging trace bit 10
		kUserTrace11	= 0x08000000u,		///< User debugging trace bit 11
		kUserTrace12	= 0x10000000u,		///< User debugging trace bit 12
		kUserTrace13	= 0x20000000u,		///< User debugging trace bit 13
		kUserTrace14	= 0x40000000u,		///< User debugging trace bit 14
		kUserTrace15	= 0x80000000u,		///< User debugging trace bit 15
		};

	cLog(DebugFlags flags = DebugFlags(kInfo | kWarning | kError | kBug))
                : m_uDebugFlags(flags)
                {};

	// neither copyable nor movable.
	cLog(const cLog&) = delete;
	cLog& operator=(const cLog&) = delete;
	cLog(const cLog&&) = delete;
	cLog& operator=(const cLog&&) = delete;

	// initialize
	bool begin(DebugFlags uDebugFlags = DebugFlags(kInfo | kWarning | kError | kBug))
		{
		this->m_uDebugFlags = uDebugFlags;
		}

	// old, incorrect polarity: return false if isEnabled() is true..
	// [[deprecated("use isEnabled(), check polarities; see issue #165")]]
	__attribute__((deprecated("use isEnabled(), check polarities; see issue #165")))
	bool isenabled(DebugFlags uDebugFlags) const
		{
		return !this->isEnabled(uDebugFlags);
		}

	// check whether flags are enabled ... inline for speed
	bool isEnabled(DebugFlags uDebugFlags) const
		{
		return ((uDebugFlags & this->m_uDebugFlags) != 0 ||
		         uDebugFlags == kAlways ||
			 uDebugFlags == kFatal);
		}

	// log, using debug flags. Note that args are evaluated even if no print.
	void printf(
		DebugFlags uDebugFlags,
		const char *pFmt,
		...
		) __attribute__((__format__(__printf__, 3, 4)));
		/* format counts start with 2 for non-static C++ member fns */

	// log, using debug flags and a template; you can write:
	//  gLog.cond(gLog.kTrace,
	//	[&](){gCatena.SafePrintf("message", arg1, arg2); })
	// and the debug code will only be called if trace is enabled.
	// But it may be nicer to write:
	// if (gLog.isEnabled(gLog.kTrace))
	//	gCatena.SafePrintf("message", arg1, arg2...);
	template <typename Functor>
	void cond(
		DebugFlags uDebugFlags,
		Functor &f
		)
		{
		if (this->isEnabled(uDebugFlags))
			f();
		}

	// fetch current log flags
	DebugFlags getFlags(void) const
		{
		return this->m_uDebugFlags;
		}

	// set log flags and return previous value
	DebugFlags setFlags(DebugFlags flags)
		{
		DebugFlags const oldFlags = this->m_uDebugFlags;

		this->m_uDebugFlags = flags;
		return oldFlags;
		}

private:
	DebugFlags m_uDebugFlags;
	};

inline cLog::DebugFlags operator| (cLog::DebugFlags a, cLog::DebugFlags b)
	{
	return cLog::DebugFlags(std::uint32_t(a) | std::uint32_t(b));
	}

inline cLog::DebugFlags& operator| (cLog::DebugFlags& a, cLog::DebugFlags b)
	{
	a = cLog::DebugFlags(std::uint32_t(a) | std::uint32_t(b));
	return a;
	}

extern cLog gLog;

}; // namespace McciCatena

/**** end of Catena_Log.h ****/
#endif /* _CATENA_LOG_H_ */
