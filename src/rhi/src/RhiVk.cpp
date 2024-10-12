#include "rhi/Rhi.h"

#include "RenderDeviceVk.h"

namespace rhi
{
	IRenderDevice* CreateRenderDevice(const RenderDeviceDesc& desc)
	{
		return RenderDeviceVk::Create(desc);
	}


}