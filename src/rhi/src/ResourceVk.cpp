#include "ResourceVk.h"
#include "rhi/common/Error.h"
#include "TextureVk.h"

namespace rhi
{
	static VkAccessFlags2 resourceStateToVkAccessFlags2(ResourceState state)
	{
		switch (state)
		{
		case ResourceState::Undefined: return 0;
		case ResourceState::Common: return 0;
		case ResourceState::VertexBuffer: return VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT;
		case ResourceState::IndexBuffer: return VK_ACCESS_2_INDEX_READ_BIT;
		case ResourceState::ConstantBuffer: return VK_ACCESS_2_UNIFORM_READ_BIT;
		case ResourceState::ShaderResource: return VK_ACCESS_2_SHADER_READ_BIT;
		case ResourceState::UnorderedAccess: return VK_ACCESS_2_SHADER_READ_BIT | VK_ACCESS_2_SHADER_WRITE_BIT;
		case ResourceState::RenderTarget: return VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
		case ResourceState::IndirectArgument: return VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT;
		case ResourceState::Present: return 0;
		case ResourceState::DepthRead: return VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		case ResourceState::DepthWrite: return VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		case ResourceState::CopySource: return VK_ACCESS_2_TRANSFER_READ_BIT;
		case ResourceState::CopyDest: return VK_ACCESS_2_TRANSFER_WRITE_BIT;
		case ResourceState::ResolveSource: return VK_ACCESS_2_TRANSFER_READ_BIT;
		case ResourceState::ResolveDest: return VK_ACCESS_2_TRANSFER_WRITE_BIT;
		default:
			LOG_ERROR("Unexpected resource state");
			return 0;
		}
	}

	static VkPipelineStageFlags2 resourceStateToVkPipelineStageFlags2(ResourceState state)
	{

	}

	VkAccessFlags2 resourceStatesToVkAccessFlags2(ResourceState states)
	{
		const uint32_t numStateBits = 32;
		uint32_t stateTmp = uint32_t(states);
		uint32_t bitIndex = 0;

		VkAccessFlags2 accessflags = 0;
		while (stateTmp != 0 && bitIndex < numStateBits)
		{
			uint32_t bit = (1 << bitIndex);
			if (bit & stateTmp)
			{
				accessflags |= resourceStateToVkAccessFlags2(ResourceState(bit));
				stateTmp &= ~bit;
			}
			bitIndex++;
		}
		return accessflags;
	}

	VkPipelineStageFlags2 resourceStatesToVkPipelineStageFlags2(ResourceState states)
	{

	}
}