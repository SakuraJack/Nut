#pragma once

namespace Nut {

	namespace FileUtils {
		namespace fs = std::filesystem;

		std::vector<fs::path> ListFilesInDirectory(const fs::path& directory);
	}
}
