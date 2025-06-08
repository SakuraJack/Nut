#pragma once
#include "Window.h"
#include "Nut/Events/Event.h"
#include "Nut/Events/ApplicationEvent.h"
#include "Nut/Events/KeyEvent.h"
#include "Nut/Editor/EditorCamera.h"
#include "Nut/Renderer/VertexArray.h"
#include "Nut/Renderer/VertexBuffer.h"
#include "Nut/Renderer/IndexBuffer.h"
#include "Nut/Renderer/Shader.h"
#include "Nut/Thread/RenderThread.h"
#include "KeyCode.h"
#include "LayerStack.h"
#include "Timestep.h"


namespace Nut {

	struct ApplicationSpecification
	{
		std::string Name = "Nut";
		uint32_t WindowWidth = 1600, WindowHeight = 900;
		bool VSync = true;
		ThreadingPolicy RenderThreadingPolicy = ThreadingPolicy::SingleThreaded;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		~Application();

		void Run();
		void Close();

		virtual void OnInit() {}
		virtual void OnShutdown();
		virtual void OnUpdate(Timestep ts) {}

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; };
		inline Window* GetWindow() { return m_Window.get(); }

		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

		RenderThread& GetRenderThread() { return m_RenderThread; }

	private:
		void ProcessEvents();

	private:
		ApplicationSpecification m_Specification;

		static Application* s_Instance;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;

		Timestep m_Frametime;
		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;

		RenderThread m_RenderThread;

	private:
		bool m_Running = true;
		bool m_Minimized = false;
	
		// ≤‚ ‘∫Ø ˝
	private:
	};

	// TODO: CLient∂®“Â
	Application* CreateApplication(int argc, char** argv);
}

