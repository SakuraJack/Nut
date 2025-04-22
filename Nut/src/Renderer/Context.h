#pragma once

struct GLFWwindow;
namespace Nut {

	class GraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow* windowHandle);

		void Init();
		void SwapBuffers();
	private:
		GLFWwindow* m_WindowHandle;
	};
}