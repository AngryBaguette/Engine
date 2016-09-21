#include <IndexBuffer.hpp>

/*****************************************************************************/
/*static*/
IndexBuffer* IndexBuffer::create(uint32_t pCount, EFormat pFormat)
{
	return new IndexBuffer(pCount, pFormat);
}

/*****************************************************************************/
IndexBuffer::IndexBuffer(uint32_t pCount, EFormat pFormat)
: mFormat(pFormat)
, mCount(pCount)
, mData( DataBuffer::create(pCount * stride()) )
{
}