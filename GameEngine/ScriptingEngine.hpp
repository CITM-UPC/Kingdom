#pragma once
#include <iostream>
#include "Engine_Globals.h"
#include "..\mono\include\mono\jit\jit.h"
#include "..\mono\include\mono\metadata\assembly.h"


class ScriptingEngine
{
public:
	MonoDomain* monoRootDomain = NULL;
	MonoDomain* monoAppDomain = NULL;

	void InitMono()
	{
		mono_set_assemblies_path("../mono/lib/4.5");

		MonoDomain* rootDomain = mono_jit_init("MyScriptRuntime");

		if (rootDomain == nullptr)
		{
			// Maybe log some error here
			std::cout << "MonoDomain has not been initialised correctly." << std::endl;
			return;
		}

		// Store the root domain pointer
		monoRootDomain = rootDomain;

		char appDomainName[] = "MyAppDomain";
		monoAppDomain = mono_domain_create_appdomain(appDomainName, nullptr);
		mono_domain_set(monoAppDomain, true);
	}
};