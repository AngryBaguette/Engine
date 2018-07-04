#include "OpenGLRHI.hpp"
#include "OpenGLRHIResource.hpp"
#include "OpenGLShader.hpp"

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <iostream>

/************************************************************************/
IDynamicRHI* CreateDynamicRHI()
{
	GLenum err = glewInit();
	assert(err == GLEW_OK);

	if (GLEW_VERSION_4_3)
	{
		std::cout << "Driver supports OpenGL 4.3\nDetails:" << std::endl;
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

			assert(lSeverities[i] != GL_DEBUG_SEVERITY_HIGH);
		}
	}
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
VertexBufferRHIPtr OpenGLRHI::RHICreateVertexBuffer(uint32_t pSize, EBufferUsage pUsage, const void* pData)
{
	return new OpenGLVertexBufferResource(pSize, pUsage, pData);
}

/************************************************************************/
void* OpenGLRHI::RHILockVertexBuffer(VertexBufferRHIPtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess)
{
	OpenGLVertexBufferResource* lVB = static_cast<OpenGLVertexBufferResource*>(pVB.get());
	void* lData = lVB->lock(pOffset, pSize, pAccess);
	return lData;
}

/************************************************************************/
void OpenGLRHI::RHIUnlockVertexBuffer(VertexBufferRHIPtr& pVB)
{
	OpenGLVertexBufferResource* lVB = static_cast<OpenGLVertexBufferResource*>(pVB.get());
	lVB->unlock();
}

/************************************************************************/
IndexBufferRHIPtr OpenGLRHI::RHICreateIndexBuffer(uint8_t pStride, uint32_t pSize, EBufferUsage pUsage, const void* pData)
{
	return new OpenGLIndexBufferResource(pStride, pSize, pUsage, pData);
}

/************************************************************************/
void* OpenGLRHI::RHILockIndexBuffer(IndexBufferRHIPtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess)
{
	OpenGLIndexBufferResource* lVB = static_cast<OpenGLIndexBufferResource*>(pVB.get());
	void* lData = lVB->lock(pOffset, pSize, pAccess);
	return lData;
}

/************************************************************************/
void OpenGLRHI::RHIUnlockIndexBuffer(IndexBufferRHIPtr& pVB)
{
	OpenGLIndexBufferResource* lVB = static_cast<OpenGLIndexBufferResource*>(pVB.get());
	lVB->unlock();
}

/************************************************************************/
VertexShaderRHIPtr OpenGLRHI::RHICreateVertexShader(const std::string& pSource)
{
	OpenGLVertexShaderResource* resource = new OpenGLVertexShaderResource(pSource);
	bool lSuccess = resource->compileShader();
	return resource;
}

/************************************************************************/
FragmentShaderRHIPtr OpenGLRHI::RHICreateFragmentShader(const std::string& pSource)
{
	OpenGLFragmentShaderResource* resource = new OpenGLFragmentShaderResource(pSource);
	bool lSuccess = resource->compileShader();
	return resource;
}


/************************************************************************/
UniformBufferRHI* OpenGLRHI::RHICreateUniformBuffer()
{
	return nullptr;
}

/************************************************************************/
void OpenGLRHI::RHIDebug(ProgramRHIPtr& pProgram)
{

// https://www.khronos.org/opengl/wiki/Example_Code
//void RHIGetUniforms(ProgramRHIPtr& pProgram)

	OpenGLProgramResource* program = static_cast<OpenGLProgramResource*>(pProgram.get());

	GLint numUniforms = 0;
	glGetProgramInterfaceiv(program->handle(), GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	const GLenum properties[5] = { GL_BLOCK_INDEX, GL_TYPE, GL_OFFSET, GL_NAME_LENGTH, GL_LOCATION };

	for (int i = 0; i < numUniforms; ++i)
	{
		GLint values[5];
		glGetProgramResourceiv(program->handle(), GL_UNIFORM, i, 5, properties, 5, NULL, values);

		//Skip any uniforms that are in a block.
		if (values[0] != -1)
		{
			// UBO
		}
		else
		{
			// Uniform

			//Get the name. Must use a std::vector rather than a std::string for C++03 standards issues.
			//C++11 would let you use a std::string directly.
			std::vector<char> nameData(values[3]);
			glGetProgramResourceName(program->handle(), GL_UNIFORM, i, nameData.size(), NULL, &nameData[0]);
			std::string name(nameData.begin(), nameData.end() - 1);
		}
	}
}
 
/************************************************************************/
ProgramRHIPtr OpenGLRHI::RHICreateProgram(VertexShaderRHIPtr & pVertex, FragmentShaderRHIPtr & pFragment)
{
	OpenGLProgramResource* resource = new OpenGLProgramResource(static_cast<OpenGLVertexShaderResource*>(pVertex.get()), static_cast<OpenGLFragmentShaderResource*>(pFragment.get()));
	bool lSuccess = resource->linkShader();

	// DEBUG TO REMOVE
	ProgramRHIPtr prog = resource;
	
	RHIDebug(prog);

	return resource;
}

/************************************************************************/
/*void OpenGLRHI::RHIDrawPrimitive(EPrimitiveType pType, uint32_t pStartIndex, uint32_t pCount, uint32_t pNumInstances)
{
	// Bind Program
	// Set Uniform
	// Bind VAO

//	glDrawElements(TranslatePrimitiveType(pType), 6, GL_UNSIGNED_SHORT, NULL);
	TODO;
}*/

/************************************************************************/
void OpenGLRHI::RHIDrawIndexedPrimitive(EPrimitiveType pType, uint32_t pStartIndex, uint32_t pCount, uint32_t pNumInstances)
{
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
	glDrawElements(TranslatePrimitiveType(pType), TranslatePrimitiveTypeSize(pType,pCount), GL_UNSIGNED_SHORT, BUFFER_OFFSET(pStartIndex*sizeof(uint16_t)));
}

/************************************************************************/
void OpenGLRHI::RHISetProgram(ProgramRHIPtr& pProgram)
{
	OpenGLProgramResource* resource = static_cast<OpenGLProgramResource*>(pProgram.get());
	glUseProgram(resource->handle());
}

/************************************************************************/
VertexInputLayoutRHIPtr OpenGLRHI::RHICreateVertexInputLayout(const VertexInputLayout& pLayout)
{
	return new OpenGLVertexInputLayoutResource(pLayout);
}

/************************************************************************/
void OpenGLRHI::RHISetVertexInputLayout(VertexInputLayoutRHIPtr& pLayout)
{
	OpenGLVertexInputLayoutResource* resource = static_cast<OpenGLVertexInputLayoutResource*>(pLayout.get());
	resource->set();
}

