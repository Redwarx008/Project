#include "ResourceVk.h"

#include "TextureVk.h"

namespace rhi
{
	ResourceState ResourceStateTracker::getOrCreateResourceState(TextureVk* texture, bool allowCreate)
	{
		auto iter = m_TextureStates.find(texture);
		if (iter != m_TextureStates.end())
		{
			return iter->second;
		}

		if (!allowCreate)
		{
			return ResourceState::Undefined;
		}

		ResourceState state = ResourceState::Common;
		m_TextureStates.insert({ texture, state });
		return state;
	}

	ResourceState ResourceStateTracker::getOrCreateResourceState(BufferVk* buffer, bool allowCreate)
	{

	}
}