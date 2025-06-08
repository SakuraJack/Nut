#include "ntpch.h"
#include "SceneRenderer.h"
#include "Nut/Core/Application.h"

namespace Nut {

	SceneRenderer::SceneRenderer(std::shared_ptr<Scene> scene)
		: m_Scene(scene)
	{
		Init();
	}

	SceneRenderer::~SceneRenderer()
	{
		Shutdown();
	}

	void SceneRenderer::Init()
	{
		m_ViewportWidth = m_Specification.ViewportWidth;
		m_ViewportHeight = m_Specification.ViewportHeight;

		// Geometry
		{
			ImageSpecification imageSpec;
			imageSpec.Width = Application::Get().GetWindow()->GetWidth();
			imageSpec.Height = Application::Get().GetWindow()->GetHeight();
			imageSpec.Format = ImageFormat::RGBA32F;
			imageSpec.Usage = ImageUsage::Attachment;
		}
	}
}