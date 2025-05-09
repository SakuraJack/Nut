#pragma once
#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "KeyCode.h"
#include "LayerStack.h"
#include "Timestep.h"

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Editor/EditorCamera.h"

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
		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		void ProcessEvents();

	private:
		static Application* s_Instance;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;

		Timestep m_Frametime;
		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;

	private:
		bool m_Running = true;
		bool m_Minimized = false;
	
		// ²âÊÔº¯Êý
	private:
	};

	Application* CreateApplication();
}

