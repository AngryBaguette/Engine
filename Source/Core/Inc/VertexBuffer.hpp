#pragma once

#include <CoreConfig.hpp>
#include <RefCounted.hpp>
#include <DataBuffer.hpp>
#include <RHIResource.hpp>

// STL
#include <vector>

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/**
 * Contain vertices
 *
 * Example of creation of a vertex buffer with a position, normal and texture coordinate (v3fv3fv2f)
 * vb = VertexBuffer::create()
 * vb->addAttribute( VertexBuffer::Position, EVertexAttributeFormat::Float3);
 * vb->addAttribute( VertexBuffer::Normal, EVertexAttributeFormat::Float3 );
 * vb->addAttribute( VertexBuffer::TexCoord0, EVertexAttributeFormat::Float2 );
 * vb->setNumOfVertices();
 */
class Core_EXPORT VertexBuffer : public RefCounted
{
public:
	/** The attribute semantic **/
	enum class ESemantic
	{
		Position,
		Color,
		Normal,
		Binormal,
		Tangent,
		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3, 
		TexCoord4,
		TexCoord5,
		TexCoord6,
		TexCoord7,

		Count
	};

	/** Attribute ddescription of each attribute array **/
	struct SAttributeDesc
	{
		ESemantic				mSemantic;		// The semantic
		EVertexAttributeFormat	mFormat;		// Float1/Float2...
		uint32_t				mOffset;		// offset in bytes
	};

public:
	/** Create the vertex buffer **/
	static VertexBuffer* create();

	/** Delete the vertex buffer **/
	void destroy();

	/** Add an attribute **/
	void addAttribute(ESemantic pSemantic, EVertexAttributeFormat pFormat);

	/** Allocate the vertex buffer data **/
	void setNumOfVertices(uint32_t pCount);

	/** Copy 
	 * This method is for convenience. NOT OPTIMAL AT ALL
	 * @param pSemantic The semantic to write.
	 * @param pIndex The index of the vertex to set
	 * @param pSrc Where to read the value
	 * @param pCount Number of value to read
	 **/
	void setAttributeValue(ESemantic pSemantic, uint32_t pIndex, const uint8_t* pSrc, uint32_t pCount = 1);

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
	VertexBuffer();
	~VertexBuffer() = default;

	/** The size in bytes of one vertex **/
	uint32_t mVertexSize;

	/** The attributes description of the vertex buffer **/
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<SAttributeDesc>	mAttributes;
#pragma warning(pop)

	/** Count vertices **/
	uint32_t mCount;

	/** The data **/
#pragma warning( push )
#pragma warning( disable : 4251 )
	DataBufferPtr mData;
#pragma warning(pop)
};


//Core_TEMPLATE_EXPORT template class Core_EXPORT RefPointer<VertexBuffer>;
Core_EXPORT typedef RefPointer<VertexBuffer> VertexBufferPtr;