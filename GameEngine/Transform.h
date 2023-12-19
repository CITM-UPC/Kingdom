#pragma once
#include "Component.h"
#include "Engine_Globals.h"
#include <glm/gtc/matrix_transform.hpp>

class Transform : public Component
{
public:
	enum class Space
	{
		LOCAL,
		GLOBAL,
	};

	Transform(GameObject* owner);
	~Transform();

	//Moves the object to 'position'.
	void MoveTo(vec3 position);

	//Moves the object in 'displacement' increments.
	void Move(vec3 displacement, Space referenceFrame = Space::LOCAL);

	//Rotates the object for its rotation to be the one given by 'axis'.
	void RotateTo(double angle, vec3 axis);

	//Rotates the object in 'axis' increments.
	void Rotate(double angle, vec3 axis, Space referenceFrame = Space::LOCAL);

	void Update() override;

	Type getType() const override {
		return Type::TRANSFORM;
	}

public:
	union
	{
		mat4 _transformationMatrix;
		struct
		{
			vec3 _right;	double _padding1;
			vec3 _up;		double _padding2;
			vec3 _forward;	double _padding3;
			vec3 _position;	double _padding4;
		};
	};

public:

	inline const mat4& transform() const { return _transformationMatrix; }
	inline const vec3& position() const { return _position; }
	inline const vec3& pos() const { return _position; }

	inline const vec3& right() const { return _right; }
	inline const vec3& up() const { return _up; }
	inline const vec3& forward() const { return _forward; }

	json SaveInfo();
};
