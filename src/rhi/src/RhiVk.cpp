#include "rhi/Rhi.h"

#include "RenderDeviceVk.h"

namespace rhi
{
	IRenderDevice* createRenderDevice(const RenderDeviceDesc& desc)
	{
		return RenderDeviceVk::create(desc);
	}


}