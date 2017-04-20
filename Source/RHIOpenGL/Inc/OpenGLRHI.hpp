#pragma once
#include <RHIOpenGLConfig.hpp>

#include <Common.hpp>
#include <RHI.hpp>

#define CHECKGLERROR OpenGLRHI::CheckError(__FILE__,__LINE__)

extern "C" RHIOpenGL_EXPORT IDynamicRHI* CreateDynamicRHI();

class OpenGLRHI : public IDynamicRHI
{
public:
	/** Use KHR_debug extension */
	static void CheckError(const char* pFile, int32_t pLine);

public:
	virtual void RHIClear(bool pColor, const glm::vec4& pClearColor, bool pDepth, float pDepthValue, bool pStencil, uint32_t pStencilValue) override final;
	virtual void RHISetScissor(bool pEnable, const glm::i32vec4& pRect) override final;
	virtual void RHISetViewport(const glm::i32vec4& pViewport, const glm::vec2& pDepthRange) override final;

	virtual VertexBufferRHIPtr RHICreateVertexBuffer(uint32_t pSize, EBufferUsage pUsage, const void* pData) override final;
	virtual void* RHILockVertexBuffer(VertexBufferRHIPtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess) override final;
	virtual void RHIUnlockVertexBuffer(VertexBufferRHIPtr& pVB) override final;
	virtual IndexBufferRHIPtr RHICreateIndexBuffer(uint8_t pStride, uint32_t pSize, EBufferUsage pUsage, const void* pData) override final;
	virtual void* RHILockIndexBuffer(IndexBufferRHIPtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess) override final;
	virtual void RHIUnlockIndexBuffer(IndexBufferRHIPtr& pIB) override final;

	virtual VertexShaderRHIPtr RHICreateVertexShader(const std::string& pSource) override final;
	virtual FragmentShaderRHIPtr RHICreateFragmentShader(const std::string& pSource) override final;
	virtual ProgramRHIPtr RHICreateProgram(VertexShaderRHIPtr& pVertex, FragmentShaderRHIPtr& pFragment) override final;
	virtual void RHISetProgram(ProgramRHIPtr& pProgram) override final;

	virtual void RHIDrawIndexedPrimitive(EPrimitiveType pType, uint32_t pStartIndex, uint32_t pCount, uint32_t pNumInstances = 1) override final;


	virtual VertexInputLayoutRHIPtr RHICreateVertexInputLayout(const VertexInputLayout& pLayout)  override final;
	virtual void RHISetVertexInputLayout(VertexInputLayoutRHIPtr& pLayout)  override final;
};