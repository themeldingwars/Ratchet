#include "CLR.h"

#include <Windows.h>
#include <map>
#include <chrono>

#include "../Utils/Utils.h"
#include "spdlog/spdlog.h"
#include "../Logger.h"
#include "../Hooks/Hooks.h"

namespace CLR
{
	const char* ManagedDll      = "RatchetSharp.dll";
	const char* ManagedDllName  = "RatchetSharp, Version=1.0.0.0";
	const char* ManagedDllClass = "RatchetSharp.InterOp";

	std::string NetCoreDir;
	std::string RatchetSharpDir;

	coreclr_initialize_ptr InitCoreCLR;
	coreclr_create_delegate_ptr CreateDelegate;
	coreclr_shutdown_ptr ShutdownCoreCLR;
	void* CLRHostHdl;
	unsigned int CLRDomainID;

	// Managed delegates
	//-------------------------------------------
	std::map<std::string, unsigned int> ExportedFuncsPtrs;
	VoidFunc OnCLRStartUp;
	VoidFunc OnCLRShutDown;
	ExposeFuncPtrs SendFuncPtrs;

	BenchmarkTest BenchmarkTest1;
	VoidFunc BenchmarkTest2;
	VoidFunc BenchmarkTest3;

	//-------------------------------------------

	void Init()
	{
		Logger::CLR->info(".Net Core starting up...");
		NetCoreDir      = Utils::GetRatchetDir() + ".Net Core\\";
		RatchetSharpDir = Utils::GetRatchetDir() + "RatchetSharp\\";

		std::string coreClrPath = NetCoreDir + "coreclr.dll";
		HMODULE coreClr = LoadLibraryExA(coreClrPath.c_str(), NULL, 0);
		if (coreClr != nullptr) {
			Logger::CLR->info("Loaded coreclr from {}", coreClrPath); 
		}
		else {
			Logger::CLR->error("Unable to load core clr from {}", coreClrPath);
		}

		InitCoreCLR     = (coreclr_initialize_ptr)GetProcAddress(coreClr, "coreclr_initialize");
		CreateDelegate  = (coreclr_create_delegate_ptr)GetProcAddress(coreClr, "coreclr_create_delegate");
		ShutdownCoreCLR = (coreclr_shutdown_ptr)GetProcAddress(coreClr, "coreclr_shutdown");

		// Dlls to load into the run time
		std::string tpaList;
		BuildTpaList(NetCoreDir.c_str(), ".dll", tpaList);
		BuildTpaList((RatchetSharpDir + "Libs").c_str(), ".dll", tpaList);
		tpaList.append(RatchetSharpDir + ManagedDll);

		const char* propertyKeys[] = {
			"TRUSTED_PLATFORM_ASSEMBLIES"
		};

		const char* propertyValues[] = {
			tpaList.c_str()
		};

		//Logger::CLR->info("tpaList: {}", tpaList);

		int hr = InitCoreCLR(NetCoreDir.c_str(), "Ratchet", sizeof(propertyKeys) / sizeof(char*),
								propertyKeys, propertyValues, &CLRHostHdl, &CLRDomainID);

		if (hr >= 0) {
			Logger::CLR->info(".Net Core started");
		}
		else {
			Logger::CLR->error(".Net Core failed to start. Status: {}", hr);
		}

		BindDelegates();
		SetExportedFunctions();
		SendExposedFuncPtrsToManaged();
		OnCLRStartUp();

		Logger::CLR->info(".Net Core setup :>");
	}

	void BuildTpaList(const char* directory, const char* extension, std::string& tpaList)
	{
		std::string searchPath(directory);
		searchPath.append("*");
		searchPath.append(extension);

		WIN32_FIND_DATAA findData;
		HANDLE fileHandle = FindFirstFileA(searchPath.c_str(), &findData);

		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			do
			{
				tpaList.append(directory);
				//tpaList.append("");
				tpaList.append(findData.cFileName);
				tpaList.append(";");
			} while (FindNextFileA(fileHandle, &findData));
			FindClose(fileHandle);
		}
	}

	void** CreateRatchetDelegate(std::string Function, std::string ClassName = ManagedDllClass)
	{
		void** DelegatePtr;
		int hr = CreateDelegate(CLRHostHdl, CLRDomainID, ManagedDllName, ClassName.c_str(), Function.c_str(), (void**)&DelegatePtr);

		if (hr >= 0) {
			Logger::CLR->info("Created delegate for {}.{}", ClassName, Function);
		}
		else {
			Logger::CLR->error("Couldn't create delegate for {}.{} Status: {}", ClassName, Function, hr);
		}

		return DelegatePtr;
	}

	void ExposeFunction(std::string Name, unsigned int Ptr)
	{
		ExportedFuncsPtrs[Name] = Ptr;
	}

	void BindDelegates()
	{
		auto testPtr = (char*(*)())CreateRatchetDelegate("Test");
		//testPtr();

		OnCLRStartUp  = (VoidFunc)CreateRatchetDelegate("Setup");
		OnCLRShutDown = (VoidFunc)CreateRatchetDelegate("Shutdown");
		SendFuncPtrs  = (ExposeFuncPtrs)CreateRatchetDelegate("GetNativeCallbacks");

		BenchmarkTest1 = (BenchmarkTest)CreateRatchetDelegate("InterOpbenchmark",	"RatchetSharp.TestLab");
		BenchmarkTest2 = (VoidFunc)CreateRatchetDelegate("InterOpbenchmark2",		"RatchetSharp.TestLab");
		BenchmarkTest3 = (VoidFunc)CreateRatchetDelegate("InterOpbenchmark3",		"RatchetSharp.TestLab");
	}

	void SetExportedFunctions()
	{
		ExposeFunction("Log",							(unsigned int)&Log);
		ExposeFunction("CallbackTest",					(unsigned int)&CallbackTest);
		ExposeFunction("ManagedBenchmarkCallback",		(unsigned int)&ManagedBenchmarkCallback);

		// Hooking
		ExposeFunction("InstallHook", (unsigned int)&InstallHookCLR);
	}

	// A simple callback to test from managed code
	void CallbackTest()
	{
		Logger::CLR->info("CallbackTest yay!");
	}

	// A callback for the managed code to log with
	void Log(int Level, char* Msg)
	{
		switch ((LogLevels)Level)
		{
			case LogLevels::Trace:
				Logger::DotNet->trace(Msg);
				break;

			case LogLevels::Debug:
				Logger::DotNet->debug(Msg);
				break;

			case LogLevels::Info:
				Logger::DotNet->info(Msg);
				break;

			case LogLevels::Warn:
				Logger::DotNet->warn(Msg);
				break;

			case LogLevels::Error:
				Logger::DotNet->error(Msg);
				break;

			default:
				Logger::DotNet->info(Msg);
				break;
		}
	}

	void SendExposedFuncPtrsToManaged()
	{
		std::vector<const char*> keys;
		std::vector<unsigned int> values;
		for (auto const&[key, val] : ExportedFuncsPtrs)
		{
			keys.push_back(key.c_str());
			values.push_back(val);
		}

		int numPtrs = keys.size();

		SendFuncPtrs(numPtrs, keys.data(), values.data());
	}

	int ManagedBenchmarkCallback(int Num)
	{
		auto test = 100 / Num + 1;
		return test;
	}

	void LogBenchmarkTime(std::string Name, std::chrono::steady_clock::time_point Begin, std::chrono::steady_clock::time_point End, int NumLoops)
	{
		auto duration      = End - Begin;
		auto avg           = duration / NumLoops;
		auto durationNs    = std::chrono::duration_cast<std::chrono::nanoseconds>(avg).count();
		auto durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(avg).count();
		auto durationMs    = std::chrono::duration_cast<std::chrono::milliseconds>(avg).count();
		Logger::CLR->info("> {} took an average of {}ns ({}Mili, {}ms) over {} iterations", Name, durationNs, durationMicro, durationMs, NumLoops);
	}

	void RunInteropBenchmark()
	{
		const int NumLoops = 10000;

		Logger::CLR->info("==== Running CLR Interop benchmarks  ====");

		Logger::CLR->info("> Benchmark 1");
		{
			auto begin = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < NumLoops; i++)
			{
				BenchmarkTest1();
			}
			auto end = std::chrono::high_resolution_clock::now();
			LogBenchmarkTime("Benchmark 1", begin, end, NumLoops);
		}

		Logger::CLR->info("> Benchmark 2");
		{
			auto begin = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < NumLoops; i++)
			{
				BenchmarkTest2();
			}
			auto end = std::chrono::high_resolution_clock::now();
			LogBenchmarkTime("Benchmark 2", begin, end, NumLoops);
		}

		BenchmarkTest3();


		Logger::CLR->info("==== Finished CLR Interop benchmarks ====");
	}

	uint64_t InstallHookCLR(uint64_t Offset, const char* Name, void** HandlerPtr)
	{
		Logger::CLR->info("Offset: {}, Name: {}, HandlerPtr: {}", Offset, Name, (uint64_t)HandlerPtr);
		auto trampoline = Hooks::InstallSimpleHook(Offset, Name, (uint64_t)HandlerPtr);
		return trampoline;

	}

	void DeInit()
	{
		OnCLRShutDown();
		ShutdownCoreCLR(CLRHostHdl, CLRDomainID);
		Logger::CLR->info(".Net Core shutdown");
	}
}
