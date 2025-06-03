#include "ntpch.h"
#include "Renderer.h"
#include "Context.h"
#include "Nut/Core/Application.h"

namespace Nut {

	static std::shared_ptr<GraphicsContext> GetContext() {
		return Application::Get().GetWindow()->GetRenderContext();
	}

	struct RendererData
	{
		std::shared_ptr<Texture2D> WhiteTexture;
		std::shared_ptr<Texture2D> BlackTexture;
		std::shared_ptr<TextureCube> WhiteCubeTexture;
	};

	constexpr static uint32_t s_RenderCommandQueueCount = 2;
	static RenderCommandQueue* s_CommandQueue[s_RenderCommandQueueCount];
	static std::atomic<uint32_t> s_RenderCommandQueueSubmissionIndex = 0;
	static RendererData* s_Data = nullptr;
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
	s_Data = nnew RendererData();
	RendererAPI::Init();

	uint32_t whiteTextureData = 0xffffffff;
	TextureSpecification spec;
	spec.Format = ImageFormat::RGBA;
	spec.Width = 1;
	spec.Height = 1;
	s_Data->WhiteTexture = Texture2D::Create(spec, Buffer(&whiteTextureData, sizeof(uint32_t)));

	uint32_t blackTextureData = 0xff000000; //OpenGL采用小端字节序
	s_Data->BlackTexture = Texture2D::Create(spec, Buffer(&blackTextureData, sizeof(uint32_t)));

	std::vector<Buffer> whiteCubeTextureData = { Buffer(&whiteTextureData, sizeof(uint32_t)), 
		Buffer(&whiteTextureData, sizeof(uint32_t)), 
		Buffer(&whiteTextureData, sizeof(uint32_t)), 
		Buffer(&whiteTextureData, sizeof(uint32_t)), 
		Buffer(&whiteTextureData, sizeof(uint32_t)), Buffer(&whiteTextureData, sizeof(uint32_t)) };
	s_Data->WhiteCubeTexture = TextureCube::Create(spec, whiteCubeTextureData);
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

void Nut::Renderer::RenderStaticMesh(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<StaticMesh> mesh, uint32_t submeshIndex, std::shared_ptr<MaterialTable> materialTable)
{
	RendererAPI::RenderStaticMesh(pipeline, mesh, submeshIndex, materialTable);
}

std::shared_ptr<Nut::Texture2D> Nut::Renderer::GetWhiteTexture()
{
	return s_Data->WhiteTexture;
}

std::shared_ptr<Nut::Texture2D> Nut::Renderer::GetBlackTexture()
{
	return s_Data->BlackTexture;
}

std::shared_ptr<Nut::TextureCube> Nut::Renderer::GetWhiteCubeTexture()
{
	return s_Data->WhiteCubeTexture;
}
