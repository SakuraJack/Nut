#include "ntpch.h"
#include "Window.h"
#include "Input.h"
#include "Nut/Events/ApplicationEvent.h"
#include "Nut/Events/MouseEvent.h"
#include "Nut/Events/KeyEvent.h"

namespace Nut
{
	static bool s_GLFWInitialized = false;
	static void GLFWErrorCallback(int error, const char* description)
	{
		// 记录GLFW初始化错误信息
		NUT_CORE_ERROR_TAG("Window", "GLFW Error ({0}): {1}", error, description);
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

void Nut::Window::ProcessEvents()
{
	glfwPollEvents();
}

void Nut::Window::SwapBuffers()
{
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
	m_Context = std::make_shared<GraphicsContext>(m_Window);
	m_Context->Init();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwSetWindowUserPointer(m_Window, &m_Data);

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		NUT_CORE_INFO_TAG("Window", "窗口关闭");
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.EventCallback(event);
	});

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		NUT_CORE_INFO_TAG("Window", "窗口大小改变 {0} x {1}", width, height);
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.m_Width = width;
		data.m_Height = height;

		WindowResizeEvent event(width, height);
		data.EventCallback(event);
	});

	glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		if (focused) {
			NUT_CORE_INFO_TAG("Window", "窗口获得焦点");
			WindowFocusEvent event;
			data.EventCallback(event);
		} else {
			NUT_CORE_INFO_TAG("Window", "窗口失去焦点");
			WindowLostFocusEvent event;
			data.EventCallback(event);
		}
	});

	glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int x, int y) {
		NUT_CORE_INFO_TAG("Window", "窗口移动到 {0}, {1}", x, y);
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.m_XPos = x;
		data.m_YPos = y;
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		if (action == GLFW_PRESS) {
			Input::UpdateButtonState((MouseButton)button, KeyState::Pressed);
			MousePressedEvent event(button);
			data.EventCallback(event);
		} else if (action == GLFW_RELEASE) {
			Input::UpdateButtonState((MouseButton)button, KeyState::Released);
			MouseReleasedEvent event(button);
			data.EventCallback(event);
		}
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
		NUT_CORE_INFO_TAG("Window", "滚轮水平移动 {0}, 垂直移动 {1}", xOffset, yOffset);
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		MouseScrolledEvent event((float)xOffset, (float)yOffset);
		data.EventCallback(event);
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
		//NUT_CORE_INFO_TAG("Window", "光标移动到 {0}, {1}", xPos, yPos);
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		MouseMovedEvent event((float)xPos, (float)yPos);
		data.EventCallback(event);
	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		if (action == GLFW_PRESS) {
			Input::UpdateKeyState((KeyCode)key, KeyState::Pressed);
			KeyPressedEvent event(key, 0);
			data.EventCallback(event);
		} else if (action == GLFW_RELEASE) {
			Input::UpdateKeyState((KeyCode)key, KeyState::Released);
			KeyReleasedEvent event(key);
			data.EventCallback(event);
		}
		else if (action == GLFW_REPEAT) {
			Input::UpdateKeyState((KeyCode)key, KeyState::Held);
			KeyPressedEvent event(key, 1);
			data.EventCallback(event);
		}
	});
}

void Nut::Window::Shutdown()
{
	glfwDestroyWindow(m_Window);
}
