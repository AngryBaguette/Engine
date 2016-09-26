#include "OpenGLShader.hpp"
#include "OpenGLRHI.hpp"
#include <Array.hpp>

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