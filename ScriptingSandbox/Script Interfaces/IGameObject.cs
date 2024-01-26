using System;
using System.Runtime.CompilerServices;

class IGameObject
{
    IGameObject()
    {
        transform = new ITransform(uuid);
    }

    public string name;
    public bool isActive;
    public ITransform transform;


    private ulong uuid;


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
