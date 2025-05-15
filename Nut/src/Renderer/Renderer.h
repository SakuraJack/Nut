#pragma once

#include "RendererAPI.h"

namespace Nut {
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginFrame();
		static void EndFrame();
		static void SetViewport(int x, int y, int width, int height);
		static void Resize(uint32_t width, uint32_t height);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);

		static void SetScissor(glm::vec4 scissor, bool enabled = false);
	public:
		static RendererAPIStatus GetStatus() { return RendererAPI::GetRendererAPIStatus(); }
	};
}
