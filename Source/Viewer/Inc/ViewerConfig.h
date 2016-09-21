#pragma once

// Static or dynamic link configuration
#ifdef _WINDOWS
#	ifdef Viewer_MAKELIB	// Create a static library.
#		define Viewer_EXPORT
#		define Viewer_TEMPLATE_EXPORT
#	elif defined Viewer_MAKEDLL || defined Viewer_MAKEEXE	// Create a DLL library or an EXECUTABLE.
#		define Viewer_EXPORT	__declspec(dllexport)
#		define Viewer_TEMPLATE_EXPORT
#	elif defined Viewer_USELIB	// Use a static library.
#		define Viewer_EXPORT
#		define Viewer_TEMPLATE_EXPORT
#	else	// Use DLL library
#		define Viewer_EXPORT	__declspec(dllimport)
#		define Viewer_TEMPLATE_EXPORT	extern
#	endif
#else
#	if defined(Viewer_USELIB)
#		define Viewer_EXPORT
#		define Viewer_TEMPLATE_EXPORT extern
#	else
#		define Viewer_EXPORT
#		define Viewer_TEMPLATE_EXPORT
#	endif
#endif
