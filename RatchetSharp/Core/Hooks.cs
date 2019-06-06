using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;

namespace RatchetSharp
{
    [AttributeUsage(AttributeTargets.Method, Inherited = false)]
    public class HookAttribute : Attribute
    {
        public HookAttribute(ulong Offset, Type DeleType, string CBName = null)
        {
            this.Offset       = Offset;
            this.DelegateType = DeleType;
            this.CBName       = CBName;
        }

        public ulong Offset;
        public Type DelegateType;
        public string CBName;
    }

    public class Hook
    {
        #region Native Bindings
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate uint InstallHookDelegate(
                [MarshalAs(UnmanagedType.U8)] UInt64 Offset,
                [MarshalAs(UnmanagedType.LPStr)] string Name,
                [MarshalAs(UnmanagedType.FunctionPtr)] Delegate HandlerPtr
            );

        [NativeBinding("InstallHook", typeof(InstallHookDelegate))]
        public static InstallHookDelegate InstallHookCB;
        #endregion

        #region Finding and installing hooks magic
        // Reflect over all the loaded types and hook and bind any that need to be
        public static void ScanAndInstallHooks()
        {
            var hooks = GetHookAttributedFunctions();

            foreach (var hookInfo in hooks)
            {
                var dele          = hookInfo.FuncInfo.CreateDelegate(hookInfo.HookAttr.DelegateType);
                var trampolinePtr = InstallHookCB(hookInfo.HookAttr.Offset, hookInfo.FuncInfo.Name, dele);
                var cb            = GetDelegateCallback(hookInfo.ClassType, hookInfo.FuncInfo.Name);
                var cbDele        = Marshal.GetDelegateForFunctionPointer((IntPtr)trampolinePtr, hookInfo.HookAttr.DelegateType);
                cb.SetValue(null, cbDele);
            }
        }

        private static List<(Type ClassType, MethodInfo FuncInfo, HookAttribute HookAttr)> GetHookAttributedFunctions()
        {
            var hooks = new List<(Type, MethodInfo, HookAttribute)>();
            Assembly assembly = Assembly.GetExecutingAssembly();

            foreach (Type type in assembly.GetTypes())
            {
                foreach (var method in type.GetMethods())
                {
                    var attr = method.GetCustomAttribute<HookAttribute>();
                    if (attr != null)
                    {
                        hooks.Add((type, method, attr));
                        Logger.Info($"Found hook target: Class: {type.Name}, Function: {method.Name}, Offset: {attr.Offset}");
                    }
                }
            }

            return hooks;
        }

        private static FieldInfo GetDelegateCallback(Type Typ, string HookName, string NameOverride = null)
        {
            var defaultName = NameOverride ?? $"{HookName}CB";
            var fInfo       = Typ.GetField(defaultName);
            return fInfo;
        }

        private static Delegate CreateDelegateForFunction(MethodInfo Function)
        {
            var tArgs = new List<Type>();
            foreach (var param in Function.GetParameters())
            {
                tArgs.Add(param.ParameterType);
            }
            tArgs.Add(Function.ReturnType);
            var delDecltype = Expression.GetDelegateType(tArgs.ToArray());

            return Delegate.CreateDelegate(delDecltype, Function);
        }

        #endregion
    }
}
