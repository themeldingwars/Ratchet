#pragma once

#include "headers/CapstoneDisassembler.hpp"
#include "headers/Detour/x86Detour.hpp"

#pragma comment(lib, "Lib/polyhook/PolyHook_2.lib")
#pragma comment(lib, "Lib/polyhook/capstone/capstone.lib")

#include <iostream>
#include <cstdarg>

#include "../Main.h"
#include "../Utils/Utils.h"
#include "../Logger.h"
#include "../Utils/MemEd.h"
#include <spdlog/spdlog.h>

namespace Hooks
{
	template<typename T>
	struct FuncDef
	{
		FuncDef::FuncDef(std::string _Name, uint64_t _Offset, T HookedHandlerPtr)
		{
			Name        = _Name;
			Offset      = _Offset;
			Trampoline  = 0;
			HookHandler = (uint64_t)HookedHandlerPtr;
		};

		std::string Name;
		uint64_t Offset;
		T Trampoline;
		uint64_t HookHandler;
	};

	typedef std::tuple<std::string, PLH::x86Detour*> DetourMapItem;

	extern PLH::CapstoneDisassembler Disambler;
	extern std::map<uint64_t, DetourMapItem> DetourMap;

	template<typename T>
	void InstallHook(FuncDef<T> &Hook)
	{
		Hook.Trampoline = (T)InstallSimpleHook(Hook.Offset, Hook.Name.c_str(), Hook.HookHandler);
	}

	uint64_t InstallSimpleHook(uint64_t Offset, const char* Name, uint64_t HookHandler);

	void UninstallHook(uint64_t Offset);

	template<typename T>
	void UninstallHook(FuncDef<T> &Hook)
	{
		UninstallHook(Hook.Offset);
	}

	void UninstallAllHooks();
}

