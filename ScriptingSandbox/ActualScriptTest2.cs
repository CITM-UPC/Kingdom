using System;
using System.Runtime.CompilerServices;

class ActualScriptTest2 : MonoBehaviour
{
    public ActualScriptTest2() { }

    override public void Update()
    {
        attachedGameObject.transform.posX = 0.1;
    }
}
