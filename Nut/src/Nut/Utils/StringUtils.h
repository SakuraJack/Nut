#pragma once

namespace Nut::Utils
{
	namespace String {
		std::string& ToLower(std::string& str);	// תСд
		std::string& ToUpper(std::string& str); // ת��д
		void Erase(std::string& str, const char* subStr);
		void Erase(std::string& str, const std::string& subStr); 
		std::string SubStr(const std::string& str, size_t offset, size_t count = std::string::npos);
		std::string TrimWhitespace(const std::string& str);	// ȥ����λ�հ��ַ�
		bool EqualsIgnoreCase(std::string_view str1, std::string_view str2);
		unsigned int CompareCase(std::string_view str1, std::string_view str2);
		std::vector<std::string> SplitStringAndKeepDelims(std::string str); // �ָ��ַ����������ָ���
		int skipBOM(std::istream& in); // ����BOM ��Byte Order Mark��
		std::string ReadFileAndSkipBOM(const std::filesystem::path& path);

		// From https://wandbox.org/permlink/iXC7DWaU8Tk8jrf3 and is modified.
		// �Ƴ�ע�ͣ����ģ�
		enum class State : char { SlashOC, StarIC, SingleLineComment, MultiLineComment, NotAComment };
		template<typename InputIt, typename OutputIt>
		void CopyWithoutComments(InputIt first, InputIt last, OutputIt out)
		{
			State state = State::NotAComment;

			while (first != last)
			{
				switch (state)
				{
				case State::SlashOC:
					if (*first == '/') state = State::SingleLineComment;
					else if (*first == '*') state = State::MultiLineComment;
					else
					{
						state = State::NotAComment;
						*out++ = '/';
						*out++ = *first;
					}
					break;
				case State::StarIC:
					if (*first == '/') state = State::NotAComment;
					else state = State::MultiLineComment;
					break;
				case State::SingleLineComment:
					if (*first == '\n')
					{
						state = State::NotAComment;
						*out++ = '\n';
					}
					break;
				case State::MultiLineComment:
					if (*first == '*') state = State::StarIC;
					else if (*first == '\n') *out++ = '\n';
					break;
				case State::NotAComment:
					if (*first == '/') state = State::SlashOC;
					else *out++ = *first;
					break;
				}
				++first;
			}
		}
	}
}