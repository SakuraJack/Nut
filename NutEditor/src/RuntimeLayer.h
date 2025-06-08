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
#include "Nut/Project/Project.h"
#include "Nut/Renderer/Pipeline.h"


namespace Nut {

	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer();
		virtual ~RuntimeLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;



	private:
		// ≤‚ ‘
		EditorCamera m_EditorCamera;
		AssetHandle m_Floor;
		AssetHandle m_Skybox;
		std::shared_ptr<Material> m_SkyBoxMaterial;
		std::shared_ptr<Material> m_FloorMaterial;
		std::shared_ptr<TextureCube> m_SkyBoxTexture;
		std::shared_ptr<Pipeline> m_Pipeline;
		std::shared_ptr<VertexBuffer> m_TransformBuffer;

		struct TransformVertexData
		{
			glm::vec4 MRow[3];
		};
	};
}