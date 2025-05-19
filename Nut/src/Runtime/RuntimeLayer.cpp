#include "ntpch.h"
#include "Core/Core.h"
#include "RuntimeLayer.h"
#include "Renderer/Mesh.h"
#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>

float dissolveEdge = 0.15f;
float dissolvePercent = 1.0f;
float noiseDensity = 2.f;

Nut::RuntimeLayer::RuntimeLayer()
	: Layer("RuntimeLayer"), m_EditorCamera(45.f, 1600.f, 900.f, 0.1f, 100.f)
{
}	

Nut::RuntimeLayer::~RuntimeLayer()
{

}

void Nut::RuntimeLayer::OnAttach()
{
	m_MeshSource = Nut::GeometryGenerator::CreateCube(glm::vec3(0.f), 2.0f);
	m_Mesh = std::make_shared<StaticMesh>(m_MeshSource);
	Shader::SetUniformBufferUniform("u_MatrixBuffer", "model", &m_Mesh->GetMeshSource()->GetSubmeshes()[0].m_GlobalTransform);
	Shader::SetUniformBufferUniform("u_MatrixBuffer", "view", &m_EditorCamera.GetViewMatrix());
	Shader::SetUniformBufferUniform("u_MatrixBuffer", "projection", &m_EditorCamera.GetProjectionMatrix());

}

void Nut::RuntimeLayer::OnDetach()
{

}

void Nut::RuntimeLayer::OnUpdate(Timestep ts)
{
	m_EditorCamera.OnUpdate(ts);
	m_Mesh->GetMeshSource()->GetVertexArray()->Bind();
	m_Mesh->GetMaterials()[0]->Bind();
	Shader::SetUniformBufferUniform("u_MatrixBuffer", "view", &m_EditorCamera.GetViewMatrix());
	Shader::SetUniformBufferUniform("u_MatrixBuffer", "projection", &m_EditorCamera.GetProjectionMatrix());
	Renderer::Submit([=]() {
		glDrawElements(GL_TRIANGLES, m_Mesh->GetMeshSource()->GetIndices().size() * 3, GL_UNSIGNED_INT, 0);
		});
}

void Nut::RuntimeLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	m_EditorCamera.OnEvent(e);
}
