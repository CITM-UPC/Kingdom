#include "Camera.h"

Camera::Camera() : fov(60), aspectRatio(4.0 / 3.0), clippingPlaneViewNear(0.1), clippingPlaneViewFar(100), lookAtPos(0, 0, 0), camCenter(2, 2, 5), upVector	(0, 1, 0) {}