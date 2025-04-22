#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Renderer/Context.h"

namespace Nut
{
	struct WindowProps
	{
		std::string m_Title;
		unsigned int m_Width;
		unsigned int m_Height;

		WindowProps(const std::string& title = "Nut",
			unsigned int width = 1280,
			unsigned int height = 720)
			: m_Title(title), m_Width(width), m_Height(height)
		{
		}
	};

	class Window
	{
	public:
		Window(const WindowProps& props = WindowProps());
		~Window();

		void OnUpdate();

		inline unsigned int GetWidth() const { return m_Data.m_Width; }
		inline unsigned int GetHeight() const { return m_Data.m_Height; }

		void SetVSync(bool enabled);
		inline bool IsVSync() const { return m_Data.m_VSync; }

		static Window* Create(const WindowProps& props = WindowProps());

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string m_Title;
			unsigned int m_Width, m_Height;
			bool m_VSync;
		};
		WindowData m_Data;
	};
}