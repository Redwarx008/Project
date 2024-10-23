#pragma once

#include "rhi/Buffer.h"
#include "ResourceVk.h"
#include <vk_mem_alloc.h>

namespace rhi
{
	class ContextVk;
	class BufferVk : public IBuffer, public MemoryResource
	{
	public:
		explicit BufferVk(const ContextVk& context, const VmaAllocator& allocator)
			:m_Context(context),
			m_Allocator(allocator)
		{}
		void setState(ResourceState state) { m_State = state; }
		~BufferVk();
		VkBuffer buffer;
	private:
		const ContextVk& m_Context;
		const VmaAllocator& m_Allocator;
	};
}