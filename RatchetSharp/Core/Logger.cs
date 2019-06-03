using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace RatchetSharp
{
    public class Logger
    {
        public enum LogLevels
        {
            Trace,
            Info,
            Debug,
            Warn,
            Error
        }

        // Native Logging callback
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void LogCBDelegate([MarshalAs(UnmanagedType.I4)] int Level, [MarshalAs(UnmanagedType.LPStr)] string Msg);

        [NativeBinding("Log", typeof(LogCBDelegate))]
        public static LogCBDelegate LogCB;

        public static void Log(int Level, string Msg)
        {
            LogCB?.Invoke(Level, Msg);
        }

        public static void Trace(string Msg)
        {
            Log((int)LogLevels.Trace, Msg);
        }

        public static void Info(string Msg)
        {
            Log((int)LogLevels.Info, Msg);
        }

        public static void Debug(string Msg)
        {
            Log((int)LogLevels.Debug, Msg);
        }

        public static void Warn(string Msg)
        {
            Log((int)LogLevels.Warn, Msg);
        }

        public static void Error(string Msg)
        {
            Log((int)LogLevels.Error, Msg);
        }

        public static void InfoD(string Msg, [CallerLineNumber] int LineNumber = 0, [CallerMemberName] string Caller = null)
        {
            var str = $"({Caller}:{LineNumber}) {Msg}";
            Info(str);
        }
    }
}
