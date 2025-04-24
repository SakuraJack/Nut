#include "ntpch.h"
#include "Renderer.h"

void Nut::Renderer::Init()
{
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
