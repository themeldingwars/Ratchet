#include "Hooks.h"


//static FFFuncDef<&HelpHooked> Help = { 0x1248160 };

namespace Hooks
{
	PLH::CapstoneDisassembler Disambler(PLH::Mode::x86);
	std::map <uint64_t, DetourMapItem> DetourMap;

	void UninstallHook(uint64_t Offset)
	{
		auto detour = DetourMap.find(Offset);
		if (detour != DetourMap.end())
		{
			Logger::Hooks->info("Removed Hook {} at {:#04x}", std::get<0>(detour->second).c_str(), Offset);

			auto detourPtr = std::get<1>(detour->second);
			detourPtr->unHook();
			DetourMap.erase(Offset);
			delete detourPtr;
		}
		else {
			Logger::Hooks->info("Unable to remove Hook {} at {:#04x}", std::get<0>(detour->second).c_str(), Offset);
		}
	}

	void UninstallAllHooks()
	{
		Logger::Hooks->info("Uninstalling all {} installed hooks...", DetourMap.size());
		for (auto const&[key, val] : DetourMap)
		{
			UninstallHook(key);
		}
		Logger::Hooks->info("Hooks uninstalled");
	}
}
