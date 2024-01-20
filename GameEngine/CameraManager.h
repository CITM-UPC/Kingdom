#pragma once
#include <list>
#include "GameObject.h"

class CameraManager
{
public:
	CameraManager() {};
	~CameraManager() {};

	void SetActiveCamera(GameObject* camToSet)
	{
		activeCameraGO = camToSet;
	}
	void AddCameraToList(GameObject* camToAdd)
	{
		cameraGOsList.push_back(camToAdd);
	}

	GameObject* activeCameraGO;
	std::list<GameObject*>cameraGOsList;
};