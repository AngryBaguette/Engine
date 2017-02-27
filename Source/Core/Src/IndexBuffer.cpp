#include <IndexBuffer.hpp>

/*****************************************************************************/
/*static*/
IndexBuffer* IndexBuffer::create(uint32_t pCount, EIndexFormat pFormat)
{
	return new IndexBuffer(pCount, pFormat);
}

/*****************************************************************************/
IndexBuffer::IndexBuffer(uint32_t pCount, EIndexFormat pFormat)
: mFormat(pFormat)
, mCount(pCount)
, mData( DataBuffer::create(pCount * TranslateIndexFormatToByte(pFormat) ) )
{
}