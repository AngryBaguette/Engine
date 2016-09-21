#include "Memory.hpp"

#include <string.h>


IMalloc* Memory::msMalloc = NULL;

void Memory::SetMalloc(IMalloc* pMalloc)
{
	assert(msMalloc == NULL);
	msMalloc = pMalloc;
}

void* Memory::Malloc(size_t pSize)
{
	return msMalloc->Malloc(pSize);
}

void* Memory::Realloc(void* pBlock, size_t pSize)
{
	return msMalloc->Realloc(pBlock, pSize);
}

void Memory::Free(void* pBlock)
{
	msMalloc->Free(pBlock);
}

void* Memory::AlignedMalloc(size_t pSize, size_t pAlign)
{
	return msMalloc->AlignedMalloc(pSize, pAlign);
}

void Memory::AlignedFree(void* pBlock)
{
	msMalloc->AlignedFree(pBlock);
}

void Memory::Memcpy(void* pDst, const void* pSrc, size_t pSize)
{
	memcpy(pDst, pSrc, pSize);
}

void Memory::Memmove(void* pDst, const void* pSrc, size_t pSize)
{
	memmove(pDst, pSrc, pSize);

}
void Memory::Memset(void* pDst, uint8_t pValue, size_t pSize)
{
	memset(pDst, pValue, pSize);
}