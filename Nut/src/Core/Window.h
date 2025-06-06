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
		uint32_t m_Width;
		uint32_t m_Height;

		WindowProps(const std::string& title = "Nut",
			uint32_t width = 1280,
			uint32_t height = 720)
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

		void ProcessEvents();
		void SwapBuffers();

		inline uint32_t GetWidth() const { return m_Data.m_Width; }
		inline uint32_t GetHeight() const { return m_Data.m_Height; }

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

		void SetVSync(bool enabled);
		inline bool IsVSync() const { return m_Data.m_VSync; }

		static Window* Create(const WindowProps& props = WindowProps());

		inline GLFWwindow* GetNativeWindow() const { return m_Window; } //返回窗口指针
		inline std::shared_ptr<GraphicsContext> GetRenderContext() const { return m_Context; } //返回上下文指针

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		std::shared_ptr<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string m_Title; //窗口标题
			uint32_t m_Width, m_Height, m_XPos, m_YPos; //窗口大小 窗口位置
			bool m_VSync;	//窗口是否开启垂直同步
			EventCallbackFn EventCallback; //事件回调函数
		};
		WindowData m_Data;
	};
}