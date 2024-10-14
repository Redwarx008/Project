#include "rhi/Rhi.h"

#include "RenderDeviceVk.h"

namespace rhi
{
	DebugMessageCallbackFunc g_DebugMessageCallback;

	IRenderDevice* createRenderDevice(const RenderDeviceDesc& desc)
	{
		return RenderDeviceVk::create(desc);
	}


}