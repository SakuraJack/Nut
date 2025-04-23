#include "ntpch.h"
#include "Window.h"
#include "Core.h"
#include "Events/ApplicationEvent.h"

namespace Nut
{
	static bool s_GLFWInitialized = false;
	static void GLFWErrorCallback(int error, const char* description)
	{
		//BUST_ERROR("GLFW Error ({0}): {1}", error, description);
	}
}

Nut::Window::Window(const WindowProps& props)
{
	Init(props);
}

Nut::Window::~Window()
{
	Shutdown();
}

void Nut::Window::OnUpdate()
{
	glfwPollEvents();
	m_Context->SwapBuffers();
}

void Nut::Window::SetVSync(bool enabled)
{
	if (enabled) {
		glfwSwapInterval(1);
	} else {
		glfwSwapInterval(0);
	}
	m_Data.m_VSync = enabled;
}

Nut::Window* Nut::Window::Create(const WindowProps& props)
{
	return new Window(props);
}

void Nut::Window::Init(const WindowProps& props)
{
	m_Data.m_Title = props.m_Title;
	m_Data.m_Width = props.m_Width;
	m_Data.m_Height = props.m_Height;

	if (!s_GLFWInitialized) {
		int success = glfwInit();
		glfwSetErrorCallback(GLFWErrorCallback);
		s_GLFWInitialized = true;
	}

	m_Window = glfwCreateWindow(props.m_Width, props.m_Height, props.m_Title.c_str(), nullptr, nullptr);
	m_Context = new GraphicsContext(m_Window);
	m_Context->Init();

	glfwSetWindowUserPointer(m_Window, &m_Data);

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.EventCallback(event);
	});

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.m_Width = width;
		data.m_Height = height;

		WindowResizeEvent event(width, height);
		data.EventCallback(event);
	});

}

void Nut::Window::Shutdown()
{
	glfwDestroyWindow(m_Window);
}
