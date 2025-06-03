#pragma once

#include "glm/glm.hpp"

#include "Nut/Core/UUID.h"
#include "Nut/Core/Timestep.h"
#include "Nut/Asset/Asset.h"
#include "Nut/Events/Event.h"
#include "Entity.h"

namespace Nut {
	using EntityMap = std::unordered_map<UUID, Entity>;

	struct DirLight
	{
		glm::vec3 Direction = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };
		float Intensity = 1.0f;
		float ShowAmount = 1.0f;
		bool CastShadows = true;
	};

	struct PointLight
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Radiance = { 0.0f, 0.0f, 0.0f };
		float Intensity = 0.0f;
		float Radius = 25.f;
		float MinRadius = 0.1f;
		float Falloff = 1.0f;
		float SourceSize = 0.1f;
		bool CastShadows = true;
		char Padding[3] = { 0, 0, 0 };


	};

	class Scene : public Asset
	{
	public:
		Scene(const std::string& name = "DefaultScene", bool isEditorScene = false, bool initalize = true);
		~Scene();

		void Init();

		void OnUpdateRuntime(Timestep ts); // ÔËÐÐ
		void OnUpdateEditor(Timestep ts); // ±à¼­Æ÷

		// void OnRenderRuntime(std::shared_ptr<SceneRenderer>, Timestep ts);
		// void OnRenderEditor(std::shared_ptr<SceneRenderer>, Timestep ts);

		void OnEvent(Event& e);
	};
}