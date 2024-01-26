using System;
using System.Runtime.CompilerServices;

public class ITransform
{
    ulong _uuid;

    public ITransform(ulong uuid)
    {
        _uuid = uuid;
        Console.WriteLine("MonoBehaviour's GamObject's Transform has been created (uuid: " + uuid + ")");
    } 

    public double posX
    {
        get 
        { 
            double outPos;
            InternalCalls.GetTransform(_uuid, out outPos);
            return outPos;
        }
        set
        {
            InternalCalls.SetTransform(_uuid, ref value);
        }
    }
}
