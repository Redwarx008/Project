#pragma once

#include "Resource.h"

namespace rhi
{
	enum class BufferAccess : uint8_t
	{
		// means VK_MEMORY_HEAP_DEVICE_LOCAL_BIT
		GpuOnly,
		// means VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		CpuWrite,
		// Buffers for data written by or transferred from the GPU that you want to read back on the CPU, e.g. results of some computations.
		// means vVK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT 
		CpuRead,
	};


	enum class BufferUsage : uint8_t
	{
		VertexBuffer,
		IndexBuffer,
		IndirectBuffer,
		UniformBuffer,
		StorageBuffer,
		UniformTexelBuffer,
		StorageTexelBuffer
	};

	struct BufferDesc
	{
		size_t size = 0;

		BufferAccess access = BufferAccess::GpuOnly;

		BufferUsage usage = BufferUsage::VertexBuffer;
	};


	class IBuffer : public IResource
	{
	public:
		virtual ~IBuffer() = default;
		const BufferDesc& getDesc() const { return m_Desc; }
	protected:
		BufferDesc m_Desc;
	};
}