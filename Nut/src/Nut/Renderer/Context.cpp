#include "ntpch.h"
#include "Context.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Nut {
	GraphicsContext::GraphicsContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
	}

	void GraphicsContext::Init()
	{
		glfwMakeContextCurrent((GLFWwindow*)m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers((GLFWwindow*)m_WindowHandle);
	}

	void GraphicsContext::SwitchContext()
	{
		glfwMakeContextCurrent(m_WindowHandle);
	}

	void GraphicsContext::ReleaseContext()
	{
		glfwMakeContextCurrent(nullptr);
	}

}