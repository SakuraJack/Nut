#pragma once

#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

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
		//inline Window& GetWindow() { return *m_Window; }

		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		static Application* s_Instance;
		std::unique_ptr<Window> m_Window;

	private:
		bool m_Running = true;
		bool m_Minimized = false;
	};

	Application* CreateApplication();
}

