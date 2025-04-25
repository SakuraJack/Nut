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
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}
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