#pragma once

#include "RendererAPI.h"
#include "RenderCommandQueue.h"
#include "Nut/Thread/RenderThread.h"
#include "Pipeline.h"
#include "Mesh.h"

namespace Nut {
	class Renderer
	{
	public:
		// 渲染队列以及渲染线程
		template<typename FuncT>
		static void Submit(FuncT&& func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				pFunc->~FuncT(); // Destructor
			};
			auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

		static void WaitAndRender(RenderThread* renderThread);
		static void RenderThreadFunc(RenderThread* renderThread);
		static void SwapQueues();
		static void WaitAndRender();
		static uint32_t GetRenderQueueIndex();
	private:
		static RenderCommandQueue& GetRenderCommandQueue();

	public:
		static void Init();
		static void Shutdown();
		static void BeginFrame();
		static void EndFrame();

		static std::shared_ptr<ShaderLibrary> GetShaderLibrary();

		// TODO: 封装进Pipeline
		static void SetViewport(int x, int y, int width, int height);
		static void Resize(uint32_t width, uint32_t height);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void SetScissor(glm::vec4 scissor, bool enabled = false);

		static void RenderStaticMesh(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<StaticMesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<MaterialTable> materialTable, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t instanceCount);
		static void RenderSubmeshInstanced(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Mesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<MaterialTable> materialTable, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t boneTransformsOffset, uint32_t instanceCount);
		static void RenderMeshWithMaterial(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Mesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t instanceCount, std::shared_ptr<Material> material, Buffer additionalUniforms = Buffer());
		static void RenderStaticMeshWithMaterial(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<StaticMesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<VertexBuffer> transformBuffer, uint32_t transformOffset, uint32_t instanceCount, std::shared_ptr<Material> material, Buffer additionalUniforms = Buffer());
		static void RenderQuad(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material, const glm::mat4& transform);
		static void SubmitFullscreenQuad(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material);
		static void SubmitFullscreenQuadWithOverrides(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material, Buffer vertexShaderOverrides, Buffer fragmentShaderOverrides);
		//static void LightCulling(std::shared_ptr<ComputePass> computePass, std::shared_ptr<Material> material, const glm::uvec3& workGroups);
		static void RenderGeometry(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Material> material, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, const glm::mat4& transform, uint32_t indexCount = 0);
		static void SubmitQuad(std::shared_ptr<Material> material, const glm::mat4& transform = glm::mat4(1.0f));

		static std::shared_ptr<Texture2D> GetWhiteTexture();
		static std::shared_ptr<Texture2D> GetBlackTexture();
		static std::shared_ptr<TextureCube> GetWhiteCubeTexture();

	public:
		static RendererAPIStatus GetStatus() { return RendererAPI::GetRendererAPIStatus(); }
	};
}
