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
		std::shared_ptr<ShaderLibrary> ShaderLibrary;

		std::shared_ptr<Texture2D> WhiteTexture;
		std::shared_ptr<Texture2D> BlackTexture;
		std::shared_ptr<TextureCube> WhiteCubeTexture;
	};

	constexpr static uint32_t s_RenderCommandQueueCount = 2;
	static RenderCommandQueue* s_CommandQueue[s_RenderCommandQueueCount];
	static std::atomic<uint32_t> s_RenderCommandQueueSubmissionIndex = 0;
	static RendererData* s_Data = nullptr;

	void Renderer::WaitAndRender(RenderThread* renderThread)
	{
		{
			renderThread->WaitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);
		}

		s_CommandQueue[GetRenderQueueIndex()]->Execute();

		renderThread->Set(RenderThread::State::Idle);
	}

	void Renderer::RenderThreadFunc(RenderThread* renderThread)
	{
		GetContext()->SwitchContext();
		while (renderThread->IsRunning())
		{
			WaitAndRender(renderThread);
		}
	}

	void Renderer::SwapQueues()
	{
		s_RenderCommandQueueSubmissionIndex = (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
	}

	void Renderer::WaitAndRender()
	{
		s_CommandQueue[0]->Execute();
	}

	uint32_t Renderer::GetRenderQueueIndex()
	{
		return (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
	}

	RenderCommandQueue& Renderer::GetRenderCommandQueue()
	{
		return *s_CommandQueue[s_RenderCommandQueueSubmissionIndex];
	}

	void Renderer::Init()
	{
		s_CommandQueue[0] = nnew RenderCommandQueue();
		s_CommandQueue[1] = nnew RenderCommandQueue();
		s_Data = nnew RendererData();
		RendererAPI::Init();

		s_Data->ShaderLibrary = std::make_shared<ShaderLibrary>();

		Renderer::GetShaderLibrary()->Load("Skybox", std::string("Resources/Shaders/SkyBoxShader.glsl"));

		// Compile shaders
		//Application::Get().GetRenderThread().Pump();

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

	void Renderer::Shutdown()
	{
		RendererAPI::Shutdown();
	}

	void Renderer::BeginFrame()
	{
		RendererAPI::BeginFrame();
	}

	void Renderer::EndFrame()
	{
		RendererAPI::EndFrame();
	}

	std::shared_ptr<ShaderLibrary> Renderer::GetShaderLibrary()
	{
		return s_Data->ShaderLibrary;
	}

	void Renderer::SetViewport(int x, int y, int width, int height)
	{
		RendererAPI::SetViewport(x, y, width, height);
	}

	void Renderer::Resize(uint32_t width, uint32_t height)
	{
		RendererAPI::Resize(width, height);
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		RendererAPI::SetClearColor(color);
	}

	void Renderer::Clear()
	{
		RendererAPI::Clear();
	}

	void Renderer::SetScissor(glm::vec4 scissor, bool enabled)
	{
		RendererAPI::SetScissorEnabled(enabled);
		RendererAPI::SetScissor(scissor.x, scissor.y, scissor.z, scissor.w);
	}

	void Renderer::RenderStaticMesh(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<StaticMesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<MaterialTable> materialTable, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t instanceCount)
	{
		RendererAPI::RenderStaticMesh(pipeline, mesh, meshSource, submeshIndex, materialTable, transformBuffer, transformOffset, instanceCount);
	}

	void Renderer::RenderSubmeshInstanced(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Mesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<MaterialTable> materialTable, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t boneTransformsOffset, uint32_t instanceCount)
	{
		RendererAPI::RenderSubmeshInstanced(pipeline, mesh, meshSource, submeshIndex, materialTable, transformBuffer, transformOffset, boneTransformsOffset, instanceCount);
	}

	void Renderer::RenderMeshWithMaterial(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Mesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t instanceCount, std::shared_ptr<Material> material, Buffer additionalUniforms)
	{
		RendererAPI::RenderMeshWithMaterial(pipeline, mesh, meshSource, submeshIndex, transformBuffer, transformOffset, instanceCount, material, additionalUniforms);
	}

	void Renderer::RenderStaticMeshWithMaterial(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<StaticMesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t instanceCount, std::shared_ptr<Material> material, Buffer additionalUniforms)
	{
		RendererAPI::RenderStaticMeshWithMaterial(pipeline, mesh, meshSource, submeshIndex, transformBuffer, transformOffset, instanceCount, material, additionalUniforms);
	}

	void Renderer::RenderQuad(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material, const glm::mat4& transform)
	{
		RendererAPI::RenderQuad(pipeline, material, transform);
	}

	void Renderer::SubmitFullscreenQuad(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material)
	{
		RendererAPI::SubmitFullscreenQuad(pipeline, material);
	}

	void Renderer::SubmitFullscreenQuadWithOverrides(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material, Buffer vertexShaderOverrides, Buffer fragmentShaderOverrides)
	{
		RendererAPI::SubmitFullscreenQuadWithOverrides(pipeline, material, vertexShaderOverrides, fragmentShaderOverrides);
	}

	void Renderer::RenderGeometry(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, const glm::mat4& transform, uint32_t indexCount)
	{
		RendererAPI::RenderGeometry(pipeline, material, vertexBuffer, indexBuffer, transform, indexCount);
	}

	void Renderer::SubmitQuad(std::shared_ptr<Material> material, const glm::mat4& transform)
	{
		RendererAPI::SubmitQuad(material, transform);
	}

	std::shared_ptr<Texture2D> Renderer::GetWhiteTexture()
	{
		return s_Data->WhiteTexture;
	}

	std::shared_ptr<Texture2D> Renderer::GetBlackTexture()
	{
		return s_Data->BlackTexture;
	}

	std::shared_ptr<TextureCube> Renderer::GetWhiteCubeTexture()
	{
		return s_Data->WhiteCubeTexture;
	}
}
