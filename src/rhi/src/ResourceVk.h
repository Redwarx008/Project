#pragma once

#include "rhi/Resource.h"

#include <vk_mem_alloc.h>

#include <unordered_map>
#include <vector>

namespace rhi
{
	class TextureVk;
	class BufferVk;

	struct TextureBarrier
	{
		TextureVk* texture = nullptr;
		ResourceState stateBefore = ResourceState::Undefined;
		ResourceState stateAfter = ResourceState::Undefined;
	};

	struct BufferBarrier
	{
		BufferVk* buffer = nullptr;
		ResourceState stateBefore = ResourceState::Undefined;
		ResourceState stateAfter = ResourceState::Undefined;
	};

	class MemoryResource
	{
	public:
		bool managed = true;
		VmaAllocation allocation = nullptr;
	};

	class ResourceStateTracker
	{
	public:
		void setTextureState(TextureVk* texture, ResourceState state);
		void setBufferState(BufferVk* buffer, ResourceState state);
	private:


		std::unordered_map<TextureVk*, ResourceState> m_TextureStates;
		std::unordered_map<BufferVk*, ResourceState> m_BufferStates;

		std::vector<TextureBarrier> m_TextureBarriers;
		std::vector<BufferBarrier> m_BufferBarriers;
	};
}