#pragma once

#include <RefCounted.h>

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/** 
 * Primitive
 */
class Primitive : public RefCounted
{
public:

protected:
	/** Forbid **/
	Primitive() = delete;

	/** Destructor **/
	~Primitive();

	/** The vertex buffers **/
	std::vector<RefPointer<VertexBuffer>> mVertexBuffers;

	/** The index buffer **/
	RefPointer<IndexBuffer> mIndexBuffer;

	/** The primitive type **/
	RefPointer<PolygonBuffer> mPolygonBuffer;
};