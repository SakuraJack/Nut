#include "Nut/EntryPoint.h"

class NutEditorApplication : public Nut::Application
{
public:
	NutEditorApplication(const Nut::ApplicationSpecification& spec, std::string_view projectPath)
		: Application(spec), m_ProjectPath(projectPath)
	{
		if (projectPath.empty())
		{
			m_ProjectPath = "NutProjects/ExampleProject.nproj";
		}
	}

	virtual void OnInit() override
	{
		// TODO: Load project from m_ProjectPath
		// PushLayer(new Nut::EditorLayer());
	}

private:
	std::string m_ProjectPath;
};

Nut::Application* Nut::CreateApplication(int argc, char** argv)
{
	std::string_view projectPath;
	if (argc > 1)
		projectPath = argv[1];

	Nut::ApplicationSpecification spec;
	spec.Name = "Nut Editor";
	spec.WindowWidth = 1600;
	spec.WindowHeight = 900;
	spec.VSync = true;

	spec.RenderThreadingPolicy = Nut::ThreadingPolicy::SingleThreaded;

	return new NutEditorApplication(spec, projectPath);
}