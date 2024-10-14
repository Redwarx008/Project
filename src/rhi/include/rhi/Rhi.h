#pragma once

#include <cstdint>

#include "RenderDevice.h"
#include "SwapChain.h"

namespace rhi
{
    IRenderDevice* createRenderDevice(const RenderDeviceDesc& desc);

    ISwapChain* createSwapChain(uint32_t* width, uint32_t* height, Format swapChainFormat, bool vsync);
}