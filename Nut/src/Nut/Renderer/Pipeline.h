#pragma once

#include "Nut/Renderer/Shader.h"
#include "Nut/Renderer/FrameBuffer.h"

namespace Nut {

	enum class PrimitiveTopology
	{
		None = 0,
		Points,
		Lines,
		Triangles,
		LineStrip,
		TriangleStrip,
		TriangleFan,
	};

	enum class DepthCompareOperator
	{
		None = 0,
		Never,
		NotEqual,
		Less,
		LessOrEqual,
		Greater,
		GreaterOrEqual,
		Equal,
		Always
	};

	struct PipelineSpecification
	{
		std::shared_ptr<Shader> Shader;
		std::shared_ptr<Framebuffer> TargetFramebuffer;
		DepthCompareOperator DepthOperator = DepthCompareOperator::GreaterOrEqual;
		bool BackfaceCulling = true;
		bool DepthTest = true;
		bool DepthWrite = true;
		float LineWidth = 1.0f;
	};

	// äÖÈ¾Á÷Ë®Ïß
	class Pipeline
	{
	public:
		Pipeline(const PipelineSpecification& spec);
		virtual ~Pipeline();

		virtual PipelineSpecification& GetSpecification() { return m_Specification; }
		virtual const PipelineSpecification& GetSpecification() const { return m_Specification; }

		virtual void Invalidate();

		virtual std::shared_ptr<Shader> GetShader() const { return m_Specification.Shader; }

	private:
		PipelineSpecification m_Specification;
	};

	class ForwardRenderPipeline : public Pipeline
	{
	};

	class DeferredRenderPipeline : public Pipeline
	{
	};
}