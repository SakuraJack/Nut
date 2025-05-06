#include "ntpch.h"
#include "Application.h"
#include "Core.h"
#include "Log.h"
#include "Renderer/Renderer.h"
#include "Renderer/Mesh.h"
#include "iostream"

float GetTimeInSeconds() {
	using namespace std::chrono;
	static auto startTime = high_resolution_clock::now(); // 程序启动时间
	auto currentTime = high_resolution_clock::now();
	duration<float> elapsedTime = currentTime - startTime; // 计算经过的时间
	return elapsedTime.count(); // 返回秒数
}
float dissolveCoefficient = 1.0f;
float noiseCoefficient = 5.0f;

namespace Nut {
	Application* Application::s_Instance = nullptr;
}

void Nut::Application::TestFunction()
{
	Nut::Vertex v1 = { {0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f} };
	Nut::Vertex v2 = { {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f} };
	Nut::Vertex v3 = { {0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f} };
	Nut::Vertex v4 = { {-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f} };
	std::vector<Nut::Vertex> vertices = { v1, v2, v3, v4 };
	std::vector<Nut::Index> indices = { {0, 1, 2}, {0, 3, 1} };
	Nut::SubMesh subMesh;
	subMesh.m_BaseVertex = 0;
	subMesh.m_BaseIndex = 0;
	subMesh.m_VertexCount = 4;
	subMesh.m_IndexCount = 2;
	subMesh.m_LocalTransform = glm::mat4(1.0f);
	subMesh.m_GlobalTransform = glm::mat4(1.0f);
	Nut::Mesh mesh(vertices, indices, { subMesh });
	m_VertexBuffer = Nut::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Nut::Vertex), vertices.size());
	m_VertexBuffer->SetLayout({ {"a_Position", Nut::DataType::Float3},
								{"a_Color", Nut::DataType::Float3},
								{"a_TexCoord", Nut::DataType::Float2},
								{"a_Tangent", Nut::DataType::Float3},
								{"a_Bitangent", Nut::DataType::Float3} });
	m_IndexBuffer = Nut::IndexBuffer::Create(indices.data(), indices.size() * sizeof(Nut::Index), indices.size() * 3);
	m_VertexArray = Nut::VertexArray::Create(m_VertexBuffer, m_IndexBuffer);
	m_Shader = Nut::Shader::Create("testShader", "D:\\dev\\Nut\\Nut\\resources\\testshader.glsl");
	m_Shader->Reload("D:\\dev\\Nut\\Nut\\resources\\testshader.glsl");
}

Nut::Application::Application()
{
	s_Instance = this;
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(NUT_BIND_EVENT_FN(Application::OnEvent));
	Renderer::Init();
	Renderer::Resize(m_Window->GetWidth(), m_Window->GetHeight());
	TestFunction();
	m_Shader->Bind();
}

Nut::Application::~Application()
{
}

void Nut::Application::Run()
{
	while (m_Running)
	{
		if (!m_Minimized) {
			Renderer::BeginFrame();
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}
			/*m_Shader->SetUniform("aTime", GetTimeInSeconds());
			m_Shader->SetUniform("aDissolveCoefficient", dissolveCoefficient);
			m_Shader->SetUniform("aNoiseCoefficient", noiseCoefficient);*/
			glUniform1f(2, GetTimeInSeconds());
			float t, d, n;
			glGetUniformfv(m_Shader->GetShaderID(), 2, &t);
			glUniform1f(3, dissolveCoefficient);
			glGetUniformfv(m_Shader->GetShaderID(), 3, &d);
			glUniform1f(0, noiseCoefficient);
			glGetUniformfv(m_Shader->GetShaderID(), 0, &n);
			std::cout << t << " " << d << " " << n << std::endl;
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
			Renderer::EndFrame();
			m_Window->OnUpdate();
		}
		else {
			m_Window->OnUpdate();
		}
	}
}

void Nut::Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(NUT_BIND_EVENT_FN(Application::OnWindowClosed));
	dispatcher.Dispatch<WindowResizeEvent>(NUT_BIND_EVENT_FN(Application::OnWindowResized));
	dispatcher.Dispatch<KeyPressedEvent>(NUT_BIND_EVENT_FN(Application::OnKeyPressed));
	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
		(*--it)->OnEvent(e);
		if (e.m_Handled) {
			break;
		}
	}
}

void Nut::Application::Close()
{
	m_Running = false;
}

bool Nut::Application::OnWindowClosed(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

bool Nut::Application::OnWindowResized(WindowResizeEvent& e)
{
	if (e.GetHeight() == 0 || e.GetWidth() == 0) {
		m_Minimized = true;
		return false;
	}
	m_Minimized = false;
	Renderer::Resize(e.GetWidth(), e.GetHeight());
	return true;
}

bool Nut::Application::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == NUT_KEY_UP) {
		dissolveCoefficient += 0.1f;
	}
	else if (e.GetKeyCode() == NUT_KEY_DOWN) {
		dissolveCoefficient -= 0.1f;
	}
	else if (e.GetKeyCode() == NUT_KEY_LEFT) {
		noiseCoefficient -= 1.0f;
	}
	else if (e.GetKeyCode() == NUT_KEY_RIGHT) {
		noiseCoefficient += 1.0f;
	}
	else if (e.GetKeyCode() == NUT_KEY_ESCAPE) {
		m_Running = false;
	}
	return true;
}

Nut::Application* Nut::CreateApplication()
{
	return new Application();
}