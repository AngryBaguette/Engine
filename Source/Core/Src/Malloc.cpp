#include "Malloc.hpp"
#include <stdlib.h>


void* DefaultMalloc::Malloc(size_t pSize)
{
	return malloc(pSize);
}

void* DefaultMalloc::Realloc(void* pBlock, size_t pSize)
{
	return realloc(pBlock, pSize);
}

void DefaultMalloc::Free(void* pBlock)
{
	free(pBlock);
}

void* DefaultMalloc::AlignedMalloc(size_t pSize, size_t pAlign)
{
	return _aligned_malloc(pSize, pAlign);
}

void DefaultMalloc::AlignedFree(void* pBlock)
{
	_aligned_free(pBlock);
}