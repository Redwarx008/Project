#include "CommandListVk.h"

#include "RenderDeviceVk.h"
#include "TextureVk.h"
#include "BufferVk.h"
namespace rhi
{
	void CommandListVk::open()
	{
		m_CurrentCmdBuf = m_RenderDevice.getOrCreateCommandBuffer();

		VkCommandBufferBeginInfo cmdBufferBeginInfo{};
		cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(m_CurrentCmdBuf->vkCmdBuf, &cmdBufferBeginInfo);
	}

	inline bool resourceStateHasWriteAccess(ResourceState state)
	{
		const ResourceState writeAccessStates =
			ResourceState::RenderTarget |
			ResourceState::UnorderedAccess |
			ResourceState::CopyDest |
			ResourceState::ResolveDest;
		return (state & writeAccessStates) == state;
	}

	void CommandListVk::transitionTextureState(ITexture& texture, ResourceState newState)
	{
		auto& textureVk = static_cast<TextureVk&>(texture);

		ResourceState oldState = textureVk.getState();

		// Always add barrier after writes.
		bool isAfterWrites = resourceStateHasWriteAccess(oldState);
		bool transitionNecessary = oldState != newState || isAfterWrites;

		if (transitionNecessary)
		{
			TextureBarrier barrier;
			barrier.texture = &textureVk;
			barrier.stateBefore = oldState;
			barrier.stateAfter = newState;
			m_TextureBarriers.push_back(barrier);
		}

		textureVk.setState(newState);
	}

	void CommandListVk::transitionBufferState(IBuffer& buffer, ResourceState newState)
	{
		auto& bufferVk = static_cast<BufferVk&>(buffer);

		ResourceState oldState = bufferVk.getState();

		if (bufferVk.getDesc().access != BufferAccess::GpuOnly)
		{
			// host visible buffers can't change state
			return;
		}

		// Always add barrier after writes.
		bool isAfterWrites = resourceStateHasWriteAccess(oldState);
		bool transitionNecessary = oldState != newState || isAfterWrites;

		if (transitionNecessary)
		{
			// See if this buffer is already used for a different purpose in this batch.
			// If it is, combine the state bits.
			// Example: same buffer used as index and vertex buffer, or as SRV and indirect arguments.
			for (BufferBarrier& barrier : m_BufferBarriers)
			{
				if (barrier.buffer == &bufferVk)
				{
					barrier.stateAfter = ResourceState(barrier.stateAfter | newState);
					bufferVk.setState(barrier.stateAfter);
					return;
				}
			}

			BufferBarrier barrier;
			barrier.buffer = &bufferVk;
			barrier.stateBefore = oldState;
			barrier.stateAfter = newState;
			m_BufferBarriers.push_back(barrier);
		}
		bufferVk.setState(newState);
	}

	void CommandListVk::commitBarriers()
	{
		if (m_BufferBarriers.empty() && m_TextureBarriers.empty())
		{
			return;
		}

		std::vector<VkImageMemoryBarrier2> imageBarriers{ m_TextureBarriers.size() };
		std::vector<VkBufferMemoryBarrier2> bufferBarriers{ m_BufferBarriers.size() };

		for (int i = 0; i < m_TextureBarriers.size(); ++i)
		{
			const TextureBarrier& barrier = m_TextureBarriers[i];

			VkImageLayout oldLayout = resourceStateToVkImageLayout(barrier.stateBefore);
			VkImageLayout newLayout = resourceStateToVkImageLayout(barrier.stateAfter);
			assert(newLayout != VK_IMAGE_LAYOUT_UNDEFINED);

			VkPipelineStageFlags2 srcStage = resourceStatesToVkPipelineStageFlags2(barrier.stateBefore);
			VkPipelineStageFlags2 dstStage = resourceStatesToVkPipelineStageFlags2(barrier.stateAfter);

			VkAccessFlags2 srcAccessMask = resourceStatesToVkAccessFlags2(barrier.stateBefore);
			VkAccessFlags2 dstAccessMask = resourceStatesToVkAccessFlags2(barrier.stateAfter);

			VkImageSubresourceRange subresourceRange{};
			subresourceRange.aspectMask = getVkAspectMask(barrier.texture->format);
			subresourceRange.baseArrayLayer = 0;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.layerCount = barrier.texture->getDesc().arraySize;
			subresourceRange.levelCount = barrier.texture->getDesc().mipLevels;

			imageBarriers[i].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
			imageBarriers[i].pNext = nullptr;
			imageBarriers[i].srcStageMask = srcStage;
			imageBarriers[i].srcAccessMask = srcAccessMask;
			imageBarriers[i].dstStageMask = dstStage;
			imageBarriers[i].dstAccessMask = dstAccessMask;
			imageBarriers[i].oldLayout = oldLayout;
			imageBarriers[i].newLayout = newLayout;
			imageBarriers[i].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarriers[i].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarriers[i].image = barrier.texture->image;
			imageBarriers[i].subresourceRange = subresourceRange;
		}

		for (int i = 0; i < m_BufferBarriers.size(); ++i)
		{
			const BufferBarrier& barrier = m_BufferBarriers[i];

			VkPipelineStageFlags2 srcStage = resourceStatesToVkPipelineStageFlags2(barrier.stateBefore);
			VkPipelineStageFlags2 dstStage = resourceStatesToVkPipelineStageFlags2(barrier.stateAfter);

			VkAccessFlags2 srcAccessMask = resourceStatesToVkAccessFlags2(barrier.stateBefore);
			VkAccessFlags2 dstAccessMask = resourceStatesToVkAccessFlags2(barrier.stateAfter);

			bufferBarriers[i].sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
			bufferBarriers[i].pNext = nullptr;
			bufferBarriers[i].srcStageMask = srcStage;
			bufferBarriers[i].srcAccessMask = srcAccessMask;
			bufferBarriers[i].dstStageMask = dstStage;
			bufferBarriers[i].dstAccessMask = dstAccessMask;
			bufferBarriers[i].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			bufferBarriers[i].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			bufferBarriers[i].buffer = barrier.buffer->buffer;
			bufferBarriers[i].size = barrier.buffer->getDesc().size;
			bufferBarriers[i].offset = 0;
		}

		VkDependencyInfo dependencyInfo{};
		dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
		dependencyInfo.pNext = nullptr;
		dependencyInfo.dependencyFlags = 0;
		dependencyInfo.imageMemoryBarrierCount = imageBarriers.size();
		dependencyInfo.pImageMemoryBarriers = imageBarriers.data();
		dependencyInfo.bufferMemoryBarrierCount = bufferBarriers.size();
		dependencyInfo.pBufferMemoryBarriers = bufferBarriers.data();

		vkCmdPipelineBarrier2(m_CurrentCmdBuf->vkCmdBuf, &dependencyInfo);

		m_BufferBarriers.clear();
		m_TextureBarriers.clear();
	}
}