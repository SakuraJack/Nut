#include "ntpch.h"
#include "Project.h"

namespace Nut {
	
	Project::Project()
	{
	}

	Project::~Project()
	{
	}

	void Project::SetActive(std::shared_ptr<Project> project)
	{
		if (s_ActiveProject)
		{
			//s_AssetManager->Shutdown();
			s_AssetManager = nullptr;
		}

		s_ActiveProject = project;
		if (s_ActiveProject)
		{
			s_AssetManager = std::make_shared<EditorAssetManager>();
		}
	}
}
