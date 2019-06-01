#include "Logger.h"

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

#include "Main.h"
#include "Config.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace Logger
{
	std::shared_ptr<spdlog::logger> Main;
	std::shared_ptr<spdlog::logger> Hooks;

	std::shared_ptr<spdlog::logger> CreateLogger(const std::string Name, const std::string LogPattern, const std::vector<spdlog::sink_ptr> &Sinks)
	{
		auto logger = std::make_shared<spdlog::logger>(Name, begin(Sinks), end(Sinks));
		logger->set_pattern(LogPattern);
		logger->flush_on(spdlog::level::warn);
		return logger;
	}

	void Init()
	{
		auto logPath = BaseDIR + "/" + "Ratchet.log";

		std::vector<spdlog::sink_ptr> sinks;
		auto fileSink      = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath);
		sinks.push_back(fileSink);

		if (Config::Cfg.ShowConsoleWindow)
		{
			CreateConsole();

			auto console    = make_shared<spdlog::sinks::stdout_color_sink_mt>();
			//auto errConsole = make_shared<spdlog::sinks::stderr_color_sink_mt>();
			sinks.push_back(console);
			//sinks.push_back(errConsole);
		}

		auto logPattern = "%^[%T]%$ %^[%n]%$ %^[%l]%$ %v";
		Main  = CreateLogger("Main", logPattern, sinks);
		Hooks = CreateLogger("Hooks", logPattern, sinks);

		Main->set_pattern(logPattern);
		Main->flush_on(spdlog::level::warn);
		spdlog::set_default_logger(Main);

		spdlog::info(L"🔧 Ratchet Init..");
	}

	void SetupFlush()
	{
		spdlog::flush_every(std::chrono::seconds(5));
	}

	void CreateConsole()
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
	}

	void Flush()
	{
		Main->flush();
		Hooks->flush();
	}
}
