#pragma once

namespace rhi
{
	class IBuffer;
	class ITexture;
	class ICommandList;
	struct TextureDesc;
	struct BufferDesc;

	class IRenderDevice
	{
	public:
		virtual ~IRenderDevice() = default;
		virtual void waitIdle() = 0;
		virtual ITexture* createTexture(const TextureDesc& desc) = 0;
		virtual IBuffer* createBuffer(const BufferDesc& desc) = 0;
		virtual IBuffer* createBuffer(const BufferDesc& desc, const void* data, size_t dataSize) = 0;
		virtual ICommandList* createCommandList() = 0;
		virtual uint32_t executeCommandList(ICommandList* const* cmdLists, size_t numCmdLists) = 0;
		virtual void waitForExecution(uint32_t executID, uint64_t timeout) = 0;
	};
}