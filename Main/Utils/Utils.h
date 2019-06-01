#pragma once

#include <string>
#include <windows.h>

class Utils
{
public:
	Utils();
	~Utils();

	static std::string GetCurrentDir();
	static std::string ReadAllText(std::string& FIlePath);
	static std::string Utils::PrettyPrintBuffer(char* buffer, DWORD bufferLen);
};

