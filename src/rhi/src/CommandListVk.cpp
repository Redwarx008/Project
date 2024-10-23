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

		std::vector<VkImageMemoryBarrier2> imageBarriers;
		std::vector<VkBufferMemoryBarrier2> bufferBarriers;


	}
}