using System;
using System.Runtime.CompilerServices;

public class CSharpTesting
{
    public CSharpTesting() { }

    public float MyPublicFloatVar = 5.0f;

    private string m_Name = "Hello";
    public string Name
    {
        get => m_Name;
        set
        {
            m_Name = value;
            MyPublicFloatVar += 5.0f;
        }
    }

    public void PrintFloatVar()
    {
        Console.WriteLine(string.Format("MyPublicFloatVar = {0}", MyPublicFloatVar));
    }

    private void IncrementFloatVar(float value)
    {
        MyPublicFloatVar += value;
    }

    public void accessEngine()
    {
        string textFromCplusplus = InternalTestingCalls.GettingCplusplus();
        Console.WriteLine("Text from C++ is: " + textFromCplusplus);
    }
}

public static class InternalTestingCalls
{
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static string GettingCplusplus();
}