#include "../Logger.h"
#include <spdlog/spdlog.h>

#include "FFHooks.h"
#include "../CLR/CLR.h"

// Split off in to files as need be
//===============================================

void FF::InstallHooks()
{
	Hooks::InstallHook(FF::Console::Help);
	//Hooks::InstallHook(FF::Console::ReloadUI);
}

int FF::Console::HelpHook(int A1)
{
	printf("Console help function :>\n");
	auto ret = FF::Console::Help.Trampoline(A1);
	CLR::RunInteropBenchmark();
	return ret;
}

int FF::Console::ReloadUIHook(int A1)
{
	printf("Console ReloadUI function :>\n");
	auto ret = FF::Console::ReloadUI.Trampoline(A1);
	return ret;
}