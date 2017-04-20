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

/** Total size in byte **/
static FORCEINLINE uint8_t TranslateIndexFormatToByte(EIndexFormat pFormat)
{
	static uint8_t lsToByte[] = { 1, 2, 4 };
	static_assert(ARRAY_COUNT(lsToByte) EQ (uint8_t)EIndexFormat::Count, "Enum count changed, reflect modification here");
	return lsToByte[(uint8_t)pFormat];
}

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

/** Total size in byte of the attribute **/
static FORCEINLINE uint8_t TranslateVertexAttributeFormatToByte(EVertexAttributeFormat pFormat)
{
	static uint8_t lsToByte[] = { 4, 8, 12, 16, 4, 4 };
	static_assert(ARRAY_COUNT(lsToByte) EQ (uint8_t)EVertexAttributeFormat::Count, "Enum count changed, reflect modification here");
	return lsToByte[(uint8_t)pFormat];
}



/** Device resource object */
class Core_EXPORT RHIResource : public RefCounted
{
protected:

};



/** **/
class SamplerStateRHI : public RHIResource {};

/** **/
class RasterizerStateRHI : public RHIResource {};

/**  **/
class DepthStencilStateRHI : public RHIResource {};

/** **/
class BlendStateRHI : public RHIResource {};


/** **/
class Core_EXPORT VertexBufferRHI : public RHIResource
{
public:
	/* Constructor */
	VertexBufferRHI(uint32_t pSize, EBufferUsage pUsage) : mSize(pSize), mUsage(pUsage) {}

	/* Get the buffer usage */
	EBufferUsage getUsage() const { return mUsage; }

	/* Get the data size */
	uint32_t getSize() const { return mSize; }

protected:
	EBufferUsage mUsage;
	uint32_t    mSize;
};
typedef RefPointer<VertexBufferRHI> VertexBufferRHIPtr;

/** **/
class Core_EXPORT IndexBufferRHI : public RHIResource
{
public:
	IndexBufferRHI(uint8_t pStride, uint32_t pSize, EBufferUsage pUsage) : mSize(pSize), mUsage(pUsage) {}

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
typedef RefPointer<IndexBufferRHI> IndexBufferRHIPtr;


class Core_EXPORT VertexShaderRHI : public RHIResource
{

};
typedef RefPointer<VertexShaderRHI> VertexShaderRHIPtr;

class Core_EXPORT FragmentShaderRHI : public RHIResource
{

};
typedef RefPointer<FragmentShaderRHI> FragmentShaderRHIPtr;

class Core_EXPORT ProgramRHI : public RHIResource
{

};
typedef RefPointer<ProgramRHI> ProgramRHIPtr;


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
	void setIndexBuffer(IndexBufferRHIPtr& pIndexBuffer)
	{
		mIndexBuffer = pIndexBuffer;
	}

	/** Vertex buffer and its binding point */
	void addVertexBuffer(VertexBufferRHIPtr& pVertexBuffer, uint8_t pStreamIndex = -1)
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
	IndexBufferRHIPtr mIndexBuffer;
	Array< std::pair<VertexBufferRHIPtr, uint8_t> > mVertexBuffers;	// VertexBuffer and its stream index
	Array< std::pair< VertexAttributeDesc, uint8_t > > mAttributes;			// Attribute and its vertex buffer
#pragma warning( pop )
};


class VertexInputLayoutRHI : public RHIResource
{
public:
	VertexInputLayoutRHI(const VertexInputLayout& pLayout) : mLayout(pLayout) {}

protected:
	VertexInputLayout mLayout;
};

typedef RefPointer<VertexInputLayoutRHI> VertexInputLayoutRHIPtr;