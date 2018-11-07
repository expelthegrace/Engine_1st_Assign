#pragma once
#include "Module.h"
#include "MathGeoLib.h"
#include "ModuleInput.h"

class ModuleCamera :
	public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	math::float4x4 lookAt(const math::float3& obs, const math::float3& vrp, math::float3& up);
	void UpdateProjection();
	void UpdateView();


	bool            Init();
	update_status   Update();
	bool            CleanUp();

	void UpdateFrustum();

	Punt lastMouse, actualMouse;

	math::float3 camPos;
	float distCamVrp;
	float movementSpeed;
	float speed1;
	float speed2;
	float rotationSpeed;

	math::float3 up;
	math::float3 fwd;
	math::float3 side;

	//math::float3 vrp;
	math::float3 Xaxis;
	math::float3 Yaxis;
	math::float3 Zaxis;

	Frustum frustum;
	math::float4x4 view;
	math::float4x4 projection;

	bool pressingRightMouse;

private:
	bool cameraChanged;

};

