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

	/** Current bound shader compile validity and report log info */
	static bool CheckShaderCompilStatus(uint32_t pShader);

	/** Current bound program link validity and report log info */
	static bool CheckProgramLinkStatus(uint32_t pShader);

public:
	virtual void RHIClear(bool pColor, const glm::vec4& pClearColor, bool pDepth, float pDepthValue, bool pStencil, uint32_t pStencilValue) override final;
	virtual void RHISetScissor(bool pEnable, const glm::i32vec4& pRect) override final;
	virtual void RHISetViewport(const glm::i32vec4& pViewport, const glm::vec2& pDepthRange) override final;
	virtual VertexBufferResourcePtr RHICreateVertexBuffer(uint32_t pSize, EBufferUsage pUsage, const void* pData) override final;
	virtual void* RHILockVertexBuffer(VertexBufferResourcePtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess) override final;
	virtual void RHIUnlockVertexBuffer(VertexBufferResourcePtr& pVB) override final;
	virtual IndexBufferResourcePtr RHICreateIndexBuffer(uint8_t pStride, uint32_t pSize, EBufferUsage pUsage, const void* pData) override final;
	virtual void* RHILockIndexBuffer(IndexBufferResourcePtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess) override final;
	virtual void RHIUnlockIndexBuffer(IndexBufferResourcePtr& pVB) override final;

	virtual VertexShaderResourcePtr RHICreateVertexShader(const char* pSource) override final;
	virtual FragmentShaderResourcePtr RHICreateFragmentShader(const char* pSource) override final;
	virtual ProgramResourcePtr RHICreateProgram(VertexShaderResourcePtr& pVertex, FragmentShaderResourcePtr& pFragment) override final;
	virtual void RHIDrawPrimitive(EPrimitiveType pType, uint32_t pStartIndex, uint32_t pCount, uint32_t pNumInstances = 0) override final;
	virtual void RHIDrawIndexedPrimitive(EPrimitiveType pType, IndexBufferResourcePtr& pIndexBuffer, uint32_t pNumInstances = 0) override final;
};