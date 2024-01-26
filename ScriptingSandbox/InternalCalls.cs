using System;
using System.Runtime.CompilerServices;

public static class InternalCalls
{
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static string NoSeQueEstaPasandoPeroPrueboAsi();

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static ulong GetUUID(out ulong uuid);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static ITransform GetTransform(ulong uuid, out double posX);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static ITransform SetTransform(ulong uuid, ref double posX);
}
