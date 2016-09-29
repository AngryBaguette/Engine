#include "OpenGLShader.hpp"
#include "OpenGLRHI.hpp"
#include <Array.hpp>

#include <fstream>


/*****************************************************************************/
std::string LoadShaderFile(const std::string& pFilename)
{
	std::string source;
	std::ifstream is(pFilename, std::ifstream::in);
	if (is)
	{
		std::string line = "";
		while (std::getline(is, line))
			source += line + "\n";
		is.close();
	}
	return source;
}

/************************************************************************/
bool CheckShaderCompilStatus(uint32_t pShader)
{
	// Check Fragment Shader
	GLint result;
	GLint messageLength;
	glGetShaderiv(pShader, GL_COMPILE_STATUS, &result); CHECKGLERROR;
	glGetShaderiv(pShader, GL_INFO_LOG_LENGTH, &messageLength); CHECKGLERROR;
	if (messageLength > 0) {
		Array<char> message(messageLength + 1);
		glGetShaderInfoLog(pShader, messageLength, nullptr, &message[0]); CHECKGLERROR;
		printf("%s\n", &message[0]);
	}

	return result EQ GL_TRUE;
}

/************************************************************************/
bool CheckProgramLinkStatus(uint32_t pShader)
{
	// Check Fragment Shader
	GLint result;
	GLint messageLength;
	glGetProgramiv(pShader, GL_LINK_STATUS, &result); CHECKGLERROR;
	glGetProgramiv(pShader, GL_INFO_LOG_LENGTH, &messageLength); CHECKGLERROR;
	if (messageLength > 0)
	{
		Array<char> message(messageLength + 1);
		glGetProgramInfoLog(pShader, messageLength, nullptr, &message[0]); CHECKGLERROR;
		printf("%s\n", &message[0]);
	}

	return result EQ GL_TRUE;
}

/************************************************************************/
OpenGLProgramResource::OpenGLProgramResource(OpenGLVertexShaderResource* pVertexShader, OpenGLFragmentShaderResource* pFragmentProgram)
: ProgramResource()
, mHandle(-1)
, mVertexShader(pVertexShader)
, mFragmentShader(pFragmentProgram)
{


}

/************************************************************************/
bool OpenGLProgramResource::linkShader()
{
	// At least vertex and fragment program
	bool lValid = mVertexShader AND mFragmentShader;
	if (lValid)
	{
		mHandle = glCreateProgram();

		glAttachShader(mHandle, mVertexShader->getHandle());
		glAttachShader(mHandle, mFragmentShader->getHandle());
		glLinkProgram(mHandle);
		bool lSuccess = CheckProgramLinkStatus(mHandle); assert(lSuccess);
		if (NOT lSuccess)
		{
			glDeleteProgram(mHandle);
			mHandle = -1;
		}
		return lSuccess;
	}

	return false;
}