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

	// 日志输出到控制台和文件
	std::vector<spdlog::sink_ptr> sinks = 
	{
		std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Nut.log", true),
		std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
	};

	// 设置日志格式 %^启用颜色 [%T]时间 [%l]日志级别 %n日志名称 %v日志内容 %$禁用颜色
	sinks[0]->set_pattern("[%T] [%l] %n: %v");
	sinks[1]->set_pattern("%^[%T] [%l] %n: %v%$");

	s_Logger = std::make_shared<spdlog::logger>("Nut Log", sinks.begin(), sinks.end());
	s_Logger->set_level(spdlog::level::trace); // 设置日志级别

	// TODO: 读取配置文件设置日志级别
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