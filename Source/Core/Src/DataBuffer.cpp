#include <DataBuffer.h>

/*****************************************************************************/
DataBuffer* DataBuffer::create(size_t pSize)
{
	return new DataBuffer(pSize);
}

/*****************************************************************************/
DataBuffer::DataBuffer(size_t pSize)
{
	mData.resize(pSize);
}
