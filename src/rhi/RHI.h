#pragma once

#include <vector>
#include <functional>

#include <vk_mem_alloc.h>

#define ASSERT_VK_SUCCESS(res) assert((res) == VK_SUCCESS)

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

    using MessageCallback = std::function<void(MessageSeverity, const char*)>;

	class MemoryResource
	{
	public:
		bool managed = true;
		VmaAllocation allocation;
	};

    enum class ResourceStates : uint32_t
    {
        Unknown = 0,
        Common = 0x00000001,
        ConstantBuffer = 0x00000002,
        VertexBuffer = 0x00000004,
        IndexBuffer = 0x00000008,
        IndirectArgument = 0x00000010,
        ShaderResource = 0x00000020,
        UnorderedAccess = 0x00000040,
        RenderTarget = 0x00000080,
        DepthWrite = 0x00000100,
        DepthRead = 0x00000200,
        StreamOut = 0x00000400,
        CopyDest = 0x00000800,
        CopySource = 0x00001000,
        ResolveDest = 0x00002000,
        ResolveSource = 0x00004000,
        Present = 0x00008000,
        AccelStructRead = 0x00010000,
        AccelStructWrite = 0x00020000,
        AccelStructBuildInput = 0x00040000,
        AccelStructBuildBlas = 0x00080000,
        ShadingRateSurface = 0x00100000,
        OpacityMicromapWrite = 0x00200000,
        OpacityMicromapBuildInput = 0x00400000,
    };

}