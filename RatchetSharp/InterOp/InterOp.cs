using RatchetSharp;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Threading;

namespace RatchetSharp
{
    public class InterOp
    {
        private static Dictionary<string, uint> NativeCallbackOffsets = new Dictionary<string, uint>();

        static void Test()
        {
            File.AppendAllText("Test.txt", "Yay :>\n");
        }

        // Called when the CLR is started
        static void Setup()
        {
            Logger.Info("Managed setup called :D");
        }

        // Called from c++ to give a list of native functions offsets to be bound
        static void GetNativeCallbacks(
            int NumPtrs,
            [In] [MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPStr, SizeParamIndex = 0)] string[] Names,
            [In] [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 0)] uint[] Offsets)
        {
            NativeCallbackOffsets = new Dictionary<string, uint>(NumPtrs);
            for (int i = 0; i < NumPtrs; i++)
            {
                var name   = Names[i];
                var offset = Offsets[i];
                NativeCallbackOffsets.Add(name, offset);
            }

            var bindings = GetAllNativeBindings();

            // Cheat abit, try and bind logging first so we can nicely log errors for the rest, so order them to the start
            bindings = bindings.OrderByDescending(x => x.Item2.Name == "Log").ToList();

            foreach (var binding in bindings)
            {
                if (NativeCallbackOffsets.TryGetValue(binding.Item2.Name, out uint offset))
                {
                    BindNativeCallback(binding.Item1, binding.Item2, offset);
                }
                else
                {
                    var logMsg = $"No native callback offset was defined for {binding.Item2.Name} of type {binding.Item2.DelegateType} for {binding.Item1.Name}";
                    Logger.Info(logMsg);
                }
            }

            TestLab.CBTest();
        }

        // Called when the CLR is shutdown
        static void Shutdown()
        {
            File.AppendAllText("Test.txt", "Shutdown\n");
        }

        // Look over all public fields and get any that should be bound to a native callback
        private static List<(FieldInfo, NativeBindingAttribute)> GetAllNativeBindings()
        {
            var bindings = new List<(FieldInfo, NativeBindingAttribute)>();
            Assembly assembly = Assembly.GetExecutingAssembly();

            foreach (Type type in assembly.GetTypes())
            {
                foreach (var item in type.GetFields().Where(x => x.GetCustomAttribute<NativeBindingAttribute>() != null))
                {
                    var attribute = item.GetCustomAttribute<NativeBindingAttribute>();
                    bindings.Add((item, attribute));
                }
            }

            return bindings;
        }

        private static void BindNativeCallback(FieldInfo Field, NativeBindingAttribute Attribute, uint Offset)
        {
            var dele = Marshal.GetDelegateForFunctionPointer((IntPtr)Offset, Attribute.DelegateType);
            Field.SetValue(null, dele);

            var logMsg = $"Bound native callback {Attribute.Name} to {Field.Name}";
            Logger.Info(logMsg);
        }
    }
}
