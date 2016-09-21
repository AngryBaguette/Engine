#pragma once
#include "Common.hpp"
#include "Malloc.hpp"

class Memory
{
public:
	static void SetMalloc(IMalloc* pMalloc);

	static void* Malloc(size_t pSize);

	static void* Realloc(void* pBlock, size_t pSize);

	static void Free(void* pBlock);

	static void* AlignedMalloc(size_t pSize, size_t pAlign);

	static void AlignedFree(void* pBlock);

	static void Memcpy(void* pDst, const void* pSrc, size_t pSize);

	static void Memmove(void* pDst, const void* pSrc, size_t pSize);

	static void Memset(void* pDst, uint8_t pValue, size_t pSize);

private:
	/**
	 * The base allocator
	 */
	static IMalloc*	msMalloc;

};