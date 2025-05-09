#include "ntpch.h"
#include "Application.h"
#include "Core.h"
#include "Log.h"
#include "Input.h"
#include "Renderer/Renderer.h"
#include "Renderer/Mesh.h"
#include "iostream"
#include "Runtime/RuntimeLayer.h"

float GetTime() {
	return (float)glfwGetTime();
}

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
	m_LayerStack.PushLayer(new RuntimeLayer());
}

Nut::Application::~Application()
{
}

void Nut::Application::Run()
{
	while (m_Running)
	{
		ProcessEvents();

		if (!m_Minimized) {
			Renderer::BeginFrame();
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate(m_Timestep);
			}
			Renderer::EndFrame();
			m_Window->OnUpdate();
		}

		Input::ClearReleasedKeys();

		float time = GetTime();
		m_Frametime = time - m_LastFrameTime;
		m_Timestep = glm::min<float>(m_Frametime, 0.0333f);
		m_LastFrameTime = time;
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
	return true;
}

void Nut::Application::ProcessEvents()
{
	Input::TransitionPressedKeys();
	Input::TransitionPressedButtons();
}

Nut::Application* Nut::CreateApplication()
{
	return new Application();
}