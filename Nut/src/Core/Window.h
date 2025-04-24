#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Renderer/Context.h"
#include "Events/Event.h"

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
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProps& props = WindowProps());
		~Window();

		void OnUpdate();

		inline unsigned int GetWidth() const { return m_Data.m_Width; }
		inline unsigned int GetHeight() const { return m_Data.m_Height; }

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

		void SetVSync(bool enabled);
		inline bool IsVSync() const { return m_Data.m_VSync; }

		static Window* Create(const WindowProps& props = WindowProps());

		inline GLFWwindow* GetNativeWindow() const { return m_Window; } //返回窗口指针

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		std::unique_ptr<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string m_Title; //窗口标题
			unsigned int m_Width, m_Height, m_XPos, m_YPos; //窗口大小 窗口位置
			bool m_VSync;	//窗口是否开启垂直同步
			EventCallbackFn EventCallback; //事件回调函数
		};
		WindowData m_Data;
	};
}