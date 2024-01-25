﻿using System;

public class CSharpTesting
{
    public CSharpTesting() { }

    public float MyPublicFloatVar = 5.0f;

    public void PrintFloatVar()
    {
        Console.WriteLine(string.Format("MyPublicFloatVar = {0}", MyPublicFloatVar));
    }

    private void IncrementFloatVar(float value)
    {
        MyPublicFloatVar += value;
    }

}