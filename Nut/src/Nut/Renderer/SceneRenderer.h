#pragma once

#include "Nut/Scene/Scene.h"
#include "Nut/Renderer/Camera.h"
#include "Nut/Renderer/Mesh.h"
#include "Nut/Renderer/Pipeline.h"
#include "Nut/Renderer/RenderPass.h"
#include "Nut/Renderer/Material.h"

namespace Nut {

	struct SceneRendererCamera
	{
		Nut::Camera Camera;
		glm::mat4 ViewMatrix;
		float Near, Far;
		float FOV;
	};

	struct SceneRendererSpecification
	{
		uint32_t ViewportWidth = 0, ViewportHeight = 0;
	};

	class SceneRenderer
	{
	public:
		struct Statistics
		{
			uint32_t DrawwCalls = 0;
			uint32_t Meshes = 0;
			uint32_t Instances = 0;

			float TotalGPUTime = 0.0f;
		};

	public:
		SceneRenderer(std::shared_ptr<Scene> scene);
		virtual ~SceneRenderer();

		void Init();

		void Shutdown();

		void InitOptions();

		void SetViewportSize(uint32_t width, uint32_t height);

		void BeginScene(const SceneRendererCamera& camera);
		void EndScene();

		void SubmitMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<MeshSource> meshSource, uint32_t submeshIndex, std::shared_ptr<MaterialTable> materialTable, const glm::mat4& transform = glm::mat4(1.0f));
		void SubmitStaticMesh(std::shared_ptr<StaticMesh> staticMesh, std::shared_ptr<MeshSource> meshSource, std::shared_ptr<MaterialTable> materialTable, const glm::mat4& transform = glm::mat4(1.0f));

	private:
		struct MeshKey
		{
			AssetHandle MeshHandle;
			AssetHandle MaterialHandle;
			uint32_t SubmeshIndex;
			bool IsSelected;

			MeshKey(AssetHandle meshHandle, AssetHandle materialHandle, uint32_t submeshIndex, bool isSelected)
				: MeshHandle(meshHandle), MaterialHandle(materialHandle), SubmeshIndex(submeshIndex), IsSelected(isSelected)
			{
			}

			bool operator<(const MeshKey& other) const
			{
				if (MeshHandle < other.MeshHandle) return true;
				if (MeshHandle > other.MeshHandle) return false;
				if (SubmeshIndex < other.SubmeshIndex) return true;
				if (SubmeshIndex > other.SubmeshIndex) return false;
				if (MaterialHandle < other.MaterialHandle) return true;
				if (MaterialHandle > other.MaterialHandle) return false;
				return IsSelected < other.IsSelected;
			}
		};

	private:
		std::shared_ptr<Scene> m_Scene;
		SceneRendererSpecification m_Specification;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		struct SceneInfo
		{
			SceneRendererCamera SceneCamera;

			DirLight ActiveLight;
		} m_SceneData;

		std::shared_ptr<Pipeline> m_SkyboxPipeline;
		std::shared_ptr<Material> m_SkyboxMaterial;
		std::shared_ptr<RenderPass> m_SkyboxPass;
	};
}