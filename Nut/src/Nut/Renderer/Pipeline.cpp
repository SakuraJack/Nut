#include "ntpch.h"
#include "Pipeline.h"

namespace Nut {

	Pipeline::Pipeline(const PipelineSpecification& spec)
		: m_Specification(spec)
	{
		Invalidate();
	}

	Pipeline::~Pipeline()
	{
	}

	void Pipeline::Invalidate()
	{
	}

}
