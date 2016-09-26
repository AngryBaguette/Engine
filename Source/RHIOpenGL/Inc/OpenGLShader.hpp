#pragma once
#include <Common.hpp>
#include <RHIResource.hpp>
#include <Array.hpp>
#include <GL/glew.h>

template <typename BaseType, GLenum ShaderType>
class OpenGLShaderResource : public BaseType
{
public:
	OpenGLShaderResource() : BaseType() {}
	FORCEINLINE GLuint getHandle() const { return mHandle; }

protected:
	Array<char> mSource;
	GLuint mHandle;
};

typedef OpenGLShaderResource<VertexShaderResource, GL_VERTEX_SHADER> OpenGLVertexShaderResource;