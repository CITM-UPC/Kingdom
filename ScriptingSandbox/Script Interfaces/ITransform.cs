using System;
using System.Runtime.CompilerServices;

class ITransform
{
    public ITransform(ulong uuid)
    {
        double outPos;
        InternalCalls.GetTransform(uuid, out outPos);
        posX = outPos;
    }

    double posX;
}
