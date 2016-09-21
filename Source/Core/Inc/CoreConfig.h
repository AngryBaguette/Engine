#pragma once

// Static or dynamic link configuration
#ifdef _WINDOWS
#	ifdef Core_MAKELIB	// Create a static library.
#		define Core_EXPORT
#		define Core_TEMPLATE_EXPORT
#	elif defined Core_MAKEDLL || defined Core_MAKEEXE	// Create a DLL library or an EXECUTABLE.
#		define Core_EXPORT	__declspec(dllexport)
#		define Core_TEMPLATE_EXPORT
#	elif defined Core_USELIB	// Use a static library.
#		define Core_EXPORT
#		define Core_TEMPLATE_EXPORT
#	else	// Use DLL library
#		define Core_EXPORT	__declspec(dllimport)
#		define Core_TEMPLATE_EXPORT	extern
#	endif
#else
#	if defined(Core_USELIB)
#		define Core_EXPORT
#		define Core_TEMPLATE_EXPORT extern
#	else
#		define Core_EXPORT
#		define Core_TEMPLATE_EXPORT
#	endif
#endif
