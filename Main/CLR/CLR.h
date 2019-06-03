#pragma once

#include <string>
#include "coreclrhost.h"

namespace CLR
{
	enum class LogLevels : unsigned char
	{
		Trace,
		Info,
		Debug,
		Warn,
		Error
	};

	typedef void(*VoidFunc)();
	typedef void(*ExposeFuncPtrs)(int Num, const char** Names, unsigned int* FuncPtrs);

	extern std::string NetCoreDir;
	extern std::string RatchetSharpDir;

	void Init();
	void DeInit();
	void BindDelegates();
	void SetExportedFunctions();
	void** CreateRatchetDelegate(std::string Function, void** DelegatePtr, std::string ClassName);
	void ExposeFunction(std::string Name, unsigned int Ptr);
	void SendExposedFuncPtrsToManaged();

	typedef void(*BenchmarkTest)();
	void RunInteropBenchmark();
	int ManagedBenchmarkCallback(int Num);

	void BuildTpaList(const char* Directory, const char* Extension, std::string& TpaList);

	extern "C" {
		__declspec(dllimport) void Log(int Level, char* Msg);
		__declspec(dllimport) void CallbackTest();
	}
}