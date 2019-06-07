# 🔧 Ratchet
A gameclient hooking framework to aid debuging by embed the .Net core run time into the target.
In this case Firefall, to all debugging and extensions written with the full power of .Net Core and C#

Can use either the C++ layer or the C# layer

# Features 
* Easy hooking from c++
```cpp
// Define the hook
int HelpHook(int A1);
static Hooks::FuncDef Help("Help", 0x1248160, &HelpHook);

// Install it with
Hooks::InstallHook(Help);

// Uninstall it with
Hooks::UninstallHook(Help);

// Handle when the hooked function is called
int HelpHook(int A1)
{
	printf("Console help function :>\n");
	auto ret = Help.Trampoline(A1); // Call the orignal function
	return ret;
}
```

* Easy hooking from C#
```csharp
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate int ReloadUIDelegate(int A1);

public static ReloadUIDelegate ReloadUICB;

[Hook(0x7E39D0, typeof(ReloadUIDelegate))]
public static int ReloadUI(int A1)
{
    Logger.Info($"OnRelaodUI A1: {A1}");
    return ReloadUICB(A1); ;
}
```
(Hoping to simplfiy that more too)

* Easy bindings for c# / C++ interop
```c++
// Binding to a c# defined function
typedef void(*VoidFunc)();
BenchmarkTest2 = (VoidFunc)CreateRatchetDelegate("InterOpbenchmark2",		"RatchetSharp.TestLab");
BenchmarkTest2();
```

C#
```csharp
namespace RatchetSharp
{
  public class TestLab
  {
    public static void InterOpbenchmark2() // Some simple math
    {

    }
  }
}
```

Binding a c++ function in C#
```c++
ExposeFunction("Log", (unsigned int)&Log);

void Log(int Level, char* Msg)
{

}
```

C# side
```csharp
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void LogCBDelegate([MarshalAs(UnmanagedType.I4)] int Level, [MarshalAs(UnmanagedType.LPStr)] string Msg);

[NativeBinding("Log", typeof(LogCBDelegate))]
public static LogCBDelegate LogCB;

LogCB(1, ":>"); // call it
```

# Building
* After cloning the repo run the Setup.ps1 script to download and extract the .Net Core runtime files to the corret directory.
* Edit the PostBuild.bat file to point to your Firefall install.
* Then just open the solution and compile the project, after the build all the needed files will be copyied to the games install and the game will launch.

# Future goals
* Runtime script reloading
* IMGUI bingings and Dx9 hooking for easy UI debug widgets
* I'd like to split it off in to a more genral purpse framework for easy use in other games / applications

## Libraries / other projects used
* [Polyhook V2](https://github.com/stevemk14ebr/PolyHook_2_0)
* [spdlog](https://github.com/gabime/spdlog)
* [json](https://github.com/nlohmann/json)
* [.Net Core CLR](https://github.com/dotnet/coreclr)

This tool was created for educational purposes only and should not be used by anyone.
