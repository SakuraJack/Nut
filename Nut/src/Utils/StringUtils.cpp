#include "ntpch.h"
#include "StringUtils.h"
#include "Core/Log.h"

std::string& Nut::Utils::String::ToLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](const unsigned char ch)
		{
			return std::tolower(ch);
		});
	return str;
}

std::string& Nut::Utils::String::ToUpper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](const unsigned char ch)
		{
			return std::toupper(ch);
		});
	return str;
}

void Nut::Utils::String::Erase(std::string& str, const char* subStr)
{
	size_t pos = str.find(subStr);
	if (pos != std::string::npos)
		str.erase(pos, strlen(subStr));
	else
		NUT_WARN("Substring not found: {0}", subStr);
}

void Nut::Utils::String::Erase(std::string& str, const std::string& subStr)
{
	Erase(str, subStr.c_str());
}

std::string Nut::Utils::String::SubStr(const std::string& str, size_t offset, size_t count /*= std::string::npos*/)
{
	if (offset >= str.size())
	{
		NUT_WARN("Offset is out of range: {0}", offset);
		return str;
	}
	return str.substr(offset, count);
}

std::string Nut::Utils::String::TrimWhitespace(const std::string& str)
{
	const std::string WHITESPACE = "\n\r\t\f\v";
	size_t first = str.find_first_not_of(WHITESPACE);
	std::string trimmed = first == std::string::npos ? "" : str.substr(first);
	size_t last = trimmed.find_last_not_of(WHITESPACE);
	return last == std::string ::npos ? "" : trimmed.substr(0, last + 1);
}

bool Nut::Utils::String::EqualsIgnoreCase(std::string_view str1, std::string_view str2)
{
	if (str1.size() != str2.size()) return false;
	for (size_t i = 0; i < str1.size(); ++i)
	{
		if (std::tolower(str1[i]) != std::tolower(str2[i]))
			return false;
	}
}

unsigned int Nut::Utils::String::CompareCase(std::string_view str1, std::string_view str2)
{
	return strcmp(str1.data(), str2.data());
}

std::vector<std::string> Nut::Utils::String::SplitStringAndKeepDelims(std::string str)
{
	const static std::regex re(R"((^\W|^\w+)|(\w+)|[:()])", std::regex_constants::optimize);

	std::regex_iterator<std::string::iterator> rit(str.begin(), str.end(), re);
	std::regex_iterator<std::string::iterator> rend;
	std::vector<std::string> result;

	while (rit != rend)
	{
		result.emplace_back(rit->str());
		++rit;
	}
	return result;
}

int Nut::Utils::String::skipBOM(std::istream& in)
{
	char test[4] = { 0 };
	in.seekg(0, std::ios::beg);
	in.read(test, 3);
	if (strcmp(test, "\xEF\xBB\xBF") == 0)
	{
		in.seekg(3, std::ios::beg);
		return 3;
	}
	in.seekg(0, std::ios::beg);
	return 0;
}

std::string Nut::Utils::String::ReadFileAndSkipBOM(const std::filesystem::path& path)
{
	std::string res;
	std::ifstream in(path, std::ios::binary | std::ios::in);
	if (in) {
		in.seekg(0, std::ios::end);
		res.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&res[0], res.size());
	}
	in.close();
	return res;
}
