#include "ntpch.h"
#include "Renderer.h"
#include "Context.h"
#include "Core/Memory.h"
#include "Core/Application.h"

namespace Nut {
	constexpr static uint32_t s_RenderCommandQueueCount = 2;
	static RenderCommandQueue* s_CommandQueue[s_RenderCommandQueueCount];
	static std::atomic<uint32_t> s_RenderCommandQueueSubmissionIndex = 0;

	static std::shared_ptr<GraphicsContext> GetContext() {
		return Application::Get().GetWindow()->GetRenderContext();
	}
}

void Nut::Renderer::WaitAndRender(RenderThread* renderThread)
{
	{
		renderThread->WaitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);
	}

	s_CommandQueue[GetRenderQueueIndex()]->Execute();

	renderThread->Set(RenderThread::State::Idle);
}

void Nut::Renderer::RenderThreadFunc(RenderThread* renderThread)
{
	GetContext()->SwitchContext();
	while (renderThread->IsRunning())
	{
		WaitAndRender(renderThread);
	}
}

void Nut::Renderer::SwapQueues()
{
	s_RenderCommandQueueSubmissionIndex = (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
}

void Nut::Renderer::WaitAndRender()
{
	s_CommandQueue[0]->Execute();
}

uint32_t Nut::Renderer::GetRenderQueueIndex()
{
	return (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
}

Nut::RenderCommandQueue& Nut::Renderer::GetRenderCommandQueue()
{
	return *s_CommandQueue[s_RenderCommandQueueSubmissionIndex];
}

void Nut::Renderer::Init()
{
	s_CommandQueue[0] = nnew RenderCommandQueue();
	s_CommandQueue[1] = nnew RenderCommandQueue();
	RendererAPI::Init();
}

void Nut::Renderer::Shutdown()
{
	RendererAPI::Shutdown();
}

void Nut::Renderer::BeginFrame()
{
	RendererAPI::BeginFrame();
}

void Nut::Renderer::EndFrame()
{
	RendererAPI::EndFrame();
}

void Nut::Renderer::SetViewport(int x, int y, int width, int height)
{
	RendererAPI::SetViewport(x, y, width, height);
}

void Nut::Renderer::Resize(uint32_t width, uint32_t height)
{
	RendererAPI::Resize(width, height);
}

void Nut::Renderer::SetClearColor(const glm::vec4& color)
{
	RendererAPI::SetClearColor(color);
}

void Nut::Renderer::Clear()
{
	RendererAPI::Clear();
}

void Nut::Renderer::SetScissor(glm::vec4 scissor, bool enabled)
{
	RendererAPI::SetScissorEnabled(enabled);
	RendererAPI::SetScissor(scissor.x, scissor.y, scissor.z, scissor.w);
}
