#pragma once

#include <cstdint>

#include "RenderDevice.h"
#include "SwapChain.h"

namespace rhi
{
    IRenderDevice* CreateRenderDevice(const RenderDeviceDesc& desc);

    ISwapChain* CreateSwapChain(uint32_t* width, uint32_t* height, Format swapChainFormat, bool vsync);
}