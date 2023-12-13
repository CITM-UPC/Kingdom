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
	App->logHistory.push_back("[Editor] Creating Renderer context");
	LOG("Creating Renderer context");

	bool ret = true;

	ret = App->gEngine->renderer3D->Init();
	vsync = VSYNC;

	App->gEngine->cameraGO.GetComponent<Transform>()->Rotate(-30, vec3(0, 1, 0), Transform::Space::GLOBAL);
	App->gEngine->cameraGO.GetComponent<Transform>()->Rotate(25, vec3(1, 0, 0));
	FocusCamera();

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

	App->gEngine->renderer3D->gameObjectList.front()->GetComponent<Transform>()->Rotate(0.2, vec3(1, 0, 0), Transform::Space::GLOBAL);
	//App->gEngine->renderer3D->gameObjectList.back()->GetComponent<Transform>()->Rotate(0.1, vec3(0, 1, 0), Transform::Space::GLOBAL);


	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
	{
		App->gEngine->renderer3D->gameObjectList.back()->GetComponent<Transform>()->Move(vec3(0, 0, 0.1), Transform::Space::GLOBAL);
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		App->gEngine->renderer3D->gameObjectList.back()->GetComponent<Transform>()->Move(vec3(0, 0, -0.1), Transform::Space::GLOBAL);
	}
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	{
		App->gEngine->renderer3D->gameObjectList.back()->GetComponent<Transform>()->Move(vec3(0.1, 0, 0), Transform::Space::GLOBAL);
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		App->gEngine->renderer3D->gameObjectList.back()->GetComponent<Transform>()->Move(vec3(-0.1, 0, 0), Transform::Space::GLOBAL);
	}
	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_REPEAT)
	{
		App->gEngine->renderer3D->gameObjectList.back()->GetComponent<Transform>()->Move(vec3(0, 0.1, 0), Transform::Space::GLOBAL);
	}
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_REPEAT)
	{
		App->gEngine->renderer3D->gameObjectList.back()->GetComponent<Transform>()->Move(vec3(0, -0.1, 0), Transform::Space::GLOBAL);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		double sensitivity = 0.5;

		int dx = -App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		App->gEngine->renderer3D->gameObjectList.back()->GetComponent<Transform>()->Rotate(dx * sensitivity, glm::vec3(0, 1, 0), Transform::Space::GLOBAL);
		App->gEngine->renderer3D->gameObjectList.back()->GetComponent<Transform>()->Rotate(dy * sensitivity, glm::vec3(1, 0, 0));
	}

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
	}
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT))
		{
			mouseCamOrbit();
		}
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
		{
			mouseCameraPan();
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		FocusCamera();
	}
}

void ModuleRenderer::keysInputFPS()
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) { camSpeed = 0.2; }
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) { camSpeed = 0.05; }

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		App->gEngine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(0, 0, camSpeed));
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		App->gEngine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(0, 0, -camSpeed));
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		App->gEngine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(camSpeed, 0, 0));
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		App->gEngine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(-camSpeed, 0, 0));
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		App->gEngine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(0, camSpeed, 0), Transform::Space::GLOBAL);
	}
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		App->gEngine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(0, -camSpeed, 0), Transform::Space::GLOBAL);
	}
}
void ModuleRenderer::mouseInputFPS()
{
	double sensitivity = 0.05;

	int dx = -App->input->GetMouseXMotion();
	int dy = App->input->GetMouseYMotion();

	App->gEngine->cameraGO.GetComponent<Transform>()->Rotate(dx * sensitivity, glm::vec3(0, 1, 0), Transform::Space::GLOBAL);
	App->gEngine->cameraGO.GetComponent<Transform>()->Rotate(dy * sensitivity, glm::vec3(1, 0, 0));
}

void ModuleRenderer::mouseCamOrbit()
{
	double sensitivity = 0.1;

	int dx = -App->input->GetMouseXMotion();
	int dy = App->input->GetMouseYMotion();

	App->gEngine->cameraGO.GetComponent<Transform>()->MoveTo(App->gEngine->cameraGO.GetComponent<Camera>()->lookAtPos);

	App->gEngine->cameraGO.GetComponent<Transform>()->Rotate(dx * sensitivity, vec3(0, 1, 0), Transform::Space::GLOBAL);
	App->gEngine->cameraGO.GetComponent<Transform>()->Rotate(dy * sensitivity, vec3(1, 0, 0));

	vec3 finalPos = App->gEngine->cameraGO.GetComponent<Transform>()->position() - (App->gEngine->cameraGO.GetComponent<Transform>()->forward() * App->gEngine->cameraGO.GetComponent<Camera>()->camOffset);
	App->gEngine->cameraGO.GetComponent<Transform>()->MoveTo(finalPos);
}
void ModuleRenderer::mouseCameraPan()
{
	int dx = App->input->GetMouseXMotion();
	int dy = App->input->GetMouseYMotion();

	float panSpeed = 0.01f;

	App->gEngine->cameraGO.GetComponent<Transform>()->Move(vec3(dx * panSpeed, 0, 0));
	App->gEngine->cameraGO.GetComponent<Transform>()->Move(vec3(0, dy * panSpeed, 0));
}

void ModuleRenderer::DoZoom()
{
	int scrollWheel = App->input->GetMouseZ();

	if (scrollWheel != 0)
	{
		float zoomSensitivity = 0.3f;

		App->gEngine->cameraGO.GetComponent<Camera>()->camOffset -= scrollWheel * zoomSensitivity;

		if (App->gEngine->cameraGO.GetComponent<Camera>()->camOffset <= 0.1f)
		{
			App->gEngine->cameraGO.GetComponent<Camera>()->camOffset = 0.1f;
			return;
		}
		else if (App->gEngine->cameraGO.GetComponent<Camera>()->camOffset >= 50.0f)
		{
			App->gEngine->cameraGO.GetComponent<Camera>()->camOffset = 50.0f;
			return;
		}

		App->gEngine->cameraGO.GetComponent<Transform>()->Move(vec3(0, 0, scrollWheel * zoomSensitivity));
	}
}

void ModuleRenderer::FocusCamera()
{
	vec3 targetPos = App->ui->GetSelectedObjectPos() - App->gEngine->cameraGO.GetComponent<Transform>()->forward() * App->gEngine->cameraGO.GetComponent<Camera>()->camOffset;

	App->gEngine->cameraGO.GetComponent<Transform>()->MoveTo(targetPos);
}