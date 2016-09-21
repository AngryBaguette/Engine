#pragma once

#include "Memory.hpp"
#include <type_traits>

/* Copy function for non-trivial type */
template <typename T>
FORCEINLINE typename std::enable_if<NOT std::is_trivially_copy_assignable<T>::value>::type CopyAssignItems(T* pDest, const T* pSource, size_t pCount)
{
	while (pCount)
	{
		*pDest = *pSource;
		++pDest;
		++pSource;
		--pCount;
	}
}

/* Copy function for trivial type */
template <typename T>
FORCEINLINE typename std::enable_if<std::is_trivially_copy_assignable<T>::value>::type CopyAssignItems(T* pDest, const T* pSource, size_t pCount)
{
	Memory::Memcpy(pDest, pSource, sizeof(T)*pCount);
}

/* Construction for non-trivial type */
template <typename T>
FORCEINLINE typename std::enable_if<NOT std::is_trivially_constructible<T>::value>::type ConstructItems(T* pDest, const T* pSource, size_t pCount)
{
	while (pCount)
	{
		new (pDest)T(*pSource);
		++pDest;
		++pSource;
		--pCount;
	}
}

/* Construction for trivial type */
template <typename T>
FORCEINLINE typename std::enable_if<std::is_trivially_constructible<T>::value>::type ConstructItems(T* pDest, const T* pSource, size_t pCount)
{
	Memory::Memcpy(pDest, pSource, sizeof(T)*pCount);
}

/* Destruction for non-trivial type */
template <typename T>
FORCEINLINE typename std::enable_if<NOT std::is_trivially_destructible<T>::value>::type DestructItems(T* pData, size_t pCount)
{
	while (pCount)
	{
		pData->T::~T();
		++pData;
		--pCount;
	}
}

/* Destruction for trivial type */
template <typename T>
FORCEINLINE typename std::enable_if<std::is_trivially_destructible<T>::value>::type DestructItems(T* pData, size_t pCount)
{
	// Nothing to do
}