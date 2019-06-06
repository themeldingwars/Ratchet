using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;

namespace RatchetSharp
{
    [SuppressUnmanagedCodeSecurity]
    public class TestLab
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void CallbackTest();

        [NativeBinding("CallbackTest", typeof(CallbackTest))]
        public static CallbackTest CBTest;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
        public delegate int InterOpbenchmark3Delegate([MarshalAs(UnmanagedType.I4)] int Num);
        [NativeBinding("ManagedBenchmarkCallback", typeof(InterOpbenchmark3Delegate))]
        public static InterOpbenchmark3Delegate InterOpbenchmark3CB;

        static void InterOpbenchmark() // Do nothing
        {

        }

        public static void InterOpbenchmark2() // Some simple math
        {
            int thing = 0;
            for (int i = 0; i < 100; i++)
            {
                thing += thing * i;
            }
        }

        public static void InterOpbenchmark3() // This time c# calls in to unmanaged
        {
            const int NumLoops = 10000;

            int thing = 0;
            var sw = Stopwatch.StartNew();
            for (int i = 1; i < NumLoops + 1; i++)
            {
                var result = InterOpbenchmark3CB(i);
                thing += result;
            }
            sw.Stop();
            var avg = sw.ElapsedMilliseconds / NumLoops;

            var logStr = $"{NumLoops} loops took a total of {sw.Elapsed} with an avg of {avg} Miliseconds";
            Logger.Info(logStr);
            Logger.Info($"thing: {thing}");

            /*var trampolinePtr = Hook.InstallHookCB(0x7E39D0, "ReloadUI", (ReloadUIDelegate)OnRelaodUI);
            ReloadUICB        = (ReloadUIDelegate)Marshal.GetDelegateForFunctionPointer((IntPtr)trampolinePtr, typeof(ReloadUIDelegate));*/

            //test(1);
        }

        public static int OnRelaodUI(int A1)
        {
            Logger.Info($"OnRelaodUI A1: {A1}");
            //ReloadUICB(913204);
            return ReloadUICB(913024);
        }

        #region Hooking tests

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate int ReloadUIDelegate(int A1);

        public static ReloadUIDelegate ReloadUICB;

        [Hook(0x7E39D0, typeof(ReloadUIDelegate))]
        public static int ReloadUI(int A1)
        {
            Logger.Info($"OnRelaodUI A1: {A1}");
            return ReloadUICB(A1); ;
        }

        #endregion
    }
}
