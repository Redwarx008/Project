#pragma once

#include "rhi/CommandList.h"

#include <vulkan/vulkan.h>
#include <vector>
namespace rhi
{
	class RenderDeviceVk;
	class TextureVk;
	class BufferVk;
	class ContextVk;

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
		void commitBarriers() override;
		void transitionTextureState(ITexture& texture, ResourceState newState) override;
		void transitionBufferState(IBuffer& buffer, ResourceState newState) override;

		void setBufferBarrier(BufferVk& buffer, VkPipelineStageFlags2 stage, VkAccessFlags2 access);
	private:
		CommandListVk() = delete;

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
		std::vector<TextureBarrier> m_TextureBarriers;
		std::vector<BufferBarrier> m_BufferBarriers;

		CommandBuffer* m_CurrentCmdBuf;

		RenderDeviceVk& m_RenderDevice;
	};
}