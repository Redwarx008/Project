#pragma once

namespace rhi
{
	class ICommandList
	{
	public:
		virtual ~ICommandList() = default;

		virtual void open() = 0;
		virtual void close() = 0;

	};
}