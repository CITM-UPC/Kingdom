#pragma once
#include <iostream>
#include <fstream>
#include "Engine_Globals.h"
#include "..\mono\include\mono\jit\jit.h"
#include "..\mono\include\mono\metadata\assembly.h"
#include "..\mono\include\mono\metadata\attrdefs.h"

class ScriptingEngine
{
public:

    enum class Accessibility : uint8_t
    {
        None = 0,
        Private = (1 << 0),
        Internal = (1 << 1),
        Protected = (1 << 2),
        Public = (1 << 3)
    };


	ScriptingEngine() {}
	~ScriptingEngine() {}

    static void SetSceneModuleContext(void* scene_module);
    bool CheckMonoError(MonoError& error);
    void InitMono();
    void ShutDownMono();
    static char* ReadBytes(const std::string& filepath, uint32_t* outSize);
    static MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
    static void PrintAssemblyTypes(MonoAssembly* assembly);
    static MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);
    static MonoObject* InstantiateClass(const std::string& assemblyPath, const char* namespaceName, const char* className);
    void CallPrintFloatVarMethod(MonoObject* objectInstance);
    void CallIncrementFloatVarMethod(MonoObject* objectInstance, float value);
    uint8_t GetFieldAccessibility(MonoClassField* field);
    uint8_t GetPropertyAccessibility(MonoProperty* property);
    std::string MonoStringToUTF8(MonoString* monoString);
    void CallCsharp(MonoObject* objectInstance);
    void LetsTestSomeThings();
    void AddInternalCalls();
    void AddTestInternalCall();

    static void SetCurrentUUID(const unsigned long& uuid);
    static void CleanCurrentUUID();

    static void UpdateScriptInstance(MonoObject* monoBehaviourInstance);

private:

};