#pragma once

#include <functional>

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

	struct RenderDeviceDesc
	{
		MessageCallback messageCallBack;
		bool enableValidationLayer;
		// enable instance features
	};

	class ITexture;
	struct TextureDesc;

	class IRenderDevice
	{
	public:
		virtual ~IRenderDevice() = default;

		virtual ITexture* createTexture(const TextureDesc& desc) = 0;
	};
}