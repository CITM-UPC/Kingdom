#include "Application.h"
#include "Globals.h"
#include "..\mono\include\mono\jit\jit.h"
#include "..\mono\include\mono\metadata\assembly.h"

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

void InitMono(MonoDomain* s_RootDomain)
{
	LOG("-------------- Init Mono --------------");


	mono_set_assemblies_path("../mono/lib/4.5");

	MonoDomain* rootDomain = mono_jit_init("MyScriptRuntime");

	if (rootDomain == nullptr)
	{
		// Maybe log some error here
		LOG("MonoDomain has not been initialised correctly.");
		return;
	}

	// Store the root domain pointer
	s_RootDomain = rootDomain;


	LOG("-------------- Finished Init Mono --------------");
}

int main(int argc, char** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	MonoDomain* monoRootDomain = NULL;
	InitMono(monoRootDomain);

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
		break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;
		}
	}

	delete monoRootDomain;
	delete App;
	LOG("Exiting game '%s'...\n", TITLE);
	return main_return;
}