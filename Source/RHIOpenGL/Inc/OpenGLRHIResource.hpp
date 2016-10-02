#pragma once 
#include <Common.hpp>
#include <RHIResource.hpp>
#include <GL/glew.h>

#define BUFFER_OFFSET(x) ((void*)((uint64_t)(x)))

static uint32_t TranslatePrimitiveTypeSize(EPrimitiveType pType, uint32_t pCount)
{
	switch (pType)
	{
	case EPrimitiveType::Triangles: return pCount * 3;
	default:
		TODO;
		break;
	}

	return pCount;
}

static GLenum TranslatePrimitiveType(EPrimitiveType pType)
{
	static GLenum lsToGL[] = { GL_TRIANGLES, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_POINTS };
	static_assert(sizeof(lsToGL) / sizeof(GLenum) EQ (uint8_t)EPrimitiveType::Count,"Enum count changed, reflect modification here");
	return lsToGL[(uint8_t)pType];
}

// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
static void TranslateVertexAttributeFormat(EVertexAttributeFormat pFormat, GLenum& pType, GLint& pSize, GLboolean& pIsNormalized)
{
	// EVertexAttributeFormat				= {	Float1	, Float2	, Float3	, Float4	, UByte4			, UByte4N			}
	static const GLenum lsToGLType[]		= {	GL_FLOAT, GL_FLOAT	, GL_FLOAT	, GL_FLOAT	, GL_UNSIGNED_BYTE	, GL_UNSIGNED_BYTE	};
	static const uint8_t lsToGLSize[]		= {	1		, 2			, 3			, 4			, 4					, 4					};
	static const bool lsToGLNormalized[]	= {	false	, false		, false		, false		, false				, false				};
	static_assert(sizeof(lsToGLType) / sizeof(GLenum) EQ (uint8_t)EVertexAttributeFormat::Count, "Enum count changed, reflect modification here");
	static_assert(sizeof(lsToGLSize) / sizeof(uint8_t) EQ (uint8_t)EVertexAttributeFormat::Count, "Enum count changed, reflect modification here");
	static_assert(sizeof(lsToGLNormalized) / sizeof(bool) EQ (uint8_t)EVertexAttributeFormat::Count, "Enum count changed, reflect modification here");

	pType = lsToGLType[(uint8_t)pFormat];
	pSize = lsToGLSize[(uint8_t)pFormat];
	pIsNormalized = lsToGLNormalized[(uint8_t)pIsNormalized];
}

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

	/** Get the opengl handle of the buffer */
	FORCEINLINE GLuint handle() const { return mBuffer; }

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



class OpenGLVertexInputLayoutResource : public VertexInputLayoutResource
{
public:
	OpenGLVertexInputLayoutResource(const VertexInputLayout& pLayout) : VertexInputLayoutResource(pLayout)
	{
		glGenVertexArrays(1, &mVAO); CHECKGLERROR;
		glBindVertexArray(mVAO); CHECKGLERROR;

		// Bind Vertex buffer
		// Assume attribute are sored by vertex buffer index
		for (size_t i = 0; i < mLayout.mVertexBuffers.count(); ++i)
		{
			OpenGLVertexBufferResource* lVB = static_cast<OpenGLVertexBufferResource*>(mLayout.mVertexBuffers[i].first.get());
			glBindBuffer(GL_ARRAY_BUFFER, lVB->handle()); CHECKGLERROR;

			for (size_t j = 0; j < mLayout.mAttributes.count(); ++j)
			{
				const VertexAttributeDesc& attr = mLayout.mAttributes[j].first;
				GLuint bindingPoint = mLayout.mAttributes[j].second;
				glEnableVertexAttribArray(bindingPoint); CHECKGLERROR;

				GLenum lType;
				GLint lSize;
				GLboolean lIsNormalized;
				TranslateVertexAttributeFormat(attr.mFormat, lType, lSize, lIsNormalized);
				glVertexAttribPointer(bindingPoint, lSize, lType, lIsNormalized, attr.mStride, BUFFER_OFFSET(attr.mOffset)); CHECKGLERROR;
				if(glVertexAttribDivisor)
					glVertexAttribDivisor(bindingPoint, attr.mDivisor); CHECKGLERROR;
			}
		}

		// Bind index buffer
		if (mLayout.mIndexBuffer)
		{
			OpenGLIndexBufferResource* lIB = static_cast<OpenGLIndexBufferResource*>(mLayout.mIndexBuffer.get());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lIB->handle()); CHECKGLERROR;
		}


		glBindVertexArray(0); CHECKGLERROR;
	}

	GLuint handle() const { return mVAO;  }

protected:
	GLuint mVAO;
};

typedef RefPointer<OpenGLVertexInputLayoutResource> OpenGLVertexInputLayoutResourcePtr;