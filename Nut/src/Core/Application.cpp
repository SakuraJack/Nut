#include "ntpch.h"
#include "Application.h"
#include "Core.h"
#include "Log.h"
#include "Renderer/Renderer.h"
#include "Renderer/Mesh.h"

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
	m_VertexBuffer = std::make_shared<Nut::VertexBuffer>(vertices.data(), vertices.size() * sizeof(Nut::Vertex), vertices.size());
	m_VertexBuffer->SetLayout({ {"a_Position", Nut::DataType::Float3},
								{"a_Color", Nut::DataType::Float3},
								{"a_TexCoord", Nut::DataType::Float2},
								{"a_Tangent", Nut::DataType::Float3},
								{"a_Bitangent", Nut::DataType::Float3} });
	m_IndexBuffer = std::make_shared<Nut::IndexBuffer>(indices.data(), indices.size() * sizeof(Nut::Index), indices.size() * 3);
	m_VertexArray = std::make_shared<Nut::VertexArray>(m_VertexBuffer, m_IndexBuffer);

	const char* vertexShaderSource = "#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"   ourColor = aColor;\n"
		"}\0";
	const char* fragmentShaderSource = "#version 460 core\n"
		"in vec3 ourColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(ourColor, 1.0f);\n"
		"}\n\0";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		NUT_ERROR_TAG("Shader", "Vertex shader compilation failed: {0}", infoLog);
	}
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		NUT_ERROR_TAG("Shader", "Fragment shader compilation failed: {0}", infoLog);
	}
	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		NUT_ERROR_TAG("Shader", "Shader program linking failed: {0}", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Nut::Application::Application()
{
	s_Instance = this;
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(NUT_BIND_EVENT_FN(Application::OnEvent));
	Renderer::Init();
	Renderer::Resize(m_Window->GetWidth(), m_Window->GetHeight());
	TestFunction();
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
			glUseProgram(shaderProgram);
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

Nut::Application* Nut::CreateApplication()
{
	return new Application();
}