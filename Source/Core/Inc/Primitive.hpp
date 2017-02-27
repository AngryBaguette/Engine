#pragma once

#include <CoreConfig.hpp>
#include <RefCounted.hpp>
#include <VertexBuffer.hpp>
#include <IndexBuffer.hpp>
#include <vector>

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

struct Core_EXPORT PrimitiveElement
{
	PrimitiveElement() : mType(EPrimitiveType::Triangles), mStartIndex(0), mNumOfIndices(0) {}
	PrimitiveElement(EPrimitiveType pType, uint32_t pStartIndex, uint32_t pNumOfIndices) : mType(pType), mStartIndex(pStartIndex), mNumOfIndices(pNumOfIndices) {  }
	EPrimitiveType	mType;
	uint32_t		mStartIndex;
	uint32_t		mNumOfIndices;
};

/** 
 * Primitive
 */
class Core_EXPORT Primitive : public RefCounted
{
public:
	/** **/
	static Primitive* create();

	/** **/
	void addVertexBuffer(VertexBuffer* pVB) { mVertexBuffers.push_back(pVB); }
	void setIndexBuffer(IndexBuffer* pIB) { mIndexBuffer = pIB;  }
	//void setElement( const PrimitiveElement& pElement)

protected:
	/** Forbid **/
	Primitive() = default;
	~Primitive() = default;

	/** The vertex buffers **/
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<VertexBufferPtr> mVertexBuffers;
#pragma warning( pop )

	/** The index buffer **/
	IndexBufferPtr mIndexBuffer;

	/** Draw element **/
	PrimitiveElement mElement;

};