#include "ntpch.h"
#include "Application.h"
#include "Input.h"
#include "Nut/Renderer/Renderer.h"
#include "Nut/Renderer/Mesh.h"
#include "iostream"
#include "Nut/Runtime/RuntimeLayer.h"

extern bool g_ApplicationRunning;

float GetTime() {
	return (float)glfwGetTime();
}

namespace Nut {
	Application* Application::s_Instance = nullptr;
}

Nut::Application::Application(const ApplicationSpecification& specification)
	: m_Specification(specification), m_RenderThread(specification.RenderThreadingPolicy)
{
	s_Instance = this;
	WindowProps props;
	props.m_Title = m_Specification.Name;
	props.m_Width = m_Specification.WindowWidth;
	props.m_Height = m_Specification.WindowHeight;
	m_Window = std::unique_ptr<Window>(Window::Create(props));
	m_Window->SetEventCallback(NUT_BIND_EVENT_FN(Application::OnEvent));
	Renderer::Init();
	if (m_Specification.RenderThreadingPolicy == ThreadingPolicy::MultiThreaded) {
		m_Window->GetRenderContext()->ReleaseContext();
		m_RenderThread.Run();
	}
	Renderer::Resize(m_Window->GetWidth(), m_Window->GetHeight());
	m_LayerStack.PushLayer(new RuntimeLayer());
}

Nut::Application::~Application()
{
	if (m_RenderThread.GetThreadingPolicy() == ThreadingPolicy::MultiThreaded) {
		m_RenderThread.Terminate();
	}

	for(Layer* layer : m_LayerStack) {
		layer->OnDetach();
		delete layer;
	}
	Renderer::Shutdown();
}

void Nut::Application::Run()
{
	while (m_Running)
	{
		m_RenderThread.BlockUntilRenderComplete();

		ProcessEvents();

		if (m_RenderThread.GetThreadingPolicy() == ThreadingPolicy::MultiThreaded) {
			m_RenderThread.NextFrame();
			m_RenderThread.Kick();
		}
		else {
			Renderer::WaitAndRender();
		}

		if (!m_Minimized) {

			Renderer::BeginFrame();
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate(m_Timestep);
			}
			Renderer::EndFrame();
			Renderer::Submit([&]() {
				m_Window->SwapBuffers();
				});
		}

		Input::ClearReleasedKeys();

		float time = GetTime();
		m_Frametime = time - m_LastFrameTime;
		m_Timestep = glm::min<float>(m_Frametime, 0.0333f);
		m_LastFrameTime = time;
	}

	OnShutdown();
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

void Nut::Application::OnShutdown()
{
	g_ApplicationRunning = false;
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

	m_Window->ProcessEvents();
}