#pragma once
#include <Common.hpp>
#include <RHIResource.hpp>
#include <Array.hpp>
#include <GL/glew.h>
#include <string>

std::string LoadShaderFile(const std::string& pFile);
bool CheckShaderCompilStatus(uint32_t pShader);
bool CheckProgramLinkStatus(uint32_t pShader);

template <typename BaseType, GLenum ShaderType>
class OpenGLShaderResource : public BaseType
{
public:
	/** Default constructor */
	OpenGLShaderResource(const std::string& pSource)
	: BaseType()
	, mHandle(-1)
	{
		mSource = pSource;
	}

	/** On success obtain an handle */
	bool compileShader()
	{
		const char* const source[] = { mSource.c_str() };
		mHandle = glCreateShader(ShaderType); CHECKGLERROR;
		glShaderSource(mHandle, 1, source, NULL); CHECKGLERROR;
		glCompileShader(mHandle); CHECKGLERROR;
		bool lSuccess = CheckShaderCompilStatus(mHandle);
		assert(lSuccess);

		if (NOT lSuccess)
		{
			glDeleteShader(mHandle);
			mHandle = -1;
		}

		return lSuccess;
	}

	/** Get the OpenGL handle */
	FORCEINLINE GLuint getHandle() const { return mHandle; }

protected:
	std::string mSource;
	GLuint mHandle;
};

typedef OpenGLShaderResource<VertexShaderResource, GL_VERTEX_SHADER> OpenGLVertexShaderResource;
typedef RefPointer<OpenGLVertexShaderResource> OpenGLVertexShaderResourcePtr;
typedef OpenGLShaderResource<FragmentShaderResource, GL_FRAGMENT_SHADER> OpenGLFragmentShaderResource;
typedef RefPointer<OpenGLFragmentShaderResource> OpenGLFragmentShaderResourcePtr;


class OpenGLProgramResource : public ProgramResource
{
public:
	OpenGLProgramResource(OpenGLVertexShaderResource* pVertexShader, OpenGLFragmentShaderResource* pFragmentShader);

	/** On success obtain an handle */
	bool linkShader();

	GLuint handle() const { return mHandle;  }
protected:
	OpenGLVertexShaderResourcePtr mVertexShader;
	OpenGLFragmentShaderResourcePtr mFragmentShader;
	GLuint mHandle;
};