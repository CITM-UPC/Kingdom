#pragma once

#include "Engine_Globals.h"
#include "GameObject.h"
//#include "BoundingBox.hpp"

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
};

struct Triangle {
	glm::vec3 vertex0;
	glm::vec3 vertex1;
	glm::vec3 vertex2;
};

static bool RayTriangleIntersection(const Ray& ray, const Triangle& triangle, float& t)
{
	const float EPSILON = 0.000001f;
	const glm::vec3& vertex0 = triangle.vertex0;
	const glm::vec3& vertex1 = triangle.vertex1;
	const glm::vec3& vertex2 = triangle.vertex2;
	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;

	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;

	h = glm::cross(ray.direction, edge2);
	a = glm::dot(edge1, h);

	if (a > -EPSILON && a < EPSILON)
		return false; // Ray is parallel to the triangle

	f = 1.0f / a;
	s = ray.origin - vertex0;
	u = f * glm::dot(s, h);

	if (u < 0.0f || u > 1.0f)
		return false;

	q = glm::cross(s, edge1);
	v = f * glm::dot(ray.direction, q);

	if (v < 0.0f || u + v > 1.0f)
		return false;

	t = f * glm::dot(edge2, q);

	return t > EPSILON;
}

static bool RayAABBIntersection(const Ray& ray, GameObject* go) {
	AABBox box = go->computeAABB();

	float tMin = (box.min.x - ray.origin.x) / ray.direction.x;
	float tMax = (box.max.x - ray.origin.x) / ray.direction.x;

	if (tMin > tMax) std::swap(tMin, tMax);

	float tYMin = (box.min.y - ray.origin.y) / ray.direction.y;
	float tYMax = (box.max.y - ray.origin.y) / ray.direction.y;

	if (tYMin > tYMax) std::swap(tYMin, tYMax);

	if ((tMin > tYMax) || (tYMin > tMax)) return false;

	if (tYMin > tMin) tMin = tYMin;
	if (tYMax < tMax) tMax = tYMax;

	float tZMin = (box.min.z - ray.origin.z) / ray.direction.z;
	float tZMax = (box.max.z - ray.origin.z) / ray.direction.z;

	if (tZMin > tZMax) std::swap(tZMin, tZMax);

	if ((tMin > tZMax) || (tZMin > tMax)) return false;

	return true;
}