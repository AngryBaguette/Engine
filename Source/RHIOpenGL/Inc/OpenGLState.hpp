#pragma once
#include <Common.hpp>

#include <glm/glm.h>

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

};


struct OpenGLState
{
	glm::vec4		mClearColor;
	float			mDepthClearValue;
	uint32_t		mStencilClearValue;

	ScissorState	mScissorState;
};

