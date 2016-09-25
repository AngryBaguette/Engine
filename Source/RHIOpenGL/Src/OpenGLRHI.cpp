#include "OpenGLRHI.hpp"
#include "OpenGLRHIResource.hpp"

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <iostream>

/************************************************************************/
IDynamicRHI* CreateDynamicRHI()
{
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
	}

	return new OpenGLRHI();
}

/************************************************************************/
void OpenGLRHI::CheckError(const char* pFile, int32_t pLine)
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

/************************************************************************/
bool OpenGLRHI::CheckShaderCompilStatus(uint32_t pShader)
{
	// Check Fragment Shader
	GLint result;
	GLint messageLength;
	glGetShaderiv(pShader, GL_COMPILE_STATUS, &result); CHECKGLERROR;
	glGetShaderiv(pShader, GL_INFO_LOG_LENGTH, &messageLength); CHECKGLERROR;
	if (messageLength > 0) {
		std::vector<char> message(messageLength + 1);
		glGetShaderInfoLog(pShader, messageLength, NULL, &message[0]); CHECKGLERROR;
		printf("%s\n", &message[0]);
	}

	return result EQ GL_TRUE;
}

/************************************************************************/
bool OpenGLRHI::CheckProgramLinkStatus(uint32_t pShader)
{
	// Check Fragment Shader
	GLint result;
	GLint messageLength;
	glGetProgramiv(pShader, GL_LINK_STATUS, &result); CHECKGLERROR;
	glGetProgramiv(pShader, GL_INFO_LOG_LENGTH, &messageLength); CHECKGLERROR;
	if (messageLength > 0) {
		std::vector<char> message(messageLength + 1);
		glGetProgramInfoLog(pShader, messageLength, NULL, &message[0]); CHECKGLERROR;
		printf("%s\n", &message[0]);
	}

	return result EQ GL_TRUE;
}

/************************************************************************/
void OpenGLRHI::RHIClear(bool pColor, const glm::vec4& pClearColor, bool pDepth, float pDepthValue, bool pStencil, uint32_t pStencilValue)
{
	GLbitfield lMask = 0;
	if (pColor)
	{
		lMask |= GL_COLOR_BUFFER_BIT;
		/*if (pClearColor != CurrentOpenGLState.mClearColor)
		{*/
		glClearColor(pClearColor.r, pClearColor.g, pClearColor.b, pClearColor.a); CHECKGLERROR;
		/*}*/
	}
	if (pDepth)
	{
		lMask |= GL_DEPTH_BUFFER_BIT;
		glClearDepthf(pDepthValue); CHECKGLERROR;
	}
	if (pStencil)
	{
		lMask |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(pStencilValue); CHECKGLERROR;
	}

	if (lMask)
	{
		glClear(lMask); CHECKGLERROR;
	}
}

/************************************************************************/
void OpenGLRHI::RHISetScissor(bool pEnable, const glm::i32vec4& pRect)
{
	if (pEnable)
	{
		glEnable(GL_SCISSOR_TEST); CHECKGLERROR;
		glScissor(pRect.x, pRect.y, pRect.z, pRect.w); CHECKGLERROR;
	}
	else
	{
		glDisable(GL_SCISSOR_TEST); CHECKGLERROR;
	}
}

/************************************************************************/
void OpenGLRHI::RHISetViewport(const glm::i32vec4& pViewport, const glm::vec2& pDepthRange)
{
	glViewport(pViewport.x, pViewport.y, pViewport.z, pViewport.w);	CHECKGLERROR;
	glDepthRange(pDepthRange.x, pDepthRange.y);	CHECKGLERROR;
}

/************************************************************************/
VertexBufferResourcePtr OpenGLRHI::RHICreateVertexBuffer(uint32_t pSize, EBufferUsage pUsage, const void* pData)
{
	return new OpenGLVertexBufferResource(pSize, pUsage, pData);
}

/************************************************************************/
void* OpenGLRHI::RHILockVertexBuffer(VertexBufferResourcePtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess)
{
	OpenGLVertexBufferResource* lVB = static_cast<OpenGLVertexBufferResource*>(pVB.get());
	void* lData = lVB->lock(pOffset, pSize, pAccess);
	return lData;
}

/************************************************************************/
void OpenGLRHI::RHIUnlockVertexBuffer(VertexBufferResourcePtr& pVB)
{
	OpenGLVertexBufferResource* lVB = static_cast<OpenGLVertexBufferResource*>(pVB.get());
	lVB->unlock();
}

/************************************************************************/
IndexBufferResourcePtr OpenGLRHI::RHICreateIndexBuffer(uint8_t pStride, uint32_t pSize, EBufferUsage pUsage, const void* pData)
{
	return new OpenGLIndexBufferResource(pStride, pSize, pUsage, pData);
}

/************************************************************************/
void* OpenGLRHI::RHILockIndexBuffer(IndexBufferResourcePtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess)
{
	OpenGLIndexBufferResource* lVB = static_cast<OpenGLIndexBufferResource*>(pVB.get());
	void* lData = lVB->lock(pOffset, pSize, pAccess);
	return lData;
}

/************************************************************************/
void OpenGLRHI::RHIUnlockIndexBuffer(IndexBufferResourcePtr& pVB)
{
	OpenGLIndexBufferResource* lVB = static_cast<OpenGLIndexBufferResource*>(pVB.get());
	lVB->unlock();
}