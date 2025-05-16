#include "ntpch.h"
#include "RenderCommandQueue.h"
#include <Core/Memory.h>
#include <Core/Core.h>

Nut::RenderCommandQueue::RenderCommandQueue()
{
	m_CommandBuffer = nnew uint8_t[10 * 1024 * 1024]; // 10MB
	m_CommandBufferPtr = m_CommandBuffer;
	memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
}

Nut::RenderCommandQueue::~RenderCommandQueue()
{
	ndelete[] m_CommandBuffer;
}

void* Nut::RenderCommandQueue::Allocate(RenderCommandFn func, uint32_t size)
{
	*(RenderCommandFn*)m_CommandBufferPtr = func;
	m_CommandBufferPtr += sizeof(RenderCommandFn);

	*(uint32_t*)m_CommandBufferPtr = size;
	m_CommandBufferPtr += sizeof(uint32_t);

	void* memory = m_CommandBufferPtr;
	m_CommandBufferPtr += size;

	m_CommandCount++;
	return memory;
}

void Nut::RenderCommandQueue::Execute()
{
	byte* buffer = m_CommandBuffer;

	for (uint32_t i = 0; i < m_CommandCount; ++i) {
		RenderCommandFn function = *(RenderCommandFn*)buffer;
		buffer += sizeof(RenderCommandFn);

		uint32_t size = *(uint32_t*)buffer;
		buffer += sizeof(uint32_t);
		function(buffer);
		buffer += size;
	}

	m_CommandBufferPtr = m_CommandBuffer;
	m_CommandCount = 0;
}
