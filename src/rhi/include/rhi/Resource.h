#pragma once

#include <cstdint>

namespace rhi
{
	enum class ResourceState : uint32_t
	{
		Undefined = (0 << 0),
		Common = (1 << 0),
		ConstantBuffer = (1 << 1),
		VertexBuffer = (1 << 2),
		IndexBuffer = (1 << 3),
		IndirectArgument = (1 << 4),
		ShaderResource = (1 << 5),
		UnorderedAccess = (1 << 6),
		RenderTarget = (1 << 7),
		DepthWrite = (1 << 8),
		DepthRead = (1 << 9),
		CopyDest = (1 << 10),
		CopySource = (1 << 11),
		ResolveDest = (1 << 12),
		ResolveSource = (1 << 13),
		Present = (1 << 14),
		AccelStructRead = (1 << 15),
		AccelStructWrite = (1 << 16),
		AccelStructBuildInput = (1 << 17),
		AccelStructBuildBlas = (1 << 18),
		ShadingRateSurface = (1 << 19),
	};

	inline ResourceState operator | (ResourceState a, ResourceState b)
	{
		return ResourceState(uint32_t(a) | uint32_t(b));
	}
	inline ResourceState operator & (ResourceState a, ResourceState b)
	{
		return ResourceState(uint32_t(a) & uint32_t(b));
	}
	inline ResourceState operator ~ (ResourceState a)
	{
		return ResourceState(~uint32_t(a));
	}
	inline bool operator !(ResourceState a)
	{
		return uint32_t(a) == 0;
	}
	inline bool operator ==(ResourceState a, uint32_t b)
	{
		return uint32_t(a) == b;
	}
	inline bool operator !=(ResourceState a, uint32_t b)
	{
		return uint32_t(a) != b;
	}

	class IResource
	{
	public:
		virtual ResourceState getState() const = 0;
	protected:
		ResourceState m_State = ResourceState::Undefined;
	};
}