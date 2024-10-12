#pragma once


namespace rhi
{
	class IRenderDevice;

	struct SwapChainCreateInfo
	{
		IRenderDevice* renderDevice;

	};

	class ISwapChain
	{
		virtual ~ISwapChain() = default;
	};
}