#include "ntpch.h"
#include "Application.h"

namespace Nut {
	Application* Application::s_Instance = nullptr;
}

Nut::Application::Application()
{
	s_Instance = this;
	m_Window = std::unique_ptr<Window>(Window::Create());
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

void Nut::Application::Close()
{
	m_Running = false;
}

Nut::Application* Nut::CreateApplication()
{
	return new Application();
}