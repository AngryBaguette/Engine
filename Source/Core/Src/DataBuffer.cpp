#include <DataBuffer.hpp>

/*****************************************************************************/
DataBuffer* DataBuffer::create(size_t pSize)
{
	return new DataBuffer(pSize);
}

/*****************************************************************************/
DataBuffer::DataBuffer(size_t pSize)
{
	TODO;
	//mData.resize(pSize);
}
