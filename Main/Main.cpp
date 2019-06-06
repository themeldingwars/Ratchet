/*
	Ratchet
	A client modification framework and tool
	- Arkii
*/

#include "Main.h"
#include <thread>
#include <filesystem>
#include <sstream>
#include <string>
#include <experimental/filesystem>

#include "../VersionProxy/VersionProxy.h"
#include "../Main/Utils/Utils.h"
#include "Logger.h"
#include "Config.h"
#include "Hooks/FFHooks.h"
#include "CLR/CLR.h"

std::thread InjectedThread;
bool AfterLoadInitHasRun = false;
std::string BaseDIR      = Utils::GetCurrentDir() + "/" + TITLE;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		return ProcessAttach(hModule);
	case DLL_PROCESS_DETACH:
		return ProcessDetach(hModule);
		break;
	}
	return TRUE;
}

BOOL ProcessAttach(HMODULE hModule)
{
	Config::Load();
	if (!VersionProxyInit() || !IsValidTarget()) { return false; }

	DisableThreadLibraryCalls(hModule);

	Start();

	return true;
}

void Start()
{
	Logger::Init();
	FF::InstallHooks();
	CLR::Init();

	// Create dirs
	std::experimental::filesystem::create_directories(BaseDIR);

	if (Config::Debug.ShowMessageBoxOnLoad)
	{
		MessageBox(NULL, "Injected :>", "Ratchet", MB_OK);
	}
}

// Work around the dll main loader lock and some things that need to do things that are forbidden there, eg thread spawning
void AfterLoadInit()
{
	if (!VersionProxyInit() || !IsValidTarget()) { return; }

	if (!AfterLoadInitHasRun)
	{
		AfterLoadInitHasRun = true;

		//InjectedThread = std::thread(ThreadRun);
		Logger::SetupFlush();
	}
}

void ThreadRun()
{
	MessageBox(NULL, "In thread", "Ratchet", MB_OK);

	while (true)
	{

		Sleep(2);
	}
}

BOOL ProcessDetach(HMODULE hModule)
{
	// Freeing the original version.dll module
	if (hVersion)
		FreeLibrary(hVersion);

	CLR::DeInit();
	Hooks::UninstallAllHooks();
	Logger::Flush();

	return true;
}

BOOL IsValidTarget()
{
	// Getting the current module name and comparing it to our defined TARGET_EXE_NAME
	TCHAR buffer[MAX_PATH] = { 0 };
	DWORD bufSize = sizeof(buffer) / sizeof(*buffer);

	GetModuleFileName(NULL, buffer, bufSize);

	string name = PathFindFileName(buffer);
	transform(name.begin(), name.end(), name.begin(), tolower);

	string temp = Config::Cfg.ExeName;
	transform(temp.begin(), temp.end(), temp.begin(), tolower);

	return (name == temp);
}