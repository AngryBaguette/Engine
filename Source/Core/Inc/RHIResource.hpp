#pragma once 
#include "Common.hpp"
#include "CoreConfig.hpp"
#include "RefCounted.hpp"
#include "Array.hpp"

enum class EPrimitiveType : uint8_t
{
	Triangles,
	TriangleFan,
	TriangleStrip,

	Lines,
	LineStrip,
	LineLoop,

	Points,

	Count
};

enum class EBufferUsage : uint8_t
{
	Static,
	Dynamic,

	Count
};

enum class EBufferAccess : uint8_t
{
	ReadOnly,
	WriteOnly,
	ReadWrite,

	Count
};

enum class EIndexFormat : uint8_t
{
	Int8,
	Int16,
	Int32,

	Count
};

/** Representation in memory of attribute data */
enum class EVertexAttributeFormat : uint8_t
{
	Float1,
	Float2,
	Float3,
	Float4,
	UByte4,
	UByte4N,	// Generally used for color

	Count
};

/** Device resource object */
class Core_EXPORT RenderResource : public RefCounted
{
};

/**
*/
class Core_EXPORT VertexBufferResource : public RenderResource
{
public:
	/* Constructor */
	VertexBufferResource(uint32_t pSize, EBufferUsage pUsage) : mSize(pSize), mUsage(pUsage) {}

	/* Get the buffer usage */
	EBufferUsage getUsage() const { return mUsage; }

	/* Get the data size */
	uint32_t getSize() const { return mSize; }

protected:
	EBufferUsage mUsage;
	uint32_t    mSize;
};
typedef RefPointer<VertexBufferResource> VertexBufferResourcePtr;


class Core_EXPORT IndexBufferResource : public RenderResource
{
public:
	IndexBufferResource(uint8_t pStride, uint32_t pSize, EBufferUsage pUsage) : mSize(pSize), mUsage(pUsage) {}

	/* Get the buffer usage */
	EBufferUsage getUsage() const { return mUsage; }

	/* Get the data size */
	uint32_t getSize() const { return mSize; }

	/* Stride between indices (generally 1-2-4)*/
	uint8_t getStride() const { return mStride; }

protected:
	EBufferUsage mUsage;
	uint32_t    mSize;
	uint8_t		mStride;
};
typedef RefPointer<IndexBufferResource> IndexBufferResourcePtr;


class Core_EXPORT VertexShaderResource : public RenderResource
{

};
typedef RefPointer<VertexShaderResource> VertexShaderResourcePtr;

class Core_EXPORT FragmentShaderResource : public RenderResource
{

};
typedef RefPointer<FragmentShaderResource> FragmentShaderResourcePtr;

class Core_EXPORT ProgramResource : public RenderResource
{

};
typedef RefPointer<ProgramResource> ProgramResourcePtr;


struct Core_EXPORT VertexAttributeDesc
{
	VertexAttributeDesc(EVertexAttributeFormat pFormat, uint8_t pOffset, uint8_t pStride, uint8_t pDivisor) : mStride(pStride), mOffset(pOffset), mFormat(pFormat), mDivisor(pDivisor) {}

	EVertexAttributeFormat mFormat;
	uint8_t mOffset;	// Offset in bytes in the vertex buffer where start the first vertex
	uint8_t mStride;	// Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
	uint8_t mDivisor;	// Modifies the rate at which generic vertex attributes advance when rendering multiple instances of primitives in a single draw call.If divisor is zero, the attribute at slot index advances once per vertex. If divisor is non-zero, the attribute advances once per divisor instances of the set(s) of vertices being rendered.
};



struct Core_EXPORT VertexInputLayout
{
	VertexInputLayout() = default;
	VertexInputLayout(const VertexInputLayout& pLayout)
	{
		mIndexBuffer = pLayout.mIndexBuffer;
		mVertexBuffers = pLayout.mVertexBuffers;	// VertexBuffer and its stream index
		mAttributes = pLayout.mAttributes;
	}

	/** Optional index buffer */
	void setIndexBuffer(IndexBufferResourcePtr& pIndexBuffer)
	{
		mIndexBuffer = pIndexBuffer;
	}

	/** Vertex buffer and its binding point */
	void addVertexBuffer(VertexBufferResourcePtr& pVertexBuffer, uint8_t pStreamIndex = -1)
	{
		uint8_t lStreamIndex = (uint8_t)mVertexBuffers.count();
		if (pStreamIndex NEQ - 1)
		{
			lStreamIndex = pStreamIndex;
		}
		mVertexBuffers.add(std::make_pair(pVertexBuffer, lStreamIndex));
	}

	/** Attribute declaration 
	* @param pVertexBufferIndex The associated vertex buffer index (if -1 associate to the last added vertex buffer)
	*/
	void addAttribute(const VertexAttributeDesc& pAttribute, uint8_t pVertexBufferIndex = 0xFF)
	{
		assert(mVertexBuffers.count() NEQ 0);
		uint8_t lVertexBufferIndex = (uint8_t)mVertexBuffers.count() - 1;
		if (pVertexBufferIndex NEQ 0xFF)
		{
			assert(pVertexBufferIndex < mVertexBuffers.count());
			lVertexBufferIndex = pVertexBufferIndex;
		}
		mAttributes.add(std::make_pair(pAttribute, lVertexBufferIndex));
	}

#pragma warning( push )
#pragma warning( disable : 4251 )
	IndexBufferResourcePtr mIndexBuffer;
	Array< std::pair<VertexBufferResourcePtr, uint8_t> > mVertexBuffers;	// VertexBuffer and its stream index
	Array< std::pair< VertexAttributeDesc, uint8_t > > mAttributes;			// Attribute and its vertex buffer
#pragma warning( pop )
};


class VertexInputLayoutResource : public RenderResource
{
public:
	VertexInputLayoutResource(const VertexInputLayout& pLayout) : mLayout(pLayout) {}

protected:
	VertexInputLayout mLayout;
};

typedef RefPointer<VertexInputLayoutResource> VertexInputLayoutResourcePtr;