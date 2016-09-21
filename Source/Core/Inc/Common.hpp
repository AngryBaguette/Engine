#pragma once

#include <stdint.h>
#include <assert.h>

/* Compiler */
#define FORCEINLINE __forceinline
#define CONSTEXPR	constexpr
#define OVERRIDE	override
#ifndef NULL
# define NULL nullptr
#endif

/* For readability */
#define AND &&
#define EQ ==
#define NEQ !=
#define NOT !
#define OR ||


#define TODO assert(false AND ">> TODO <<")