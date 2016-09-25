#pragma once 
#include "Common.hpp"
#include "RefCounted.hpp"


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


/** Device resource object */
class RenderResource : public RefCounted
{
};

/**
*/
class VertexBufferResource : public RenderResource
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


class IndexBufferResource : public RenderResource
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


class VertexShaderResource : public RenderResource
{

};
typedef RefPointer<VertexShaderResource> VertexShaderResourcePtr;

class FragmentShaderResource : public RenderResource
{

};
typedef RefPointer<FragmentShaderResource> FragmentShaderResourcePtr;

class ProgramResource : public RenderResource
{

};
typedef RefPointer<ProgramResource> ProgramResourcePtr;