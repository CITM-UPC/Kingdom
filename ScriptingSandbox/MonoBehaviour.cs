using System;
using System.Runtime.CompilerServices;

public class MonoBehaviour
{
    public MonoBehaviour()
    {
        ulong uuid;
        InternalCalls.GetUUID(out uuid);
        Console.WriteLine("Received UUID is: " + uuid);
        attachedGameObject = new IGameObject((ulong)uuid);
    }

    public IGameObject attachedGameObject;

    public virtual void Start() { }
    public virtual void Update() { }
}
