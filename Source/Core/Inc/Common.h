#pragma once 

#include <iostream>
#include <cassert>

#if WIN32
#	define FORCEINLINE __forceinline
#else
#	define FORCEINLINE always_inline
#endif