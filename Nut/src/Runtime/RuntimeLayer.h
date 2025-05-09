#pragma once

#include "Core/Layer.h"
#include "Editor/EditorCamera.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"

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
		std::shared_ptr<Nut::VertexArray> m_VertexArray;
		std::shared_ptr<Nut::VertexBuffer> m_VertexBuffer;
		std::shared_ptr<Nut::IndexBuffer> m_IndexBuffer;
		std::shared_ptr<Nut::Shader> m_Shader;
		EditorCamera m_EditorCamera;
	};
}