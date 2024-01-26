using System;
using System.Runtime.CompilerServices;

public static class InternalCalls
{
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static ITransform GetTransform(ulong uuid, out double posX);
}
