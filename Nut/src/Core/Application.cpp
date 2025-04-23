#include "ntpch.h"
#include "Application.h"
#include "Core.h"

namespace Nut {
	Application* Application::s_Instance = nullptr;
}

Nut::Application::Application()
{
	s_Instance = this;
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(NUT_BIND_EVENT_FN(Application::OnEvent));
}

Nut::Application::~Application()
{
}

void Nut::Application::Run()
{
	while (m_Running)
	{
		m_Window->OnUpdate();
	}
}

void Nut::Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(NUT_BIND_EVENT_FN(Application::OnWindowClosed));
	dispatcher.Dispatch<WindowResizeEvent>(NUT_BIND_EVENT_FN(Application::OnWindowResized));
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
	glViewport(0, 0, e.GetWidth(), e.GetHeight());
	return true;
}

Nut::Application* Nut::CreateApplication()
{
	return new Application();
}