#include "Engine.h"
#include "rendersystem/include/GraphicsBuffer.hpp"

namespace Air
{
	GraphicsBuffer::GraphicsBuffer(BufferUsage usage, uint32_t access_hint, uint32_t size_in_byte)
		:mUsage(usage), mAccessHint(access_hint), mSizeInByte(size_in_byte)
	{

	}
	GraphicsBuffer::~GraphicsBuffer()
	{

	}
}