#pragma once
#include "../Lib/json/json.hpp"

struct ConfigDebug
{
	bool ShowMessageBoxOnLoad;
};

struct ConfigCfg
{
	bool ShowConsoleWindow;
	std::string ExeName;
	unsigned int ModuleBaseAddr;
};

class Config
{
public:
	Config();
	~Config();

	static nlohmann::json ConfigJson;

	static ConfigDebug Debug;
	static ConfigCfg Cfg;

	static void Load();
};

