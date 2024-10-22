#pragma once

#include "rhi/Resource.h"

namespace rhi
{
	class ITexture;
	class IBuffer;
	class ICommandList
	{
	public:
		virtual ~ICommandList() = default;

		virtual void open() = 0;
		virtual void close() = 0;
		virtual void transitionTextureState(ITexture& texture, ResourceState newState) = 0;
		virtual void transitionBufferState(IBuffer& buffer, ResourceState newState) = 0;
	};
}