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
		std::shared_ptr<Nut::VertexArray> m_VertexArray;
		std::shared_ptr<Nut::VertexBuffer> m_VertexBuffer;
		std::shared_ptr<Nut::IndexBuffer> m_IndexBuffer;
		std::shared_ptr<Nut::Shader> m_Shader;
		std::shared_ptr<Nut::Material> m_Material;
		std::shared_ptr<Nut::TextureCube> m_TextureCube;
		EditorCamera m_EditorCamera;
	};
}