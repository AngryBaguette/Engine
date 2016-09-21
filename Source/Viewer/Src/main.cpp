/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#include "Common.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>


#include "RefCounted.hpp"

#include <iostream>

template<typename T>
constexpr unsigned int ArraySize(T array)
{
	return sizeof(array) / sizeof(T);
}

#define CHECKGLERROR checkError(__FILE__,__LINE__)

/** Use KHR_debug extension */
void checkError(const char* pFile, int32_t pLine)
{
#define GL_GET_DEBUG_MESSAGE_LOG_COUNT 10
	char lBuffer[2048];
	GLsizei lLengths[GL_GET_DEBUG_MESSAGE_LOG_COUNT];
	GLenum lSeverities[GL_GET_DEBUG_MESSAGE_LOG_COUNT];

	GLuint count = GL_GET_DEBUG_MESSAGE_LOG_COUNT;
	GLsizei bufSize = sizeof(lBuffer);
	GLenum *sources = NULL;
	GLenum *types = NULL;
	GLuint *ids = NULL;
	GLenum *severities = lSeverities;
	GLsizei *lengths = lLengths;
	GLchar *messageLog = lBuffer;
	GLuint lNbMessage = glGetDebugMessageLog(count, bufSize, sources, types, ids, severities, lengths, messageLog);
	if
		(lNbMessage)
	{
		unsigned int offset = 0;
		for
			(unsigned int i = 0; i < lNbMessage; ++i)
		{
			std::cout << messageLog + offset << std::endl;
			offset += lengths[i];
		}
	}
}


// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glVertexAttribPointer.xml
struct VertexAttribute
{
	uint32_t mIndex;    // The index of the generic vertex attribute
	uint32_t mTypeSize;    // The number of components per generic vertex attribute. Must be 1, 2, 3, or 4
	uint32_t mType;        // GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_FIXED, or GL_FLOAT
	uint32_t mOffset;    // Offset in bytes in the vertex buffer where start the first vertex
	uint32_t mStride;    // Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
	uint32_t mDivisor;    // Modifies the rate at which generic vertex attributes advance when rendering multiple instances of primitives in a single draw call.If divisor is zero, the attribute at slot index advances once per vertex. If divisor is non-zero, the attribute advances once per divisor instances of the set(s) of vertices being rendered.
	bool mIsNormalized;    // Specifies whether fixed-point data values should be normalized or converted directly as fixed-point values when they are accessed.
};


class VertexAttributeDeclaration
{

};

struct ViewportState
{

};

struct ScissorState
{
	bool        mEnable;
	uint32_t    mX;            // Bottom
	uint32_t    mY;            // Left
	uint32_t    mWidth;
	uint32_t    mHeight;
};

struct VertexAttributeInfo
{
	GLenum Type;
	GLuint StreamIndex;
	GLuint Offset;
	GLuint Size;
	GLuint Divisor;
	uint8_t bNormalized;
	uint8_t AttributeIndex;
	uint8_t bShouldConvertToFloat;
};

struct OpenGLState
{
	glm::vec4    mClearColor;
	float        mDepthClearValue;
	uint32_t    mStencilClearValue;

	ScissorState mScissorState;
};


OpenGLState CurrentOpenGLState;
OpenGLState PendingOpenGLState;

enum class BufferUsage : uint32_t
{
	eStatic,
	eDynamic,

	eCount
};

enum class BufferAccess : uint32_t
{
	eReadOnly,
	eWriteOnly,
	eReadWrite,
	eCount
};

/**Device resource object*/
class ResourceObject : public RefCounted
{

};

/**
*/
class VertexBufferObject : public ResourceObject
{
public:
	/* Constructor */
	VertexBufferObject(uint32_t pSize, BufferUsage pUsage) : mSize(pSize), mUsage(pUsage) {}

	/* Get the buffer usage */
	BufferUsage getUsage() const { return mUsage; }

	/* Get the data size */
	uint32_t getSize() const { return mSize; }

protected:
	BufferUsage mUsage;
	uint32_t    mSize;
};
typedef RefPointer<VertexBufferObject> VertexBufferObjectPtr;

// Abstraction for all OpenGL Buffer
// For unifom buffer, it's your job to ensure memory to be aligned
// Target : https://www.opengl.org/sdk/docs/man/html/glBindBuffer.xhtml
template< typename BaseType, GLenum Target >
class OpenGLBufferObject : public BaseType
{
public:
	/* Create the Buffer */
	OpenGLBufferObject(uint32_t pSize, BufferUsage pUsage, const void* pData)
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

	/* Access the buffer */
	void* lock(uint32_t pOffset, uint32_t pSize, BufferAccess pAccess)
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
		return lsToGL[(uint32_t)mUsage];
	}

	/* Convert to GL Access enum */
	GLenum getAccess(BufferAccess pAccess) const
	{
		assert((uint32_t)pAccess < 3);
		static const GLenum lsToGL[3] = { GL_MAP_READ_BIT, GL_MAP_WRITE_BIT , GL_MAP_READ_BIT | GL_MAP_WRITE_BIT };
		return lsToGL[(uint32_t)pAccess];
	}

protected:
	GLuint mBuffer;

};
typedef OpenGLBufferObject<VertexBufferObject, GL_ARRAY_BUFFER> OpenGLVertexBuffer;

void RHIClear(bool pColor, const glm::vec4& pClearColor, bool pDepth, float pDepthValue, bool pStencil, uint32_t pStencilValue)
{
	GLbitfield lMask = 0;
	if (pColor)
	{
		lMask |= GL_COLOR_BUFFER_BIT;
		if (pClearColor != CurrentOpenGLState.mClearColor)
		{
			glClearColor(pClearColor.r, pClearColor.g, pClearColor.b, pClearColor.a);
		}
	}
	if (pDepth)
	{
		lMask |= GL_DEPTH_BUFFER_BIT;
		glClearDepthf(pDepthValue);
	}
	if (pStencil)
	{
		lMask |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(pStencilValue);
	}

	if (lMask)
	{
		glClear(lMask);
	}
}

void RHISetScissor(bool pEnable, const glm::i32vec4& pRect)
{
	if (pEnable)
	{
		glEnable(GL_SCISSOR_TEST);
		CHECKGLERROR;
		glScissor(pRect.x, pRect.y, pRect.z, pRect.w);
		CHECKGLERROR;
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
		CHECKGLERROR;
	}
}

void RHISetViewport(const glm::i32vec4& pViewport, const glm::vec2& pDepthRange)
{
	glViewport(pViewport.x, pViewport.y, pViewport.z, pViewport.w);
	CHECKGLERROR;
	glDepthRange(pDepthRange.x, pDepthRange.y);
	CHECKGLERROR;
}

VertexBufferObjectPtr RHICreateVertexBuffer(uint32_t pSize, BufferUsage pUsage, const void* pData)
{
	return new OpenGLVertexBuffer(pSize, pUsage, pData);
}

void* RHILockVertexBuffer(VertexBufferObjectPtr& pVB, uint32_t pOffset, uint32_t pSize, BufferAccess pAccess)
{
	OpenGLVertexBuffer* lVB = static_cast<OpenGLVertexBuffer*>(pVB.get());
	void* lData = lVB->lock(pOffset, pSize, pAccess);
	return lData;
}

void RHIUnlockVertexBuffer(VertexBufferObjectPtr& pVB)
{
	OpenGLVertexBuffer* lVB = static_cast<OpenGLVertexBuffer*>(pVB.get());
	lVB->unlock();
}


void RHICreateVertexAttributeDeclaration()
{
	// glGenVertexArrays();
}

void RHISetVertexAttribute(uint32_t pIndex, VertexBufferObjectPtr& pVB, uint32_t pStride, uint32_t pOffset)
{
	//glVertexAttribPointer(pIndex, )
}

void RHIDraw()
{

}

/*****************************************************************************/
bool initScene()
{
	float positions[4][3] =
	{
		{ -0.25f, -0.25f, 0.0f },
		{ -0.25f, 0.25f, 0.0f },
		{ 0.25f, 0.25f, 0.0f },
		{ 0.25F, -0.25F, 0.0f }
	};

	//


	VertexBufferObjectPtr vb = RHICreateVertexBuffer(sizeof(positions), BufferUsage::eStatic, positions);
	RHILockVertexBuffer(vb, 0, vb->getSize(), BufferAccess::eWriteOnly);
	RHIUnlockVertexBuffer(vb);

	/*
	VertexBuffer* vb = VertexBuffer::create(4);
	vb->addAttribute(VertexBuffer::ePosition, 3, VertexBuffer::eFloat);
	//vb->addAttribute(VertexBuffer::eNormal, 3, VertexBuffer::eFloat);
	//vb->addAttribute(VertexBuffer::eTexCoord0, 2, VertexBuffer::eFloat);
	vb->allocate();
	vb->setAttributeValue(VertexBuffer::ePosition, 0, (uint8_t*)positions, 4);
	*/

	// Load sponza
	/*
	std::string sponzaFile = gDataDir + "Models/sponza.obj";
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(sponzaFile,
	aiProcess_PreTransformVertices |
	aiProcess_CalcTangentSpace |
	aiProcess_Triangulate |
	aiProcess_JoinIdenticalVertices |
	aiProcess_SortByPType);
	*/

	return true;
}


/*****************************************************************************/
void resize(int pWidth, int pHeight)
{
	RHISetViewport(glm::i32vec4(0, 0, pWidth, pHeight), glm::vec2(0, 0));
}

/*****************************************************************************/
void render()
{
	RHIClear(true, glm::vec4(0, 0, 0, 0), true, 0.0f, false, 0);

	glutSwapBuffers();
}

/*****************************************************************************/
void idle()
{
	render();
}


/*****************************************************************************/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);

	glutCreateWindow("GLUT Viewer");

	glutDisplayFunc(render);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);


	GLenum err = glewInit();
	assert(err == GLEW_OK);

	if (GLEW_VERSION_4_1)
	{
		std::cout << "Driver supports OpenGL 4.1\nDetails:" << std::endl;
	}

	if
		(GLEW_KHR_debug)
	{
		glEnable(GL_DEBUG_OUTPUT);
		//glDebugMessageCallback(debugFunc, NULL);
	}



	initScene();

	glutMainLoop();

	return 0;
}