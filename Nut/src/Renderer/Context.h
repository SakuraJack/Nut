#pragma once

struct GLFWwindow;
namespace Nut {

	class GraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow* windowHandle);

		void Init();
		void SwapBuffers();
		void SwitchContext();
		void ReleaseContext();
	private:
		GLFWwindow* m_WindowHandle;
	};
}