#include "Utils.h"

#include "../Main.h"
#include <Windows.h>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <string>

using std::string;

Utils::Utils()
{

}

std::string Utils::GetCurrentDir()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}

std::string Utils::GetRatchetDir()
{
	auto dir = Utils::GetCurrentDir() + "\\" + TITLE + "\\";
	return dir;
}

std::string Utils::ReadAllText(std::string& FilePath)
{
	std::ifstream fs(FilePath);
	std::string str((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
	return str;
}

// Prints out a nice hex view of a byte buffer
std::string Utils::PrettyPrintBuffer(char* buffer, DWORD bufferLen)
{
	std::stringstream ss;
	ss << "-------------------------------------------------------------------------------------" << std::endl;
	ss << "| 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | 0 1 2 3 4 5 6 7 8 9 A B C D E F |" << std::endl;
	ss << "-------------------------------------------------------------------------------------" << std::endl;
	DWORD offset = 0;
	while (offset < bufferLen)
	{
		ss << "  ";

		int tempOffset = 0;
		for (int i = 0; i < 0x10; i++)
		{
			if ((offset + i) >= bufferLen)
			{
				ss << "   ";
			}
			else
			{
				BYTE byte = buffer[offset + tempOffset++];
				char outBuff[6];
				sprintf(outBuff, "%02x ", byte);
				ss << outBuff;
			}
		}

		ss << "| ";

		tempOffset = 0;
		for (int i = 0; i < 0x10; i++)
		{
			if ((offset + i) >= bufferLen)
			{
				ss << "  ";
			}
			else
			{
				BYTE byte = buffer[offset + tempOffset++];
				char outBuff[1];
				if (byte >= 0x20 && byte < 0x7F)
					sprintf(outBuff, "%c ", static_cast<char>(byte));
				else
					sprintf(outBuff, "%s ", ".");
				ss << outBuff;
			}
		}

		offset += tempOffset;
		ss << std::endl;
	}

	return ss.str();
}

Utils::~Utils()
{

}
