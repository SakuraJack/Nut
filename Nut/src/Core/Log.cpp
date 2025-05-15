#include "ntpch.h"
#include "Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Nut {
	std::shared_ptr<spdlog::logger> Log::s_Logger;
}

void Nut::Log::Init()
{
	std::string logDirectory = "logs";
	if (!std::filesystem::exists(logDirectory))
		std::filesystem::create_directory(logDirectory);

	// ��־���������̨���ļ�
	std::vector<spdlog::sink_ptr> sinks = 
	{
		std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Nut.log", true),
		std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
	};

	// ������־��ʽ %^������ɫ [%T]ʱ�� [%l]��־���� %n��־���� %v��־���� %$������ɫ
	sinks[0]->set_pattern("[%T] [%l] %n: %v");
	sinks[1]->set_pattern("%^[%T] [%l] %n: %v%$");

	s_Logger = std::make_shared<spdlog::logger>("Nut Log", sinks.begin(), sinks.end());
	s_Logger->set_level(spdlog::level::trace); // ������־����

	// TODO: ��ȡ�����ļ�������־����
	s_EnabledTags["Window"] = { true, Level::Trace };
	s_EnabledTags["Renderer"] = { true, Level::Trace };
	s_EnabledTags["OpenGL"] = { true, Level::Warn };
	s_EnabledTags["Shader"] = { true, Level::Trace };
	s_EnabledTags["Texture"] = { true, Level::Trace };
	s_EnabledTags["VertexArray"] = { true, Level::Trace };
	s_EnabledTags["Memory"] = { true, Level::Trace };
	s_EnabledTags["Timer"] = { true, Level::Trace };
}

void Nut::Log::ShutDown()
{
	s_Logger.reset();
	spdlog::drop_all();
	spdlog::shutdown();
}