#pragma once
#include "CoreConfig.hpp"
#include "Common.hpp"
#include "RHIResource.hpp"
#include <glm/fwd.hpp>


class Core_EXPORT IDynamicRHI
{
public:
	/** */
	static IDynamicRHI* DynamicLoadRenderer(const char* pName);

	/** Set the renderer */
	static void SetRenderer(IDynamicRHI* pRenderer);
	FORCEINLINE static IDynamicRHI* Get() { return msRHI; }

	virtual void RHIClear(bool pColor, const glm::vec4& pClearColor, bool pDepth, float pDepthValue, bool pStencil, uint32_t pStencilValue) = 0;
	virtual void RHISetScissor(bool pEnable, const glm::i32vec4& pRect) = 0;
	virtual void RHISetViewport(const glm::i32vec4& pViewport, const glm::vec2& pDepthRange) = 0;
	virtual VertexBufferResourcePtr RHICreateVertexBuffer(uint32_t pSize, EBufferUsage pUsage, const void* pData) = 0;
	virtual void* RHILockVertexBuffer(VertexBufferResourcePtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess) = 0;
	virtual void RHIUnlockVertexBuffer(VertexBufferResourcePtr& pVB) = 0;
	virtual IndexBufferResourcePtr RHICreateIndexBuffer(uint8_t pStride, uint32_t pSize, EBufferUsage pUsage, const void* pData) = 0;
	virtual void* RHILockIndexBuffer(IndexBufferResourcePtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess) = 0;
	virtual void RHIUnlockIndexBuffer(IndexBufferResourcePtr& pVB) = 0;

private:
	static IDynamicRHI* msRHI;

};


FORCEINLINE static void RHIClear(bool pColor, const glm::vec4& pClearColor, bool pDepth, float pDepthValue, bool pStencil, uint32_t pStencilValue)
{
	IDynamicRHI::Get()->RHIClear(pColor, pClearColor, pDepth, pDepthValue, pStencil, pStencilValue);
}

FORCEINLINE static void RHISetScissor(bool pEnable, const glm::i32vec4& pRect)
{
	IDynamicRHI::Get()->RHISetScissor(pEnable, pRect);
}

FORCEINLINE static void RHISetViewport(const glm::i32vec4& pViewport, const glm::vec2& pDepthRange)
{
	IDynamicRHI::Get()->RHISetViewport(pViewport, pDepthRange);
}

FORCEINLINE static VertexBufferResourcePtr RHICreateVertexBuffer(uint32_t pSize, EBufferUsage pUsage, const void* pData)
{
	return IDynamicRHI::Get()->RHICreateVertexBuffer(pSize, pUsage, pData);
}

FORCEINLINE static void* RHILockVertexBuffer(VertexBufferResourcePtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess)
{
	return IDynamicRHI::Get()->RHILockVertexBuffer(pVB, pOffset, pSize, pAccess);
}

FORCEINLINE static void RHIUnlockVertexBuffer(VertexBufferResourcePtr& pVB)
{
	IDynamicRHI::Get()->RHIUnlockVertexBuffer(pVB);
}
FORCEINLINE static IndexBufferResourcePtr RHICreateIndexBuffer(uint8_t pStride, uint32_t pSize, EBufferUsage pUsage, const void* pData)
{
	return IDynamicRHI::Get()->RHICreateIndexBuffer(pStride, pSize, pUsage, pData);
}

FORCEINLINE static void* RHILockIndexBuffer(IndexBufferResourcePtr& pVB, uint32_t pOffset, uint32_t pSize, EBufferAccess pAccess)
{
	return IDynamicRHI::Get()->RHILockIndexBuffer(pVB, pOffset, pSize, pAccess);
}