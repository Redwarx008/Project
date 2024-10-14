#include <cstdint>


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

	typedef void(*DebugMessageCallback) (MessageSeverity severity, const char* msg);

	extern DebugMessageCallback g_DebugMessageCallback;

	inline void LogError(MessageSeverity severity, const char* functionName, int line, const char* msg)
	{
		if (g_DebugMessageCallback != nullptr)
		{
			g_DebugMessageCallback(severity)
		}
	}
}