#pragma once

namespace rhi
{

	class ITexture;
	class ICommandList;
	struct TextureDesc;

	class IRenderDevice
	{
	public:
		virtual ~IRenderDevice() = default;
		virtual void waitIdle() = 0;
		virtual ITexture* createTexture(const TextureDesc& desc) = 0;
		virtual ICommandList* createCommandList() = 0;
	};
}