#include "Memed.h"
#include <Windows.h>

#include "../Logger.h"
#include "../Config.h"

namespace MemEd
{
	namespace OpCodes
	{
		const unsigned char NOP = 0x90;
	}

	uint64_t GetRelPtr(const uint64_t Offset)
	{
		return (Offset - Config::Cfg.ModuleBaseAddr) + (uint64_t)GetModuleHandle(NULL);
	}

	// Write data to the given memory address
	void Write(void* Address, char* Data, size_t DataLen)
	{
		DWORD d, ds;
		VirtualProtect(Address, DataLen, PAGE_EXECUTE_READWRITE, &d);
		memcpy(Address, Data, DataLen);
		VirtualProtect(Address, DataLen, d, &ds);
	}

	void Write(DWORD Address, char* Data, size_t DataLen)
	{
		Write((void*)Address, Data, DataLen);
	}

	// Set a section of memory to the given value
	void FillSection(void* Address, int Val, size_t Bytes)
	{
		DWORD d, ds;
		VirtualProtect(Address, Bytes, PAGE_EXECUTE_READWRITE, &d);
		memset(Address, Val, Bytes);
		VirtualProtect(Address, Bytes, d, &ds);
	}

	// Replace a section of memory with NOPs
	void NopSection(void* Address, size_t Bytes)
	{
		FillSection(Address, OpCodes::NOP, Bytes);
	}

	void NopSection(DWORD Address, size_t Bytes)
	{
		NopSection((void*)Address, Bytes);
	}
}
