#pragma once
#include <cstdint>
#include <vector>
#include "Texture.h"
#include "Shader.h"
#include "common/Utils.h"
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

	struct VertexInputDesc
	{
		const VertexAttributeDesc* vertexAttributes = nullptr;
		uint32_t numVertexAttribute = 0;
	};

	struct ShaderBindingDesc
	{
		ShaderType shaderStages = ShaderType::Unknown;
		ShaderResourceType type = ShaderResourceType::Unknown;
		uint32_t bindingSlot = 0;

#define SHADER_BINDING_DESC_INITIALIZER(TYPE) \
		static ShaderBindingDesc TYPE(ShaderType stage, const uint32_t bindingSlot) \
		{														\
				ShaderBindingDesc res{};						\
				res.shaderStages = stage;						\
				res.type = ShaderResourceType::TYPE;			\
				res.bindingSlot = bindingSlot;					\
				return res;										\
		}														


		SHADER_BINDING_DESC_INITIALIZER(SampledTexture)
		SHADER_BINDING_DESC_INITIALIZER(StorageTexture)
		SHADER_BINDING_DESC_INITIALIZER(UniformBuffer)
		SHADER_BINDING_DESC_INITIALIZER(StorageBuffer)
		SHADER_BINDING_DESC_INITIALIZER(Sampler)
		SHADER_BINDING_DESC_INITIALIZER(SamplerTexture)

#undef SHADER_BINDING_DESC_INITIALIZER
	};


	class IShaderBindingLayout
	{
	public:
		virtual ~IShaderBindingLayout() = default;
	};

	class IShaderBindingSet
	{
	public:
		virtual ~IShaderBindingSet() = default;
	};

	enum class BlendFactor : uint8_t
	{
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DstAlpha,
		OneMinusDstAlpha,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,
		SrcAlphaSaturate,
		Src1Color,
		OneMinusSrc1Color,
		Src1Alpha,
		OneMinusSrc1Alpha
	};

	enum class BlendOp : uint8_t
	{
		Add = 1,
		Subrtact = 2,
		ReverseSubtract = 3,
		Min = 4,
		Max = 5
	};

	enum class ColorMask : uint8_t
	{
		// These values are equal to their counterparts in DX11, DX12, and Vulkan.
		Red = 1,
		Green = 2,
		Blue = 4,
		Alpha = 8,
		All = 0xF
	};

	ENUM_CLASS_FLAG_OPERATORS(ColorMask)

	struct BlendStateDesc
	{
		bool alphaToCoverageEnable = false;

		struct RenderTargetBlendState
		{
			bool        blendEnable = false;
			BlendFactor srcBlend = BlendFactor::One;
			BlendFactor destBlend = BlendFactor::Zero;
			BlendOp     blendOp = BlendOp::Add;
			BlendFactor srcBlendAlpha = BlendFactor::One;
			BlendFactor destBlendAlpha = BlendFactor::Zero;
			BlendOp     blendOpAlpha = BlendOp::Add;
			ColorMask   colorWriteMask = ColorMask::All;
		};

		static constexpr uint32_t MaxRenderTargets = 8;
		RenderTargetBlendState renderTargetBlendStates[MaxRenderTargets];
	};

	enum class CompareOp : uint8_t
	{
		Never = 1,
		Less = 2,
		Equal = 3,
		LessOrEqual = 4,
		Greater = 5,
		NotEqual = 6,
		GreaterOrEqual = 7,
		Always = 8
	};

	enum class StencilOp : uint8_t
	{
		Keep = 1,
		Zero = 2,
		Replace = 3,
		IncrementAndClamp = 4,
		DecrementAndClamp = 5,
		Invert = 6,
		IncrementAndWrap = 7,
		DecrementAndWrap = 8
	};

	struct StencilOpState
	{
		StencilOp failOp = StencilOp::Keep;
		StencilOp passOp = StencilOp::Keep;
		StencilOp depthFailOp = StencilOp::Keep;
		CompareOp compareOp = CompareOp::Always;
	};

	struct DepthStencilStateDesc
	{
		bool            depthTestEnable = true;
		bool            depthWriteEnable = true;
		CompareOp		depthCompareOp = CompareOp::Less;
		bool            stencilEnable = false;
		uint8_t         stencilReadMask = 0xff;
		uint8_t         stencilWriteMask = 0xff;
		StencilOpState	frontFaceStencil;
		StencilOpState	backFaceStencil;
	};

	enum class PolygonMode : uint8_t
	{
		Fill = 0,
		Line = 1,
		Point = 2
	};

	enum class CullMode : uint8_t
	{
		None,
		Front,
		back
	};

	struct RasterState
	{
		PolygonMode fillMode = PolygonMode::Fill;
		CullMode cullMode = CullMode::back;
		bool depthClampEnable = false;
		bool depthBiasEnable = false;
		bool rasterDiscardEnable = true;
		float lineWidth = 1.0f;
	};

	struct GraphicsPipelineDesc
	{
		PrimitiveType primType = PrimitiveType::TriangleList;

		VertexInputDesc vertexInputDesc;
		std::vector<IShaderBindingLayout*> shaderBindingLayouts;

		IShader* vertexShader = nullptr;
		IShader* fragmentShader = nullptr;
		IShader* tessControlShader = nullptr;
		IShader* tessEvaluationShader = nullptr;
		IShader* taskShader = nullptr;
		IShader* meshShader = nullptr;
		IShader* computeShader = nullptr;

		BlendStateDesc blendDesc;
		RasterState rasterState;
	};

	class IGraphicsPipeline
	{
	public:
		virtual ~IGraphicsPipeline() = default;
		virtual const GraphicsPipelineDesc& getDesc() = 0;
	};
}