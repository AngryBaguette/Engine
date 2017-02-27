#pragma once

#include <CoreConfig.hpp>
#include <RefCounted.hpp>
#include <DataBuffer.hpp>
#include "RHIResource.hpp"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/**
 * Contain indexes
 */
class Core_EXPORT IndexBuffer : public RefCounted
{
public:

public:
	/** Create and allocate an index buffer **/
	static IndexBuffer* create(uint32_t pCount, EIndexFormat pFormat = EIndexFormat::Int32);

	/**  **/
	FORCEINLINE void setNumOfIndices(uint32_t pCount) { mCount = pCount;  mData->resize(mCount); }

	/** Number of indexes **/
	FORCEINLINE uint32_t count() const { return mCount; }

	/** Get the stride between index **/
	FORCEINLINE uint32_t stride() const;

	/** device raw data **/
	FORCEINLINE DataBuffer* data() const { return mData.get(); }

protected:
	/** Forbid default constructor **/
	IndexBuffer() = delete;

	/** Constructor **/
	IndexBuffer(uint32_t pCount, EIndexFormat pFormat);

	/** The format of index buffer **/
	EIndexFormat mFormat;

	/** Count indices **/
	uint32_t mCount;

	/** The index array **/
#pragma warning( push )
#pragma warning( disable : 4251 )
	DataBufferPtr mData;
#pragma warning(pop)
	
};

/*****************************************************************************/
Core_EXPORT typedef RefPointer<IndexBuffer> IndexBufferPtr;