// tu_memdebug.h	-- by Thatcher Ulrich <tu@tulrich.com>

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#ifndef TU_MEMDEBUG_H
#define TU_MEMDEBUG_H

#ifdef USE_STACKWALKER
	#include <windows.h>
	#include "base/Stackwalker.h"
#endif

namespace tu_memdebug
{
	enum debug_type
	{
		ALLOCCHECK,
		EXCEPTIONCATCH
	};

	void open(debug_type)
	{
#ifdef USE_STACKWALKER
		InitAllocCheck(ACOutput_XML);
#endif

	}

	void close()
	{
#ifdef USE_STACKWALKER
		DeInitAllocCheck();
#endif
	}

}

#endif
