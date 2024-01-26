using System;
using System.Runtime.CompilerServices;

class ActualScriptTest : MonoBehaviour
{
    public ActualScriptTest() { }

    override public void Update()
    {
        attachedGameObject.transform.posX = -0.1;
    }
}
