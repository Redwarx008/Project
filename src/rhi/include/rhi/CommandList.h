#pragma once

namespace rhi
{
	enum class CommandListType

	class ICommandList
	{
	public:
		virtual ~ICommandList() = default;

		virtual void open() = 0;
		virtual void close() = 0;

	};
}