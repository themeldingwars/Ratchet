#pragma once

#include <memory>
namespace spdlog
{
	class logger;
}

namespace Logger
{
	void Init();
	void SetupFlush();
	void CreateConsole();
	void Flush();

	extern std::shared_ptr<spdlog::logger> Main;
	extern std::shared_ptr<spdlog::logger> Hooks;
	extern std::shared_ptr<spdlog::logger> CLR;
	extern std::shared_ptr<spdlog::logger> DotNet;
}

