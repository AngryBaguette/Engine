#pragma once

#include "Common.hpp"
#include "CoreConfig.hpp"
#include "RefCounted.hpp"
#include "Array.hpp"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class Core_EXPORT DataBuffer : public RefCounted
{
public:
	/** Create and allocate **/
	static DataBuffer* create(size_t pSize = 0);

	FORCEINLINE uint8_t* data() { return &mData[0]; }

	/** The size in bytes of the buffer **/
	FORCEINLINE void resize(size_t pSize) { mData.resize(pSize); }

	/** The size in bytes of the buffer **/
	FORCEINLINE size_t size() const { return mData.dataSize(); }

protected:
	/** Destructor **/
	~DataBuffer() = default;

	/** Forbid default constructor **/
	DataBuffer() = default;

	/** Constructor **/
	DataBuffer(size_t pSize);

	/** The data **/
#pragma warning( push )
#pragma warning( disable : 4251 )
	Array<uint8_t> mData;
#pragma warning(pop)
};

Core_EXPORT typedef RefPointer<DataBuffer> DataBufferPtr;