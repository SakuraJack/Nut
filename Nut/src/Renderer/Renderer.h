#pragma once

#include "RendererAPI.h"
#include "RenderCommandQueue.h"
#include "Thread/RenderThread.h"
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

		// TODO: 封装进Pipeline
		static void SetViewport(int x, int y, int width, int height);
		static void Resize(uint32_t width, uint32_t height);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void SetScissor(glm::vec4 scissor, bool enabled = false);

		static void RenderStaticMesh(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<StaticMesh>, uint32_t submeshIndex, std::shared_ptr<Material> material);
		static void RenderSubmeshInstanced(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<StaticMesh> mesh, uint32_t submeshIndex, std::shared_ptr<Material> material, uint32_t instanceCount);
		static void RenderMesh(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
		static void RenderMeshInstanced(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, uint32_t instanceCount);

	public:
		static RendererAPIStatus GetStatus() { return RendererAPI::GetRendererAPIStatus(); }
	};
}
