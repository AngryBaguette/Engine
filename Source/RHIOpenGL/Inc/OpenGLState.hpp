#pragma once
#include <Common.hpp>
#include <OpenGLRHIResource.hpp>
#include <glm/glm.hpp>

struct ViewportState
{
	int32_t		mX;            // Bottom
	int32_t		mY;            // Left
	uint32_t    mWidth;
	uint32_t    mHeight;
};

struct ScissorState
{
	bool        mEnabled;
	int32_t		mX;            // Bottom
	int32_t		mY;            // Left
	uint32_t    mWidth;
	uint32_t    mHeight;
};

struct DepthStencilState
{
	glm::vec2 mDepthRange;
};


struct OpenGLState
{
	glm::vec4		mClearColor;
	float			mDepthClearValue;
	uint32_t		mStencilClearValue;

	ScissorState	mScissorState;
};


struct Stream
{
	OpenGLVertexBufferResource* mVertexBuffer;
	uint32_t mOffset;
	uint8_t mStride;
};

struct OpenGLState
{
	Stream mStream[16];

};
