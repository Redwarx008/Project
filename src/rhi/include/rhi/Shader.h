#pragma once

#include <cstdint>
#include "Resource.h"
namespace rhi
{
	enum class ShaderResourceType : uint8_t
	{
		Unknown,
		SampledTexture, //SRV
		StorageTexture, //URV
		UniformBuffer, // CBV
		StorageBuffer, // UAV
		Sampler,
		SamplerTexture // Combined texture and sampler
	};
	enum class ShaderType : uint32_t
	{
		Unknown,
		Vertex,
		TessellationControl,
		TessellationEvaluation,
		Geometry,
		Fragment,
		Task,
		Mesh,
		Compute
	};

	inline ShaderType operator | (ShaderType a, ShaderType b)
	{
		return ShaderType(uint32_t(a) | uint32_t(b));
	}
	inline ShaderType operator & (ShaderType a, ShaderType b)
	{
		return ShaderType(uint32_t(a) & uint32_t(b));
	}
	inline ShaderType operator ~ (ShaderType a)
	{
		return ShaderType(~uint32_t(a));
	}
	inline bool operator !(ShaderType a)
	{
		return uint32_t(a) == 0;
	}
	inline bool operator ==(ShaderType a, uint32_t b)
	{
		return uint32_t(a) == b;
	}
	inline bool operator !=(ShaderType a, uint32_t b)
	{
		return uint32_t(a) != b;
	}

	struct ShaderDesc
	{
		ShaderType type = ShaderType::Unknown;
		const char* entry = nullptr;
	};

	class IShader
	{
	public:
		~IShader() = default;
		virtual ShaderDesc getDesc() = 0;
	};
}