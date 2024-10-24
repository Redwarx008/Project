#pragma once
#include <cstdint>

#include "Texture.h"
#include "Shader.h"
namespace rhi
{
	enum class PrimitiveType : uint8_t
	{
		PointList,
		LineList,
		TriangleList,
		TriangleStrip,
	};

	struct VertexAttributeDesc
	{
		uint32_t bufferIndex;
		uint32_t location;
		Format format;
		bool isInstanced = false;
	};

	struct VertexInputLayoutDesc
	{
		const VertexAttributeDesc* vertexAttributeArray = nullptr;
		uint32_t numVertexAttribute = 0;
	};

	struct ShaderBindingDesc
	{
		ShaderType shaderStages = ShaderType::Unknown;
		uint32_t bindingSlot = 0;
	};

	struct ShaderBindingLayoutDesc
	{

	};

	struct GraphicsPipelineDesc
	{
		PrimitiveType primType = PrimitiveType::TriangleList;

		VertexInputLayoutDesc vertexInputLayout;


		IShader* vertexShader = nullptr;
		IShader* fragmentShader = nullptr;
		IShader* tessControlShader = nullptr;
		IShader* tessEvaluationShader = nullptr;
		IShader* taskShader = nullptr;
		IShader* meshShader = nullptr;
		IShader* computeShader = nullptr;
	};
	class IGraphicsPipeline
	{

	};
}