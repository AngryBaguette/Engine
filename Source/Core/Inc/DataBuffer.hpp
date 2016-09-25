#pragma once

#include "Common.hpp"
#include "CoreConfig.hpp"
#include "RefCounted.hpp"
#include "Array.hpp"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class DataBuffer : public RefCounted
{
public:
	/** Create and allocate **/
	static DataBuffer* create(size_t pSize = 0);

	FORCEINLINE uint8_t* data() { return &mData[0]; }

	/** The size in bytes of the buffer **/
	FORCEINLINE size_t size() const;

protected:
	/** Destructor **/
	~DataBuffer() = default;

	/** Forbid default constructor **/
	DataBuffer() = default;

	/** Constructor **/
	DataBuffer(size_t pSize);

	/** The data **/
	Array<uint8_t> mData;
};

/*****************************************************************************/
FORCEINLINE size_t DataBuffer::size() const
{
	TODO;
	return 0;
}