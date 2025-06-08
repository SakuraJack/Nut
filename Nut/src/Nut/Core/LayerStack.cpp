#include "ntpch.h"
#include "LayerStack.h"

Nut::LayerStack::LayerStack()
{
}

Nut::LayerStack::~LayerStack()
{
}

void Nut::LayerStack::PushLayer(Layer* layer)
{
	m_Layers.insert(m_Layers.begin() + m_LayerInsertIndex, layer);
	m_LayerInsertIndex++;
}

void Nut::LayerStack::PopLayer(Layer* layer)
{
	auto iter = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (iter != m_Layers.end())
	{
		m_Layers.erase(iter);
		m_LayerInsertIndex--;
	}
}

void Nut::LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

void Nut::LayerStack::PopOverlay(Layer* overlay)
{
	auto iter = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (iter != m_Layers.end())
	{
		m_Layers.erase(iter);
	}
}