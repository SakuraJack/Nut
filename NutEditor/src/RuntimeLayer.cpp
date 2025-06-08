#include "ntpch.h"
#include "Nut/Core/Input.h"
#include "RuntimeLayer.h"
#include "Nut/Renderer/Mesh.h"
#include "Nut/Renderer/Renderer.h"
#include "Nut/Renderer/MeshFactory.h"
#include "Nut/Asset/AssetManager.h"

#include <GLFW/glfw3.h>

Nut::RuntimeLayer::RuntimeLayer()
	: Layer("RuntimeLayer"), m_EditorCamera(45.f, 1600.f, 900.f, 0.1f, 100.f)
{
	Project::SetActive(std::make_shared<Project>());
}

Nut::RuntimeLayer::~RuntimeLayer()
{
}

void Nut::RuntimeLayer::OnAttach()
{
	m_Floor = MeshFactory::CreateBoxTexture(glm::vec3(4.f, 1.f, 4.f));
	m_Skybox = MeshFactory::CreateBoxGeometry(glm::vec3(1.f, 1.f, 1.f));
	m_TransformBuffer = VertexBuffer::Create(sizeof(TransformVertexData) * 100);
	TextureSpecification spec;
	spec.DebugName = "SkyBoxTexture";
	spec.Type = TextureType::TextureCube;
	spec.Format = ImageFormat::RGBA;
	spec.Width = 1;
	spec.Height = 1;
	m_SkyBoxMaterial = Material::Create(Renderer::GetShaderLibrary()->Get("Skybox"));
	m_SkyBoxTexture = TextureCube::Create(spec, "Resources/Images/skybox");
	m_SkyBoxMaterial->Set("u_Texture", m_SkyBoxTexture);

	m_FloorMaterial = Material::Create("FloorShader");

	std::vector<TransformVertexData> transforms(100);
	int idx = 0;
	for (int z = 0; z < 10; ++z) {
		for (int x = 0; x < 10; ++x) {
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x * 4.0f, 0.0f, z * 4.0f));
			TransformVertexData& storage = transforms[idx++];
			storage.MRow[0] = { transform[0][0], transform[1][0], transform[2][0], transform[3][0] };
			storage.MRow[1] = { transform[0][1], transform[1][1], transform[2][1], transform[3][1] };
			storage.MRow[2] = { transform[0][2], transform[1][2], transform[2][2], transform[3][2] };
		}
	}

	// Ìî³äµ½ VertexBuffer
	m_TransformBuffer->SetData(transforms.data(), sizeof(TransformVertexData) * transforms.size(), 0);
}

void Nut::RuntimeLayer::OnDetach()
{

}

void Nut::RuntimeLayer::OnUpdate(Timestep ts)
{
	m_EditorCamera.OnUpdate(ts);
	{
		auto mesh = AssetManager::GetAsset<StaticMesh>(m_Floor);
		auto meshSource = AssetManager::GetAsset<MeshSource>(mesh->GetMeshSource());
		m_FloorMaterial->Set("ViewProjectionMatrix", m_EditorCamera.GetViewProjectionMatrix());
		m_FloorMaterial->Set("InverseViewProjectionMatrix", m_EditorCamera.GetInverseViewProjectionMatrix());
		m_FloorMaterial->Set("ProjectionMatrix", m_EditorCamera.GetProjectionMatrix());
		m_FloorMaterial->Set("InverseProjectionMatrix", m_EditorCamera.GetInverseProjectionMatrix());
		m_FloorMaterial->Set("ViewMatrix", m_EditorCamera.GetViewMatrix());
		m_FloorMaterial->Set("InverseViewMatrix", m_EditorCamera.GetInverseViewMatrix());
		Renderer::RenderStaticMeshWithMaterial(m_Pipeline, mesh, meshSource, 0, m_TransformBuffer, 0, 100, m_FloorMaterial);
	}


	{
		auto mesh = AssetManager::GetAsset<StaticMesh>(m_Skybox);
		auto meshSource = AssetManager::GetAsset<MeshSource>(mesh->GetMeshSource());
		m_SkyBoxMaterial->Set("ViewProjectionMatrix", m_EditorCamera.GetViewProjectionMatrix());
		m_SkyBoxMaterial->Set("InverseViewProjectionMatrix", m_EditorCamera.GetInverseViewProjectionMatrix());
		m_SkyBoxMaterial->Set("ProjectionMatrix", m_EditorCamera.GetProjectionMatrix());
		m_SkyBoxMaterial->Set("InverseProjectionMatrix", m_EditorCamera.GetInverseProjectionMatrix());
		m_SkyBoxMaterial->Set("ViewMatrix", m_EditorCamera.GetViewMatrix());
		m_SkyBoxMaterial->Set("InverseViewMatrix", m_EditorCamera.GetInverseViewMatrix());
		Renderer::RenderStaticMeshWithMaterial(m_Pipeline, mesh, meshSource, 0, nullptr, 0, 0, m_SkyBoxMaterial);
	}
}

void Nut::RuntimeLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	m_EditorCamera.OnEvent(e);
}
