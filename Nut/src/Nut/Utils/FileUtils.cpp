#include "ntpch.h"
#include "FileUtils.h"

std::vector<Nut::FileUtils::fs::path> Nut::FileUtils::ListFilesInDirectory(const fs::path& directory)
{
	try
	{
		if (fs::exists(directory) && fs::is_directory(directory)) {
			std::vector<fs::path> files;
			for (const auto& entry : fs::directory_iterator(directory)) {
				if (fs::is_regular_file(entry.status())) {
					files.push_back(entry.path());
				}
			}
			return files;
		}
		else {
			NUT_CORE_ERROR("·�������ڻ���·��: {0}", directory.string());
			return {};
		}
	}
	catch(const fs::filesystem_error& e)
	{
		NUT_CORE_ERROR("��ȡ·�����ļ�����: {0}", e.what());
		return {};
	}
}
