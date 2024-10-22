#pragma once

#include "rhi/CommandList.h"

#include <vulkan/vulkan.h>

namespace rhi
{
	class RenderDeviceVk;

	class CommandBuffer
	{
	public:
		explicit CommandBuffer(const ContextVk& context)
			:m_Context(context)
		{}

		VkCommandBuffer vkCmdBuf{ VK_NULL_HANDLE };
		VkCommandPool vkCmdPool{ VK_NULL_HANDLE };
	private:
		const ContextVk& m_Context;
	};

	class CommandListVk final : public ICommandList
	{
	public:
		~CommandListVk();
		explicit CommandListVk(RenderDeviceVk& renderDevice)
			:m_RenderDevice(renderDevice)
		{}
		void open() override;
		void close() override;
		void transitionTextureState(ITexture& texture, ResourceState newState) override;
		void transitionBufferState(IBuffer& buffer, ResourceState newState) override;
	private:
		CommandListVk() = delete;

		std::vector<TextureBarrier> m_TextureBarriers;
		std::vector<BufferBarrier> m_BufferBarriers;

		CommandBuffer* m_CurrentCmdBuf;

		RenderDeviceVk& m_RenderDevice;
	};
}