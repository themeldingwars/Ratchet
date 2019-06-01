#pragma once

#include <stdint.h>
#include <windows.h>

namespace MemEd
{
	uint64_t GetRelPtr(const uint64_t offset);
	void Write(void *Address, char* Data, size_t DataLen);
	void Write(DWORD Address, char* Data, size_t DataLen);
	void FillSection(PVOID Address, int Val, size_t Bytes);
	void NopSection(PVOID Address, size_t Bytes);
	void NopSection(DWORD Address, size_t Bytes);
}

