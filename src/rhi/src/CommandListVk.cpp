#include "CommandListVk.h"

#include "RenderDeviceVk.h"

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
}