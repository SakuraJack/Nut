#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Nut {
	class Log
	{
	public:
		enum class Level {
			Trace = 0, Info, Warn, Error, Critical
		};

		struct TagDetails
		{
			bool Enabled = true;
			Level LevelFilter = Level::Trace;
		};

		static void Init();
		static void ShutDown();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

		static std::unordered_map<std::string, TagDetails>& GetEnabledTags() { return s_EnabledTags; }

		template<typename... Args>
		static void PrintMessage(Log::Level level, std::string_view tag, fmt::format_string<Args...> fmt, Args&&... args);

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
		inline static std::unordered_map<std::string, TagDetails> s_EnabledTags; // 启用标签 目前只会在代码里固定一些标签 将来可能会从配置文件里读取

	public:
		static const char* LevelToString(Level level)
		{
			switch (level)
			{
			case Level::Trace:	return "Trace";
			case Level::Info:	return "Info";
			case Level::Warn:	return "Warn";
			case Level::Error:	return "Error";
			case Level::Critical:	return "Critical";
			}

			return "";
		}

		static Level LevelFromString(std::string_view string)
		{
			if (string == "Trace")	return Level::Trace;
			if (string == "Info")	return Level::Info;
			if (string == "Warn")	return Level::Warn;
			if (string == "Error")	return Level::Error;
			if (string == "Critical")	return Level::Critical;

			return Level::Trace;
		}
	};
}

#define NUT_TRACE(...) ::Nut::Log::PrintMessage(::Nut::Log::Level::Trace, "", __VA_ARGS__)
#define NUT_INFO(...) ::Nut::Log::PrintMessage(::Nut::Log::Level::Info, "", __VA_ARGS__)
#define NUT_WARN(...) ::Nut::Log::PrintMessage(::Nut::Log::Level::Warn, "", __VA_ARGS__)
#define NUT_ERROR(...) ::Nut::Log::PrintMessage(::Nut::Log::Level::Error, "", __VA_ARGS__)
#define NUT_CRITICAL(...) ::Nut::Log::PrintMessage(::Nut::Log::Level::Critical, "", __VA_ARGS__)

#define NUT_TRACE_TAG(tag, ...) ::Nut::Log::PrintMessage(::Nut::Log::Level::Trace, tag, __VA_ARGS__)
#define NUT_INFO_TAG(tag, ...) ::Nut::Log::PrintMessage(::Nut::Log::Level::Info, tag, __VA_ARGS__)
#define NUT_WARN_TAG(tag, ...) ::Nut::Log::PrintMessage(::Nut::Log::Level::Warn, tag, __VA_ARGS__)
#define NUT_ERROR_TAG(tag, ...) ::Nut::Log::PrintMessage(::Nut::Log::Level::Error, tag, __VA_ARGS__)
#define NUT_CRITICAL_TAG(tag, ...) ::Nut::Log::PrintMessage(::Nut::Log::Level::Critical, tag, __VA_ARGS__)

namespace Nut {
	template<typename... Args>
	void Log::PrintMessage(Log::Level level, std::string_view tag, fmt::format_string<Args...> fmt, Args&&... args)
	{
		auto detail = s_EnabledTags[std::string(tag)];
		if (detail.Enabled && detail.LevelFilter <= level)
		{
			auto logger = GetLogger();
			std::string tagStr = tag.empty() ? std::string(tag) : ("[" + std::string(tag) + "]");
			switch (level)
			{
			case Level::Trace:
				logger->trace("{0}{1}", tagStr, fmt::format(fmt, std::forward<Args>(args)...));
				break;
			case Level::Info:
				logger->info("{0}{1}", tagStr, fmt::format(fmt, std::forward<Args>(args)...));
				break;
			case Level::Warn:
				logger->warn("{0}{1}", tagStr, fmt::format(fmt, std::forward<Args>(args)...));
				break;
			case Level::Error:
				logger->error("{0}{1}", tagStr, fmt::format(fmt, std::forward<Args>(args)...));
				break;
			case Level::Critical:
				logger->critical("{0}{1}", tagStr, fmt::format(fmt, std::forward<Args>(args)...));
				break;
			}
		}
	}
}