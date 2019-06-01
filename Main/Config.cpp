#include "Config.h"
#include "Main.h"
#include "Utils/Utils.h"

using nlohmann::json;

nlohmann::json	Config::ConfigJson;
ConfigDebug		Config::Debug;
ConfigCfg		Config::Cfg;

Config::Config()
{
}


Config::~Config()
{
}

void Config::Load()
{
	auto path    = BaseDIR + "/Config.json";
	auto jsonStr = Utils::ReadAllText(path);
	ConfigJson   = json::parse(jsonStr);

	// Debug
	//----------------------------------
	auto debugName = "Debug";
	Debug.ShowMessageBoxOnLoad = ConfigJson[debugName]["ShowMessageBoxOnLoad"].get<bool>();

	// Main
	//----------------------------------
	Cfg.ShowConsoleWindow = ConfigJson["ShowConsoleWindow"].get<bool>();
	Cfg.ExeName           = ConfigJson["ExeName"].get<std::string> ();
	Cfg.ModuleBaseAddr    = ConfigJson["ModuleBaseAddr"].get<unsigned int>();
}
