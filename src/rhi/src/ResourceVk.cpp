#include "ResourceVk.h"
#include "rhi/common/Error.h"
#include "TextureVk.h"

namespace rhi
{
	static VkAccessFlags2 resourceStateToVkAccessFlags2(ResourceState state)
	{
		// clang-format off
		switch (state)
		{
		case ResourceState::Undefined:			return VK_ACCESS_2_NONE;
		case ResourceState::Common:				return VK_ACCESS_2_NONE;
		case ResourceState::VertexBuffer:		return VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT;
		case ResourceState::IndexBuffer:		return VK_ACCESS_2_INDEX_READ_BIT;
		case ResourceState::ConstantBuffer:	    return VK_ACCESS_2_UNIFORM_READ_BIT;
		case ResourceState::ShaderResource:		return VK_ACCESS_2_SHADER_READ_BIT;
		case ResourceState::UnorderedAccess:	return VK_ACCESS_2_SHADER_READ_BIT | VK_ACCESS_2_SHADER_WRITE_BIT;
		case ResourceState::RenderTarget:		return VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
		case ResourceState::IndirectArgument:   return VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT;
		case ResourceState::Present:			return VK_ACCESS_2_NONE;
		case ResourceState::DepthRead:			return VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		case ResourceState::DepthWrite:			return VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		case ResourceState::CopySource:			return VK_ACCESS_2_TRANSFER_READ_BIT;
		case ResourceState::CopyDest:		    return VK_ACCESS_2_TRANSFER_WRITE_BIT;
		case ResourceState::ResolveSource:      return VK_ACCESS_2_TRANSFER_READ_BIT;
		case ResourceState::ResolveDest:	    return VK_ACCESS_2_TRANSFER_WRITE_BIT;
		default:
			LOG_ERROR("Unexpected resource state");
			return 0;
		}
		// clang-format on
	}

	static VkPipelineStageFlags2 resourceStateToVkPipelineStageFlags2(ResourceState state)
	{
		constexpr VkPipelineStageFlags2 allShaderStages = VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_2_TESSELLATION_CONTROL_SHADER_BIT |
			VK_PIPELINE_STAGE_2_TESSELLATION_EVALUATION_SHADER_BIT | VK_PIPELINE_STAGE_2_GEOMETRY_SHADER_BIT | VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT |
			VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT;
		// clang-format off
		switch (state)
		{
		case ResourceState::Undefined:			return VK_PIPELINE_STAGE_2_NONE;
		case ResourceState::Common:				return VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
		case ResourceState::VertexBuffer:		return VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT;
		case ResourceState::IndexBuffer:		return VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT;
		case ResourceState::ConstantBuffer:	    return allShaderStages;
		case ResourceState::ShaderResource:		return allShaderStages;
		case ResourceState::UnorderedAccess:	return allShaderStages;
		case ResourceState::RenderTarget:		return VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
		case ResourceState::IndirectArgument:   return VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT;
		case ResourceState::Present:			return VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
		case ResourceState::DepthRead:			return VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
		case ResourceState::DepthWrite:			return VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
		case ResourceState::CopySource:			return VK_PIPELINE_STAGE_2_COPY_BIT_KHR;
		case ResourceState::CopyDest:		    return VK_PIPELINE_STAGE_2_COPY_BIT_KHR;
		case ResourceState::ResolveSource:      return VK_PIPELINE_STAGE_2_RESOLVE_BIT;
		case ResourceState::ResolveDest:	    return VK_PIPELINE_STAGE_2_RESOLVE_BIT;
		default:
			LOG_ERROR("Unexpected resource state");
			return 0;
		}
		// clang-format on
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
		const uint32_t numStateBits = 32;
		uint32_t stateTmp = uint32_t(states);
		uint32_t bitIndex = 0;

		VkPipelineStageFlags2 pipelineStages = 0;
		while (stateTmp != 0 && bitIndex < numStateBits)
		{
			uint32_t bit = (1 << bitIndex);
			if (bit & stateTmp)
			{
				pipelineStages |= resourceStateToVkPipelineStageFlags2(ResourceState(bit));
				stateTmp &= ~bit;
			}
			bitIndex++;
		}

		return pipelineStages;
	}

	VkImageLayout resourceStateToVkImageLayout(ResourceState state)
	{
		// clang-format off
		switch (state)
		{
		case ResourceState::Undefined:			return VK_IMAGE_LAYOUT_UNDEFINED;
		case ResourceState::Common:				return VK_IMAGE_LAYOUT_GENERAL;
		case ResourceState::ShaderResource:		return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		case ResourceState::UnorderedAccess:	return VK_IMAGE_LAYOUT_GENERAL;
		case ResourceState::RenderTarget:		return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		case ResourceState::Present:			return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		case ResourceState::DepthRead:			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		case ResourceState::DepthWrite:			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		case ResourceState::CopySource:			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		case ResourceState::CopyDest:		    return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		case ResourceState::ResolveSource:      return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		case ResourceState::ResolveDest:	    return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		default:
			LOG_ERROR("Unexpected resource state");
			return VK_IMAGE_LAYOUT_UNDEFINED;
		}
		// clang-format on
	}
}