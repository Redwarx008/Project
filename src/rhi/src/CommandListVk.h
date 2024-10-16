#pragma once

#include "rhi/CommandList.h"

namespace rhi
{
	class CommandBuffer
	{
	public:

	};

	class CommandListVk final : public ICommandList
	{
	public:
		~CommandListVk();
		void open() override;
		void close() override;
	private:
		CommandListVk() {}


	};
}