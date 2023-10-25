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
	DoZoom();

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		keysInputFPS();
		mouseInputFPS();

		App->gEngine->cam.UpdateLookAt();
	}
	if (App->input->GetKey(SDL_SCANCODE_LALT) && App->input->GetMouseButton(SDL_BUTTON_LEFT))
	{
		mouseCamOrbit();
	}
}

void ModuleRenderer::keysInputFPS()
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) { camSpeed = 0.2; }
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) { camSpeed = 0.1; }

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		App->gEngine->cam.transform.Move(glm::dvec3(0, 0, camSpeed));
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		App->gEngine->cam.transform.Move(glm::dvec3(0, 0, -camSpeed));
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		App->gEngine->cam.transform.Move(glm::dvec3(camSpeed, 0, 0));
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		App->gEngine->cam.transform.Move(glm::dvec3(-camSpeed, 0, 0));
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		App->gEngine->cam.transform.Move(glm::dvec3(0, camSpeed, 0), Transform::Space::GLOBAL);
	}
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		App->gEngine->cam.transform.Move(glm::dvec3(0, -camSpeed, 0), Transform::Space::GLOBAL);
	}
}
void ModuleRenderer::mouseInputFPS()
{
	float sensitivity = 0.1;

	int dx = App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	App->gEngine->cam.transform.Rotate(glm::vec3(0, dx * sensitivity, 0), Transform::Space::GLOBAL);
	App->gEngine->cam.transform.Rotate(glm::vec3(dy * sensitivity, 0, 0));
}

void ModuleRenderer::mouseCamOrbit()
{
	float sensitivity = 0.2;

	int dx = App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	App->gEngine->cam.transform.MoveTo(App->gEngine->cam.lookAtPos);

	App->gEngine->cam.transform.Rotate(vec3(0, dx * sensitivity, 0), Transform::Space::GLOBAL);
	App->gEngine->cam.transform.Rotate(vec3(dy * sensitivity, 0, 0));

	vec3 finalPos = App->gEngine->cam.transform.position - (App->gEngine->cam.transform.forward * App->gEngine->cam.camOffset);
	App->gEngine->cam.transform.MoveTo(finalPos);
}
void ModuleRenderer::DoZoom()
{
	int scrollWheel = App->input->GetMouseZ();

	if (scrollWheel != 0)
	{
		float zoomSensitivity = 0.3f;

		App->gEngine->cam.camOffset -= scrollWheel * zoomSensitivity;

		if (App->gEngine->cam.camOffset <= 0.1f)
		{
			App->gEngine->cam.camOffset = 0.1f;
			return;
		}
		else if (App->gEngine->cam.camOffset >= 50.0f)
		{
			App->gEngine->cam.camOffset = 50.0f;
			return;
		}

		App->gEngine->cam.transform.Move(vec3(0, 0, scrollWheel * zoomSensitivity));
	}
}
