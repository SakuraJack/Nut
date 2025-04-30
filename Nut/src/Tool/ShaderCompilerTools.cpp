#include "ntpch.h"
#include "ShaderCompilerTools.h"
#include "Utils/StringUtils.h"

Nut::ShaderCompiler::ShaderCompiler(const std::string& shaderSourcePath)
	: m_ShaderSourcePath(shaderSourcePath)
{

}

Nut::ShaderCompiler::~ShaderCompiler()
{

}

std::unordered_map<GLenum, std::string> Nut::ShaderCompiler::CompileShader(const std::string& shaderSourcePath)
{
	std::shared_ptr<ShaderCompiler> compiler = std::make_shared<ShaderCompiler>(shaderSourcePath);
	compiler->PreprocessShader(shaderSourcePath);	//  预处理着色器
	return compiler->m_ShaderSource;
}

void Nut::ShaderCompiler::PreprocessShader(const std::string& shaderSourcePath)
{
	std::stringstream sourceStream;
	std::string fileString = Nut::Utils::String::ReadFileAndSkipBOM(shaderSourcePath);	//  读取文件并跳过BOM
	Nut::Utils::String::CopyWithoutComments(fileString.begin(), fileString.end(), std::ostream_iterator<char>(sourceStream));
	std::string source = sourceStream.str();

	std::vector<std::pair<GLenum, size_t>> stagePositions;

	size_t startOfStage = 0;
	size_t pos = source.find_first_of('#');

	while (pos != std::string::npos) {
		const size_t endOfLine = source.find("\r\n", pos) + 1;
		std::vector<std::string> tokens = Nut::Utils::String::SplitStringAndKeepDelims(source.substr(pos, endOfLine - pos));
		size_t index = 1;
		if (tokens[1] == "type") {
			++index;

			const std::string_view stage = tokens[index];
			auto shaderStage = ShaderUtils::StageToGLShaderStage(stage);
			startOfStage = pos;
			stagePositions.push_back({ shaderStage, startOfStage });
		}

		pos = source.find_first_of('#', pos + 1);
	}

	for (int index = 0; index < stagePositions.size(); ++index) {
		if (index == stagePositions.size() - 1)	//  最后一个阶段
		{
			auto startPos = stagePositions[index].second;
			std::string subSource = source.substr(startPos);
			unsigned int firstLineEnd = subSource.find_first_of("\r\n");
			std::string resSource = subSource.substr(firstLineEnd + 1);
			resSource = Nut::Utils::String::TrimWhitespace(resSource);
			m_ShaderSource[stagePositions[index].first] = resSource;
		}
		else if (index < stagePositions.size() - 1)
		{
			auto startPos = stagePositions[index].second;
			auto endPos = stagePositions[index + 1].second;
			std::string subSource = source.substr(startPos, endPos - startPos);
			unsigned int firstLineEnd = subSource.find_first_of("\r\n");
			std::string resSource = subSource.substr(firstLineEnd + 1);
			resSource = Nut::Utils::String::TrimWhitespace(resSource);
			m_ShaderSource[stagePositions[index].first] = resSource;
		}
	}
}
