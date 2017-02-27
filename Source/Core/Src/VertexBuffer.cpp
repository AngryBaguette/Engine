#include "VertexBuffer.hpp"
#include <algorithm>

/*****************************************************************************/
VertexBuffer* VertexBuffer::create()
{
	return new VertexBuffer();
}

/*****************************************************************************/
void VertexBuffer::destroy()
{
	delete this;
}

/*****************************************************************************/
VertexBuffer::VertexBuffer()
: mCount(0)
, mVertexSize(0)
, mData(DataBuffer::create())
{
}

/*****************************************************************************/
void VertexBuffer::setNumOfVertices(uint32_t pCount)
{
	if
		(pCount NEQ mCount)
	{
		mData->resize(pCount * mVertexSize);
		mCount = pCount;
	}
}

/*****************************************************************************/
void VertexBuffer::setAttributeValue(ESemantic pSemantic, uint32_t pIndex, const uint8_t* pDataValue, uint32_t pCount)
{
	// Find the semantic
	auto lItr = std::find_if(mAttributes.begin(), mAttributes.end(), [pSemantic](const VertexBuffer::SAttributeDesc& pDesc) { return pDesc.mSemantic EQ pSemantic; });

	if
		(lItr NEQ mAttributes.end())
	{
		uint32_t lAttributeSize = TranslateVertexAttributeFormatToByte(lItr->mFormat);
		for (uint32_t i = 0; i < pCount; ++i)
		{
			Memory::Memcpy(mData->data() + lItr->mOffset + (pIndex + i)*mVertexSize, pDataValue + (i*mVertexSize), lAttributeSize);
		}
	}
}

/*****************************************************************************/
void VertexBuffer::addAttribute(ESemantic pSemantic, EVertexAttributeFormat pFormat)
{
	SAttributeDesc lDesc;
	lDesc.mSemantic = pSemantic;
	lDesc.mFormat = pFormat;
	lDesc.mOffset = mVertexSize;
	mAttributes.push_back(lDesc);

	// Size of one vertex
	mVertexSize += TranslateVertexAttributeFormatToByte(pFormat);
}
