#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"

ModuleRenderer::ModuleRenderer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer::~ModuleRenderer()
{}

// Called before render is available
bool ModuleRenderer::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;

	ret = App->gEngine->renderer3D->Init();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer::PreUpdate()
{
	App->gEngine->renderer3D->PreUpdate();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer::Update()
{
	App->gEngine->renderer3D->Update();
	DoCameraInput();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer::PostUpdate()
{
	App->gEngine->renderer3D->DrawGrid(100, 1, true);

	App->gEngine->renderer3D->PostUpdate();
	App->ui->RenderUI();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer::CleanUp()
{
	LOG("Destroying Renderer");

	App->gEngine->renderer3D->CleanUp();

	return true;
}

void ModuleRenderer::DoCameraInput()
{
	if (App->input->GetMouseButton(3))
	{
		double increase = 0.1;

		glm::dvec3 directorVector = App->gEngine->cam.lookAtPos - App->gEngine->cam.transform.position;
		glm::dvec3 forward = glm::normalize(directorVector) * increase;
		
		glm::dmat3x3 rotMatrix = glm::dmat3x3(	0,	0,	1,
												0,	1,	0,
												-1,	0,	0);

		glm::dvec3 right = rotMatrix * forward;
		right.y = 0;

		double speed = 0.01;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			//App->gEngine->cam.lookAtPos += forward;
			//App->gEngine->cam.transform.position += forward;
			App->gEngine->cam.transform.Move(glm::dvec3(0, 0, -speed));
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			//App->gEngine->cam.lookAtPos -= forward;
			//App->gEngine->cam.transform.position -= forward;
			App->gEngine->cam.transform.Move(glm::dvec3(0, 0, speed));
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			//App->gEngine->cam.lookAtPos += right;
			//App->gEngine->cam.transform.position += right;
			App->gEngine->cam.transform.Move(glm::dvec3(speed, 0, 0));
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			//App->gEngine->cam.lookAtPos -= right;
			//App->gEngine->cam.transform.position -= right;
			App->gEngine->cam.transform.Move(glm::dvec3(-speed, 0, 0));
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		{
			//App->gEngine->cam.lookAtPos += glm::dvec3(0, increase, 0);
			//App->gEngine->cam.transform.position += glm::dvec3(0, increase, 0);
			App->gEngine->cam.transform.Move(glm::dvec3(0, speed, 0));
		}
		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
		{
			//App->gEngine->cam.lookAtPos -= glm::dvec3(0, increase, 0);
			//App->gEngine->cam.transform.position -= glm::dvec3(0, increase, 0);
			App->gEngine->cam.transform.Move(glm::dvec3(0, -speed, 0));
		}


		if (App->input->GetKey(SDL_SCANCODE_U) == KEY_REPEAT)
		{
			App->gEngine->cam.transform.Move(glm::dvec3(0, 0, 1));
		}
		if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		{
			App->gEngine->cam.transform.Move(glm::dvec3(0, 0, -1));
		}

		//double bruh = App->gEngine->cam.camOffset;
		App->gEngine->cam.UpdateLookAt();
	}
}
