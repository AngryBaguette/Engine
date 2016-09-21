#include "VertexBuffer.hpp"
#include <algorithm>

/*****************************************************************************/
VertexBuffer* VertexBuffer::create(size_t pNumOfVertices)
{
	return new VertexBuffer(pNumOfVertices);
}

/*****************************************************************************/
VertexBuffer::VertexBuffer(size_t pNumOfVertices)
: mCount((uint32_t)pNumOfVertices)
, mIsAllocated(false)
, mVertexSize(0)
{
}

/*****************************************************************************/
void VertexBuffer::allocate()
{
	assert(!mIsAllocated && ">> Already allocated <<");
	size_t lSize = 0;
	for (int i = 0; i < mAttributes.size(); ++i)
	{
		lSize += getTypeSize(mAttributes[i].mType) * mAttributes[i].mSize;
	}

	mData = DataBuffer::create(lSize);
	mIsAllocated = true;
}

/*****************************************************************************/
void VertexBuffer::setAttributeValue(ESemantic pSemantic, uint32_t pIndex, uint8_t* pDataValue, uint32_t pCount)
{
	assert(mIsAllocated && ">> Try to set value on non allocated vertex buffer <<");

	// Find the semantic
	auto lItr = std::find_if(mAttributes.begin(), mAttributes.end(), [pSemantic](const VertexBuffer::SAttributeDesc& pDesc) { return pDesc.mSemantic == pSemantic; });

	if
		(lItr != mAttributes.end())
	{
		uint32_t lAttributeSize = getTypeSize(lItr->mType) * lItr->mSize;
		for (uint32_t i = 0; i < pCount; ++i)
		{
			std::memcpy(mData->data() + lItr->mOffset + (pIndex + i)*mVertexSize, pDataValue + (i*mVertexSize), lAttributeSize);
		}
	}
}

/*****************************************************************************/
void VertexBuffer::addAttribute(ESemantic pSemantic, uint32_t pSize, Type pType, bool pNormalized)
{
	SAttributeDesc lDesc;
	lDesc.mSemantic = pSemantic;
	lDesc.mSize = pSize;
	lDesc.mType = pType;
	lDesc.mNormalized = pNormalized;
	lDesc.mOffset = mAttributes.empty() ? 0 : mAttributes.back().mOffset + mAttributes.back().mSize * getTypeSize(pType);
	mAttributes.push_back(lDesc);

	// Size of one vertex
	mVertexSize += pSize * getTypeSize(pType);
}
