#pragma once
// Generated file

// Static or dynamic link configuration
#ifdef _WINDOWS
#	ifdef VulkanSimple_MAKELIB	// Create a static library.
#		define VulkanSimple_EXPORT
#		define VulkanSimple_TEMPLATE_EXPORT
#	elif defined VulkanSimple_MAKEDLL || defined VulkanSimple_MAKEEXE	// Create a DLL library or an EXECUTABLE.
#		define VulkanSimple_EXPORT	__declspec(dllexport)
#		define VulkanSimple_TEMPLATE_EXPORT
#	elif defined VulkanSimple_USELIB	// Use a static library.
#		define VulkanSimple_EXPORT
#		define VulkanSimple_TEMPLATE_EXPORT
#	else	// Use DLL library
#		define VulkanSimple_EXPORT	__declspec(dllimport)
#		define VulkanSimple_TEMPLATE_EXPORT	extern
#	endif
#else
#	if defined(VulkanSimple_USELIB)
#		define VulkanSimple_EXPORT
#		define VulkanSimple_TEMPLATE_EXPORT extern
#	else
#		define VulkanSimple_EXPORT
#		define VulkanSimple_TEMPLATE_EXPORT
#	endif
#endif
