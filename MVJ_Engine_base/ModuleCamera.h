#pragma once
#include "Module.h"
#include "MathGeoLib.h"

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
	math::float3 transformation(math::float3 point, math::float3 transf);
	void rotationX(math::float3& p, float angle);
	void rotationY(math::float3& p, float angle);
	void rotationZ(math::float3& p, float angle);


	void setFoV(float fov);//changes vertical Fov
	void SetAspectRatio(float aspect);//changes horizontal Fov
	//variables
	float distCamVrp;
	math::float3 cam;
	math::float3 vrp;
	math::float3 up;
	math::float3 fwd;
	math::float3 side;
	math::float3 Xaxis;
	math::float3 Yaxis;
	math::float3 Zaxis;
	float movementSpeed;


	Frustum frustum;
	math::float4x4 view;
	math::float4x4 projection;

private:
	bool cameraChanged;

};

