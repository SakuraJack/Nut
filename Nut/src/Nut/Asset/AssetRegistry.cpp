#include "ntpch.h"
#include "AssetRegistry.h"


namespace Nut {
	std::mutex s_AssetRegistryMutex;
}

Nut::AssetMetaData& Nut::AssetRegistry::operator[](const AssetHandle handle)
{
	std::scoped_lock<std::mutex> lock(s_AssetRegistryMutex);
	return m_AssetRegistry[handle];
}

Nut::AssetMetaData& Nut::AssetRegistry::Get(const AssetHandle handle)
{
	std::scoped_lock<std::mutex> lock(s_AssetRegistryMutex);
	return m_AssetRegistry.at(handle);
}

const Nut::AssetMetaData& Nut::AssetRegistry::Get(const AssetHandle handle) const
{
	std::scoped_lock<std::mutex> lock(s_AssetRegistryMutex);
	return m_AssetRegistry.at(handle);
}

bool Nut::AssetRegistry::Contains(const AssetHandle handle) const
{
	std::scoped_lock<std::mutex> lock(s_AssetRegistryMutex);
	return m_AssetRegistry.find(handle) != m_AssetRegistry.end();
}

size_t Nut::AssetRegistry::Remove(const AssetHandle handle)
{
	std::scoped_lock<std::mutex> lock(s_AssetRegistryMutex);
	return m_AssetRegistry.erase(handle);
}

void Nut::AssetRegistry::Clear()
{
	std::scoped_lock<std::mutex> lock(s_AssetRegistryMutex);
	m_AssetRegistry.clear();
}
