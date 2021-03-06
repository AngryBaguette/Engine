#pragma once
#include "Common.hpp"
#include "CoreConfig.hpp"

/* Malloc interface */
class Core_EXPORT IIMalloc
{
public:
	virtual void* Malloc(size_t pSize) = 0;
	virtual void* Realloc(void* pBlock, size_t pSize) = 0;
	virtual void Free(void* pBlock) = 0;
	virtual void* AlignedMalloc(size_t pSize, size_t pAlign) = 0;
	virtual void AlignedFree(void* pBlock) = 0;
};

/* Default Malloc use malloc/free/realloc */
class Core_EXPORT DefaultMalloc : public IIMalloc
{
public:
	DefaultMalloc() {}
	~DefaultMalloc() {}
	virtual void* Malloc(size_t pSize) override;
	virtual void* Realloc(void* pBlock, size_t pSize) override;
	virtual void Free(void* pBlock) override;
	virtual void* AlignedMalloc(size_t pSize, size_t pAlign) override;
	virtual void AlignedFree(void* pBlock) override;
};