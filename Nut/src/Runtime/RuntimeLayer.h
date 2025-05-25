#pragma once

#include "Core/Layer.h"
#include "Editor/EditorCamera.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include "Renderer/Texture.h"
#include "Renderer/Image.h"

#include "Tool/SimpleGeometryGenerator.h"


namespace Nut {

	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer();
		~RuntimeLayer() override;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;

	private:
		// ≤‚ ‘
		std::shared_ptr<MeshSource> m_MeshSource;
		std::shared_ptr<StaticMesh> m_Mesh;
		EditorCamera m_EditorCamera;
		glm::vec2 GetMousePosInLocal(float x, float y);
	};
}