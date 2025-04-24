#include "ntpch.h"
#include "Application.h"
#include "Core.h"
#include "Renderer/Renderer.h"
#include "Log.h"

namespace Nut {
	Application* Application::s_Instance = nullptr;
}

Nut::Application::Application()
{
	s_Instance = this;
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(NUT_BIND_EVENT_FN(Application::OnEvent));
	Renderer::Init();
	Renderer::Resize(m_Window->GetWidth(), m_Window->GetHeight());
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
			m_Window->OnUpdate();
			Renderer::EndFrame();
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
	if (e.GetKeyCode() == GLFW_KEY_S) {
		Renderer::SetClearColor(glm::vec4(1, 0, 0, 1));
	}
	else if (e.GetKeyCode() == GLFW_KEY_D) {
		Renderer::SetClearColor(glm::vec4(0, 1, 0, 1));
	}
	else if (e.GetKeyCode() == GLFW_KEY_M) {
	}
	return true;
}

Nut::Application* Nut::CreateApplication()
{
	return new Application();
}