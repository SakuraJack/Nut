#pragma once

#include "Pipeline.h"

namespace Nut {

	struct RenderPassSpecification
	{
		std::shared_ptr<Pipeline> Pipeline;
	};

	class RenderPass
	{
	public:
		RenderPass(const RenderPassSpecification& spec);
		virtual ~RenderPass();

		virtual RenderPassSpecification& GetSpecification() { return m_Specification; }
		virtual const RenderPassSpecification& GetSpecification() const { return m_Specification; }

	private:
		RenderPassSpecification m_Specification;
	};
}
