#pragma once

#include "Nut.h"

#include "Nut/Editor/EditorCamera.h"
#include "Nut/Asset/AssetMetaData.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Nut {

	class NutEditorLayer : public Layer
	{
		NutEditorLayer();
		virtual ~NutEditorLayer() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;

		virtual void OnEvent(Event& e) override;

		void OnRender2D();

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MousePressedEvent& e);

		void OpenProject();
		void OpenProject(const std::filesystem::path& filepath);
		void CreateProject(std::filesystem::path projectPath);
		void EmptyProject();
		void UpdateCurrentProject();
		void SaveProject();
		void CloseProject(bool unloadProject = true);
		void NewScene(const std::string& name = "UntitledScene");
		bool OpenScene();
		bool OpenScene(const std::filesystem::path& filepath, const bool checkAutoSave);
		bool OpenScene(const AssetMetaData& assetMetaData);
	};
}