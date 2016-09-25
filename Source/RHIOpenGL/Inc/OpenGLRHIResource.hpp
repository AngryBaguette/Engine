#pragma once 
#include <Common.hpp>
#include <RHIResource.hpp>
#include <GL/glew.h>

// Abstraction for all OpenGL Buffer
// For unifom buffer, it's your job to ensure memory to be aligned
// Target : https://www.opengl.org/sdk/docs/man/html/glBindBuffer.xhtml
template< typename BaseType, GLenum Target >
class OpenGLBufferResource : public BaseType
{
public:
	/* Create the Buffer */
	OpenGLBufferResource(uint32_t pSize, EBufferUsage pUsage, const void* pData)
		: BaseType(pSize, pUsage)
	{
		glGenBuffers(1, &mBuffer);
		CHECKGLERROR;

		assert(mBuffer >= 0);
		glBindBuffer(Target, mBuffer);
		CHECKGLERROR;

		glBufferData(Target, mSize, pData, getUsage());
		CHECKGLERROR;
	}

	/* Create the Buffer */
	OpenGLBufferResource(uint8_t pStride, uint32_t pSize, EBufferUsage pUsage, const void* pData)
		: BaseType(pStride, pSize, pUsage)
	{
		glGenBuffers(1, &mBuffer);
		CHECKGLERROR;

		assert(mBuffer >= 0);
		glBindBuffer(Target, mBuffer);
		CHECKGLERROR;

		glBufferData(Target, mSize, pData, getUsage());
		CHECKGLERROR;
	}

	/* Access the buffer */
	void* lock(uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess)
	{
		assert(pOffset + pSize <= mSize);
		glBindBuffer(Target, mBuffer);
		CHECKGLERROR;

		void* lBuffer = glMapBufferRange(Target, pOffset, pSize, getAccess(pAccess));
		CHECKGLERROR;

		assert(lBuffer);
		return lBuffer;
	}

	/* Unlock the buffer */
	void unlock()
	{
		GLboolean lSuccess = glUnmapBuffer(Target);
		CHECKGLERROR;
		assert(lSuccess);
	}

protected:

	/* Convert to GL Usage enum */
	GLenum getUsage() const
	{
		assert((uint32_t)mUsage < 2);
		static const GLenum lsToGL[2] = { GL_STATIC_DRAW, GL_DYNAMIC_DRAW };
		return lsToGL[(uint8_t)mUsage];
	}

	/* Convert to GL Access enum */
	GLenum getAccess(EBufferAccess pAccess) const
	{
		assert((uint32_t)pAccess < 3);
		static const GLenum lsToGL[3] = { GL_MAP_READ_BIT, GL_MAP_WRITE_BIT , GL_MAP_READ_BIT | GL_MAP_WRITE_BIT };
		return lsToGL[(uint8_t)pAccess];
	}

protected:
	GLuint mBuffer;

};
typedef OpenGLBufferResource<VertexBufferResource, GL_ARRAY_BUFFER> OpenGLVertexBufferResource;
typedef OpenGLBufferResource<IndexBufferResource, GL_ELEMENT_ARRAY_BUFFER> OpenGLIndexBufferResource;