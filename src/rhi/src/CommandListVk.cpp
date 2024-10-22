#include "CommandListVk.h"

#include "RenderDeviceVk.h"
#include "TextureVk.h"

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
			barrier.stateAfter = state;
			m_TextureBarriers.push_back(barrier);
		}
	}
}