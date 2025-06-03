#pragma once

#include "Nut/Core/Layer.h"
#include "Nut/Editor/EditorCamera.h"
#include "Nut/Renderer/VertexArray.h"
#include "Nut/Renderer/VertexBuffer.h"
#include "Nut/Renderer/IndexBuffer.h"
#include "Nut/Renderer/Shader.h"
#include "Nut/Renderer/Material.h"
#include "Nut/Renderer/Texture.h"
#include "Nut/Renderer/Image.h"
#include "Nut/Tool/SimpleGeometryGenerator.h"


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