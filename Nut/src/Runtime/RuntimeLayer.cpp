#include "ntpch.h"
#include "Core/Core.h"
#include "RuntimeLayer.h"
#include "Renderer/Mesh.h"

#include <GLFW/glfw3.h>

float dissolveEdge = 0.15f;
float dissolvePercent = 1.0f;
float noiseDensity = 2.f;

Nut::RuntimeLayer::RuntimeLayer()
	: Layer("RuntimeLayer"), m_EditorCamera(45.f, 1280.f, 720.f, 0.1f, 100.f)
{
}	

Nut::RuntimeLayer::~RuntimeLayer()
{

}

void Nut::RuntimeLayer::OnAttach()
{
	//std::shared_ptr<Nut::Mesh> cube = Nut::GeometryGenerator::CreateCube(glm::vec3(0.f), 2.0f);
	//m_VertexBuffer = Nut::VertexBuffer::Create((void*)cube->GetVertices().data(), cube->GetVertices().size() * sizeof(Nut::Vertex), cube->GetVertices().size());
	//m_VertexBuffer->SetLayout({ {"a_Position", Nut::DataType::Float3},
	//							{"a_Color", Nut::DataType::Float3},
	//							{"a_TexCoord", Nut::DataType::Float2},
	//							{"a_Tangent", Nut::DataType::Float3},
	//							{"a_Bitangent", Nut::DataType::Float3} });
	//m_IndexBuffer = Nut::IndexBuffer::Create((void*)cube->GetIndices().data(), cube->GetIndices().size() * sizeof(Nut::Index), cube->GetIndices().size() * 3);
	//m_VertexArray = Nut::VertexArray::Create(m_VertexBuffer, m_IndexBuffer);
	//m_Shader = Nut::Shader::Create("SkyBox", "D:\\dev\\Nut\\Nut\\resources\\Shaders\\SkyBoxShader.glsl");
	//m_Material = Nut::Material::Create(m_Shader, "SkyBox");
	//TextureSpecification spec;
	//spec.DebugName = "SkyBox";
	//spec.Type = TextureType::TextureCube;
	//spec.SamplerWrap = TextureWrap::Clamp;
	//m_TextureCube = Nut::TextureCube::Create(spec, "D:\\dev\\Nut\\Nut\\resources\\Images\\skybox");
	//m_VertexArray->Bind();
	//glm::mat4 modelMatrix = glm::mat4(1.f);
	//m_Material->Bind();
	//m_Material->Set("u_Skybox", m_TextureCube);
	//glDepthFunc(GL_LEQUAL);
	////m_Material->Set("u_Color", glm::vec4(1.f, 0.f, 0.f, 1.f));
	//Shader::SetUniformBufferUniform("u_MatrixBuffer", "model", &modelMatrix);
	//Shader::SetUniformBufferUniform("u_MatrixBuffer", "view", &m_EditorCamera.GetViewMatrix());
	//Shader::SetUniformBufferUniform("u_MatrixBuffer", "projection", &m_EditorCamera.GetProjectionMatrix());

}

void Nut::RuntimeLayer::OnDetach()
{

}

void Nut::RuntimeLayer::OnUpdate(Timestep ts)
{
	m_EditorCamera.OnUpdate(ts);
	m_Material->Bind();
	m_VertexArray->Bind();
	Shader::SetUniformBufferUniform("u_MatrixBuffer", "view", &m_EditorCamera.GetViewMatrix());
	Shader::SetUniformBufferUniform("u_MatrixBuffer", "projection", &m_EditorCamera.GetProjectionMatrix());
}

void Nut::RuntimeLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	m_EditorCamera.OnEvent(e);
}
