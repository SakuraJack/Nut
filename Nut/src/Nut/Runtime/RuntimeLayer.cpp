#include "ntpch.h"
#include "Nut/Core/Input.h"
#include "RuntimeLayer.h"
#include "Nut/Renderer/Mesh.h"
#include "Nut/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

float dissolveEdge = 0.15f;
float dissolvePercent = 1.0f;
float noiseDensity = 2.f;

float r = 3.0f, e = 0.1f;
glm::vec3 edgeColor = { 0.0f, 0.0f, 1.0f };
glm::vec3 fillColor = { 0.0f, 1.0f, 0.0f };
float fillAlpha = 0.5f;
float highlightDist = 0.1f;
glm::vec2 mousePos{ 0, 0 };

Nut::RuntimeLayer::RuntimeLayer()
	: Layer("RuntimeLayer"), m_EditorCamera(45.f, 1600.f, 900.f, 0.1f, 100.f)
{
}	

Nut::RuntimeLayer::~RuntimeLayer()
{

}

void Nut::RuntimeLayer::OnAttach()
{
	m_MeshSource = Nut::GeometryGenerator::CreateCircle(glm::vec3(0.f), 3.0f);
	//m_MeshSource = Nut::GeometryGenerator::CreateCube(glm::vec3(0.f), 3.0f);
	m_Mesh = std::make_shared<StaticMesh>(m_MeshSource);
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("model", m_Mesh->GetMeshSource()->GetSubmeshes()[0].m_GlobalTransform);
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("view", m_EditorCamera.GetViewMatrix());
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("projection", m_EditorCamera.GetProjectionMatrix());
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("radius", r);
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("edgeWidth", e);
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("edgeColor", edgeColor);
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("fillColor", fillColor);
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("fillAlpha", fillAlpha);
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("highlightDist", highlightDist);
}

void Nut::RuntimeLayer::OnDetach()
{

}

void Nut::RuntimeLayer::OnUpdate(Timestep ts)
{
	m_EditorCamera.OnUpdate(ts);
	Renderer::Submit([=]() {
		glDisable(GL_CULL_FACE);
		});
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("model", m_Mesh->GetMeshSource()->GetSubmeshes()[0].m_GlobalTransform);
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("view", m_EditorCamera.GetViewMatrix());
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("projection", m_EditorCamera.GetProjectionMatrix());
	mousePos = GetMousePosInLocal(Input::GetMouseX(), Input::GetMouseY());
	m_Mesh->GetMeshSource()->GetMaterials()[0]->Set("mousePos", mousePos);
	Renderer::RenderStaticMesh(nullptr, m_Mesh, 0, m_Mesh->GetMaterials());
	/*m_Mesh->GetMeshSource()->GetVertexArray()->Bind();
	m_Mesh->GetMaterials()[0]->Bind();
	Shader::SetUniformBufferUniform("u_MatrixBuffer", "view", &m_EditorCamera.GetViewMatrix());
	Shader::SetUniformBufferUniform("u_MatrixBuffer", "projection", &m_EditorCamera.GetProjectionMatrix());
	mousePos = GetMousePosInLocal(Input::GetMouseX(), Input::GetMouseY());
	Shader::SetUniformBufferUniform("u_DetectParameter", "mousePos", &mousePos);*/
	
}

void Nut::RuntimeLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	m_EditorCamera.OnEvent(e);
}

glm::vec2 Nut::RuntimeLayer::GetMousePosInLocal(float x, float y)
{
	float ndcX = (2.0f * x) / 1600.f - 1.0f;
	float ndcY = 1.0f - (2.0f * y) / 900.f;

	glm::vec4 rayStartNDC = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
	glm::vec4 rayEndNDC = glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

	glm::mat4 invVP = glm::inverse(m_EditorCamera.GetProjectionMatrix() * m_EditorCamera.GetViewMatrix());
	glm::vec4 rayStartWorld = invVP * rayStartNDC; rayStartWorld /= rayStartWorld.w;
	glm::vec4 rayEndWorld = invVP * rayEndNDC; rayEndWorld /= rayEndWorld.w;

	glm::vec3 rayOrigin = glm::vec3(rayStartWorld);
	glm::vec3 rayDirection = glm::normalize(glm::vec3(rayEndWorld - rayStartWorld));

	glm::vec3 circleCenter = glm::vec3(glm::mat4(1.f) * glm::vec4(0.f, 0.f, 0.f, 1.f));
	glm::vec3 circleNormal = glm::vec3(glm::mat4(1.f) * glm::vec4(0.f, 0.f, 1.f, 0.f));

	float denom = glm::dot(circleNormal, rayDirection);
	glm::vec3 mouseWorldPos;
	if (fabs(denom) > 1e-6) {
		float t = glm::dot(circleCenter - rayOrigin, circleNormal) / denom;
		mouseWorldPos = rayOrigin + t * rayDirection;
	}
	else {
		mouseWorldPos = circleCenter;
	}

	glm::mat4 invModel = glm::mat4(1.f);
	glm::vec4 mouseLocal4 = invModel * glm::vec4(mouseWorldPos, 1.f);
	return { mouseLocal4.x, mouseLocal4.y };
}

