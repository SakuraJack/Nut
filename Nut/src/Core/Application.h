#pragma once
#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "LayerStack.h"

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

namespace Nut {
	class Application
	{
	public:
		Application();
		~Application();

		void Run();
		void OnEvent(Event& e);
		void Close();

		inline static Application& Get() { return *s_Instance; };
		inline Window* GetWindow() { return m_Window.get(); }

		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		static Application* s_Instance;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;

	private:
		bool m_Running = true;
		bool m_Minimized = false;
	
		// ²âÊÔº¯Êý
	private:
		void TestFunction();
		std::shared_ptr<Nut::VertexArray> m_VertexArray;
		std::shared_ptr<Nut::VertexBuffer> m_VertexBuffer;
		std::shared_ptr<Nut::IndexBuffer> m_IndexBuffer;
		unsigned int shaderProgram;
	};

	Application* CreateApplication();
}

