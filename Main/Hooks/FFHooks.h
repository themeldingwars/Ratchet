#pragma once

#include "Hooks.h"

namespace FF
{
	void InstallHooks();

	namespace Console
	{
		int HelpHook(int A1);
		static Hooks::FuncDef Help("Help", 0x1248160, &HelpHook);

		int ReloadUIHook(int A1);
		static Hooks::FuncDef ReloadUI("ReloadUI", 0x7E39D0, &ReloadUIHook);
	}
}