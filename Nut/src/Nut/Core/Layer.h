#pragma once

#include "Nut/Events/Event.h"
#include "Timestep.h"

namespace Nut {
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(Timestep ts) {};
		virtual void OnEvent(Event& event) {};

		inline const std::string& GetName() const { return m_LayerName; }

	private:
		std::string m_LayerName;
	};
}
