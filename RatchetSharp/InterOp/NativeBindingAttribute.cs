using System;
using System.Collections.Generic;
using System.Text;

namespace RatchetSharp
{
    [AttributeUsage(AttributeTargets.Field, Inherited = false)]
    public class NativeBindingAttribute : Attribute
    {
        public NativeBindingAttribute(string Name, Type DelegateType)
        {
            this.Name         = Name;
            this.DelegateType = DelegateType;
        }

        public string Name;
        public Type DelegateType;
    }
}
