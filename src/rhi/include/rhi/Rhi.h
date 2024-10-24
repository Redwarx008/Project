#pragma once

#include <cstdint>
#include <functional>

#include "RenderDevice.h"
#include "SwapChain.h"

namespace rhi
{
	enum class MessageSeverity : uint8_t
	{
		Verbose,
		Info,
		Warning,
		Error,
		Fatal
	};

	//typedef void (_stdcall *DebugMessageCallbackFunc) (MessageSeverity severity, const char* msg);
	using DebugMessageCallbackFunc = std::function<void(MessageSeverity severity, const char* msg)>;

	struct RenderDeviceDesc
	{
		DebugMessageCallbackFunc messageCallback;
		bool enableValidationLayer;
		// enable instance features
	};

    IRenderDevice* createRenderDevice(const RenderDeviceDesc& desc);

    ISwapChain* createSwapChain(uint32_t* width, uint32_t* height, Format swapChainFormat, bool vsync);
}