#pragma once

#include "CoreConfig.h"
#include "RefCounted.hpp"
#include "DataBuffer.hpp"

// STL
#include <vector>

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/**
 * Contain vertices
 *
 * Example of creation of a vertex buffer with a position, normal and texture coordinate (v3fv3fv2f)
 * vb = VertexBuffer::create(NumOfVertices)
 * vb->addAttribute( VertexBuffer::ePosition, 3, VertexBuffer::eFloat );
 * vb->addAttribute( VertexBuffer::eNormal, 3, VertexBuffer::eFloat );
 * vb->addAttribute( VertexBuffer::eTexCoord0, 2, VertexBuffer::eFloat );
 * vb->allocate();
 */
class Core_EXPORT VertexBuffer
{
public:
	/** The attribute semantic **/
	enum ESemantic
	{
		ePosition,
		eColor,
		eNormal,
		eBinormal,
		eTangent,
		eTexCoord0,
		eTexCoord1,
		eTexCoord2,
		eTexCoord3, 
		eTexCoord4,
		eTexCoord5,
		eTexCoord6,
		eTexCoord7,

		eCountSemantic
	};

	/** The type of data store in the array **/
	enum Type
	{
		// glVertexArrayAttribFormat
		// GL_BYTE, GL_SHORT, GL_INT, GL_FIXED, GL_FLOAT, GL_HALF_FLOAT, GL_DOUBLE, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV
		eFloat,
		eInt,
		eUInt,
		eByte,
		eUByte,

		eCountType
	};

	/** Attribute ddescription of each attribute array **/
	struct SAttributeDesc
	{
		ESemantic	mSemantic;		// The semantic
		uint32_t	mSize;			// 1 2 3 4
		Type		mType;			// float byte etc...
		bool		mNormalized;	// 
		uint32_t	mOffset;		// offset in bytes
	};

public:
	/** Create the vertex buffer **/
	static VertexBuffer* create(size_t pNumOfVertices = 0);

	/** Add an attribute **/
	void addAttribute(ESemantic pSemantic, uint32_t pSize, Type pType, bool pNormalized = false);

	/** Allocate the vertex buffer data **/
	void allocate();

	/** Copy 
	 * @param pSemantic The semantic to write.
	 * @param pIndex The index of the vertex to set
	 * @param pDataValue Where to read the value
	 * @param pCount Number of of value to read
	 **/
	void setAttributeValue(ESemantic pSemantic, uint32_t pIndex, uint8_t* pDataValue, uint32_t pCount = 0);

	/** Number of attributes **/
	FORCEINLINE uint32_t countAttributes() const { return static_cast<uint32_t>(mAttributes.size()); }

	/** Get the attribute description by its index **/
	FORCEINLINE const SAttributeDesc& attributeDesc(uint32_t pIndex) const { assert(pIndex < mAttributes.size()); return mAttributes[pIndex]; }

	/** Number of vertices **/
	FORCEINLINE uint32_t countVertices() const { return mCount; }

	/** Get the data buffer **/
	FORCEINLINE const DataBuffer* data() const { return mData.get(); }

protected:
	/** Forbid **/
	VertexBuffer() = delete;

	/** The constructor **/
	VertexBuffer(size_t NumOfVertices);

	/** The size in bytes of one vertex **/
	uint32_t mVertexSize;

	/** To know if the buffer has been allocated **/
	bool mIsAllocated;

	/** The attributes description of the vertex buffer **/
	std::vector<SAttributeDesc>	mAttributes;

	/** Count vertices **/
	uint32_t mCount;

	/** The data **/
	RefPointer<DataBuffer> mData;

	/** pType refer to the GLenum pass to the glVertexArray **/
	FORCEINLINE uint32_t getTypeSize(Type pType) const;
};

// Instantiate classes vector<int> and vector<char>
// This does not create an object. It only forces the generation of all
// of the members of classes vector<int> and vector<char>. It exports
// them from the DLL and imports them into the .exe file.
Core_TEMPLATE_EXPORT template class Core_EXPORT std::vector<VertexBuffer::SAttributeDesc>;


/*****************************************************************************/
FORCEINLINE uint32_t VertexBuffer::getTypeSize(Type pType) const
{
	assert(pType < eCountType);
	switch
		(pType)
	{
	case eFloat:
	case eInt:
	case eUInt:
		return 4;
	case eByte:
	case eUByte:
		return 1;
	default: 
		assert(false && "not managed yet");
		break;
	}

	assert(false);
	return 0;
}