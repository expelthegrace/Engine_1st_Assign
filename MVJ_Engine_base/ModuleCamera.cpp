#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleInput.h"
#include "ModuleMenu.h"
#include "SDL.h"


ModuleCamera::ModuleCamera()
{
}


ModuleCamera::~ModuleCamera()
{
}

math::float3 ModuleCamera::transformation(math::float3 point, math::float3 transf) {
	math::float4x4 transfMat = math::float4x4::identity;
	transfMat[0][3] = transf.x; transfMat[1][3] = transf.y; transfMat[2][3] = transf.z;
	math::float4 point4; point4[0] = point.x; point4[1] = point.y; point4[2] = point.z; point4[3] = 1;

	point4 = transfMat * point4;
	math::float3 res; res[0] = point4.x; res[1] = point4.y; res[2] = point4.z;
	return res;
}

void ModuleCamera::rotationX(math::float3& p, float angle) {
	math::float3x3 rotX;
	rotX[0][0] = 1; rotX[0][1] = 0;					rotX[0][2] = 0;
	rotX[1][0] = 0; rotX[1][1] = SDL_cos(angle);	rotX[1][2] = -SDL_sin(angle);
	rotX[2][0] = 0; rotX[2][1] = SDL_sin(angle);	rotX[2][2] = SDL_cos(angle);

	p = p * rotX;
	//p = transformation(p, cam);
	
}

void ModuleCamera::rotationY(math::float3& p, float angle) {
	math::float3x3 rotY;
	rotY[0][0] = math::Cos(angle);	rotY[0][1] = 0;					rotY[0][2] = math::Sin(angle);
	rotY[1][0] = 0;					rotY[1][1] = 1;					rotY[1][2] = 0;
	rotY[2][0] = -math::Sin(angle);	rotY[2][1] = 0;					rotY[2][2] = math::Cos(angle);
	p = rotY * p;
}

void ModuleCamera::rotationZ(math::float3& p, float angle) {
	math::float3x3 rotZ;
	rotZ[0][0] = math::Cos(angle);	rotZ[0][1] = -math::Sin(angle);	rotZ[0][2] = 0;
	rotZ[1][0] = math::Sin(angle);	rotZ[1][1] = math::Cos(angle);	rotZ[1][2] = 0;
	rotZ[2][0] = 0;					rotZ[2][1] = 0;					rotZ[2][2] = 1;
	p = rotZ * p;
}

bool  ModuleCamera::Init() { // ----------------------------------------------------------------
	cameraChanged = false;
	movementSpeed = 0.1;
	cam = math::float3(0, 0, 3);
	distCamVrp = 2.f;
	fwd = math::float3(0, 0, -1);
	vrp = cam + fwd * distCamVrp;

	//vrp = math::float3(0, 0, 0);
	up = math::float3(0, 1, 0);

	Xaxis = math::float3(1, 0, 0);
	Yaxis = math::float3(0, 1, 0);
	Zaxis = math::float3(0, 0, 1);
	//distCamVrp = sqrt(pow((cam.x - vrp.x), 2) + pow((cam.y - vrp.y), 2) + pow((cam.z - vrp.z), 2));

	view = lookAt(cam, vrp, up);
	UpdateProjection();

	return true;
}
update_status   ModuleCamera::Update() {
	//keyboard lecture
	//const Uint8 *keyboard = NULL;
	//keyboard listeners
	if (App->input->keyboard[SDL_SCANCODE_Q]) {
		cam += Yaxis*movementSpeed;
		vrp += Yaxis*movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_E]) {
		cam -= Yaxis*movementSpeed;
		vrp -= Yaxis*movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_W]) {
		cam -= Zaxis*movementSpeed;
		vrp -= Zaxis*movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_A]) {
		cam -= Xaxis*movementSpeed;
		vrp -= Xaxis*movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_S]) {
		cam += Zaxis *movementSpeed;
		vrp += Zaxis *movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_D]) {
		cam += Xaxis *movementSpeed;
		vrp += Xaxis *movementSpeed;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_LSHIFT]) {
		movementSpeed = 2;
	}
	else movementSpeed = 0.1;

	//arrows to rotate the camera
	if (App->input->keyboard[SDL_SCANCODE_UP]) {
		rotationX(up, 0.05);
		fwd = up.Cross(side);
		vrp = cam + fwd*distCamVrp;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_DOWN]) {
		rotationX(up, -0.05);
		fwd = up.Cross(side);
		vrp = cam + fwd * distCamVrp;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_LEFT]) {
		rotationY(side, -0.05);
		fwd = up.Cross(side);
		vrp = cam + fwd * distCamVrp;
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_RIGHT]) {
		rotationY(side, 0.05);
		fwd = up.Cross(side);
		vrp = cam + fwd * distCamVrp;
		cameraChanged = true;
	}

	//mouse listeners
	/*static SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			mouse_motion.x = event.motion.xrel / SCREEN_SIZE;
			mouse_motion.y = event.motion.yrel / SCREEN_SIZE;
			mouse.x = event.motion.x / SCREEN_SIZE;
			mouse.y = event.motion.y / SCREEN_SIZE;
			break;
		}
	}*/


	//update matrixs and output console log with camera info
	if (cameraChanged) {
		UpdateView();
		UpdateProjection();

		char* b = new char[50];
		sprintf(b, "Camera moved! New values:\n");
		App->menu->console.AddLog(b);
		sprintf(b, "Camera position: %.2f, %.2f, %.2f \n", cam.x, cam.y, cam.z);
		App->menu->console.AddLog(b);
		sprintf(b, "VRP position: %.2f, %.2f, %.2f \n", vrp.x, vrp.y, vrp.z);
		App->menu->console.AddLog(b);
		sprintf(b, "Up values: %.2f, %.2f, %.2f \n", up.x, up.y, up.z);
		App->menu->console.AddLog(b);
		sprintf(b, "Forward valuse: %.2f, %.2f, %.2f \n", fwd.x, fwd.y, fwd.z);
		App->menu->console.AddLog(b);
		sprintf(b, "Side valuse: %.2f, %.2f, %.2f \n\n", side.x, side.y, side.z);
		App->menu->console.AddLog(b);
		cameraChanged = false;
		delete b;
	}
	return UPDATE_CONTINUE;
}
bool            ModuleCamera::CleanUp() {
	return true;
}

math::float4x4 ModuleCamera::lookAt(const math::float3& obss, const math::float3& vrpp, math::float3& upp) {
	cam = obss;

	fwd = (vrp - obss);
	fwd = fwd.Normalized();

	side = (fwd.Cross(upp));
	side = side.Normalized();

	math::float3 newUp = (side.Cross(fwd));
	math::float4x4 viewAux;
	//now that we have all the values, we generate the view matrix
	viewAux[0][0] = side.x;         viewAux[0][1] = side.y;          viewAux[0][2] = side.z;       viewAux[3][0] = 0;
	viewAux[1][0] = newUp.x;        viewAux[1][1] = newUp.y;         viewAux[1][2] = newUp.z;      viewAux[3][1] = 0;
	viewAux[2][0] = fwd.x;          viewAux[2][1] = fwd.y;           viewAux[2][2] = fwd.z;        viewAux[3][2] = 0;
	viewAux[0][3] = -side.Dot(obss); viewAux[1][3] = -newUp.Dot(obss); viewAux[2][3] = fwd.Dot(obss); viewAux[3][3] = 1;
	
	//updating cam values
	up = newUp.Normalized();

	return viewAux;
}

void ModuleCamera::UpdateProjection() {
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = cam;//cam;
	frustum.front = fwd.Normalized();
	frustum.up = up;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);

	projection = frustum.ProjectionMatrix();
}

void ModuleCamera::UpdateView() {
	fwd = (vrp - cam);
	fwd.Normalize();
	side = (fwd.Cross(up));
	side.Normalize();
	math::float3 newUp = (side.Cross(fwd));
	//now that we have all the values, we generate the view matrix
	view[0][0] = side.x;         view[0][1] = side.y;          view[0][2] = side.z;       view[3][0] = 0;
	view[1][0] = newUp.x;        view[1][1] = newUp.y;         view[1][2] = newUp.z;      view[3][1] = 0;
	view[2][0] = fwd.x;          view[2][1] = fwd.y;           view[2][2] = fwd.z;        view[3][2] = 0;
	view[0][3] = -side.Dot(cam); view[1][3] = -newUp.Dot(cam); view[2][3] = fwd.Dot(cam); view[3][3] = 1;

	//updating cam values
	up = newUp.Normalized();
	fwd = fwd.Normalized();
	side = side.Normalized();
}

void ModuleCamera::setFoV(float fov){

}

void ModuleCamera::SetAspectRatio(float aspect) {

}