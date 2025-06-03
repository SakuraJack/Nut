#include "ntpch.h"
#include "Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Nut/Editor/EditorConsole/EditorConsoleSink.h"

#define NT_HAS_CONSOLE !NT_DIST

namespace Nut {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Log::s_EditorConsoleLogger;
}

void Nut::Log::Init()
{
	std::string logDirectory = "logs";
	if (!std::filesystem::exists(logDirectory))
		std::filesystem::create_directory(logDirectory);

	// 日志输出到控制台和文件
	std::vector<spdlog::sink_ptr> nutSinks = 
	{
		std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Nut.log", true),
#if NT_HAS_CONSOLE
		std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
#endif
	};

	std::vector<spdlog::sink_ptr> appSinks =
	{
		std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/APP.log", true),
#if NT_HAS_CONSOLE
		std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
#endif
	};

	std::vector<spdlog::sink_ptr> eidtorConsoleSinks =
	{
		std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/APP.log", true),
#if NT_HAS_CONSOLE
		std::make_shared<EditorConsoleSink>(1)
#endif
	};

	nutSinks[0]->set_pattern("[%T] [%l] %n: %v"); // 日志格式
	appSinks[0]->set_pattern("[%T] [%l] %n: %v");

#if NT_HAS_CONSOLE
	nutSinks[1]->set_pattern("%^[%T] [%l] %n: %v%$"); // 控制台日志格式
	appSinks[1]->set_pattern("%^[%T] [%l] %n: %v%$");
	for (auto sink : eidtorConsoleSinks)
		sink->set_pattern("%^%v%$");
#endif

	s_CoreLogger = std::make_shared<spdlog::logger>("Nut", nutSinks.begin(), nutSinks.end());
	s_CoreLogger->set_level(spdlog::level::trace); // 设置日志级别

	s_ClientLogger = std::make_shared<spdlog::logger>("App", appSinks.begin(), appSinks.end());
	s_ClientLogger->set_level(spdlog::level::trace); // 设置日志级别

	s_EditorConsoleLogger = std::make_shared<spdlog::logger>("Console", eidtorConsoleSinks.begin(), eidtorConsoleSinks.end());
	s_EditorConsoleLogger->set_level(spdlog::level::trace); // 设置日志级别

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

void Nut::Log::Shutdown()
{
	s_CoreLogger.reset();
	s_ClientLogger.reset();
	s_EditorConsoleLogger.reset();
	spdlog::drop_all();
	spdlog::shutdown();
}