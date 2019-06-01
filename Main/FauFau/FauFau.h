#pragma once

//=========================================================
// Functions to interop with the Firefall game client
// Builds on top of the raw hooks to provide a simpler API
//=========================================================

#include <string>

namespace FAUFAU
{
	void SetTitle(std::string& Title);
	void AppendToTitle(std::string& Title);
}