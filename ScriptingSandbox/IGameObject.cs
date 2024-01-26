using System;
using System.Runtime.CompilerServices;

public class IGameObject
{
    public IGameObject(ulong uuid)
    {
        _uuid = uuid;
        transform = new ITransform(_uuid);
    }

    private ulong _uuid;

    public string name;
    public bool isActive;
    public ITransform transform;


    enum ComponentType
    {
        Transform,
        Mesh,
        Texture,
        Script
    }

    static void GetComponent<T>(ComponentType type)
    {
        switch (type)
        {
            case ComponentType.Transform:
                //return the ITransform that is in the GameOject
                break;
            case ComponentType.Mesh:
                break;
            case ComponentType.Texture:
                break;
            case ComponentType.Script:
                break;
            default:
                break;
        }
    }
}
