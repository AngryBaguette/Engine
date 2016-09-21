#pragma once

#include <RefCounted.h>
#include <DataBuffer.h>

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/**
 * Contain indexes
 */
class IndexBuffer : public RefCounted
{
public:
	/** The format of index **/
	enum EFormat
	{
		eUInt16,
		eUInt32,
		eCountFormat
	};

public:
	/** Create and allocate an index buffer **/
	static IndexBuffer* create(uint32_t pCount, EFormat pFormat = eUInt16);

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
	IndexBuffer(uint32_t pCount, EFormat pFormat = eUInt16);

	/** The format of index buffer **/
	EFormat mFormat;

	/** Count indices **/
	uint32_t mCount;

	/** The index array **/
	RefPointer<DataBuffer> mData;
};

/*****************************************************************************/
FORCEINLINE uint32_t IndexBuffer::stride() const
{
	switch
		(mFormat)
	{
	case eUInt16: return 2;
	case eUInt32: return 4;
	default: break;
	}

	assert(false);
	return 0;
}