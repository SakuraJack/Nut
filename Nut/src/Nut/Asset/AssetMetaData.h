#pragma once

#include "Asset.h"
#include <filesystem>

namespace Nut {

	struct AssetMetaData
	{
		AssetHandle Handle = 0;
		AssetType Type;

		std::filesystem::path FilePath;
		bool IsDataLoaded = false;
		bool IsMemoryOnly = false;

		bool IsValid() const { return !Handle && !IsMemoryOnly; }
	};
}