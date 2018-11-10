#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleInput.h"
#include "ModuleMenu.h"
#include "SDL.h"
#include "ModuleModelLoader.h"


ModuleCamera::ModuleCamera()
{
}


ModuleCamera::~ModuleCamera()
{
}



bool  ModuleCamera::Init() { // ----------------------------------------------------------------
	cameraChanged = false;
	speed1 = 0.1;
	speed2 = speed1 * 3.5;
	movementSpeed = speed1;
	rotationSpeed = 0.0015;

	camPos = math::float3(0, 1, 8);
	distCamVrp = 2.f;
	fwd = math::float3(0, 0, -1);
	up = math::float3(0, 1, 0);

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = camPos;//cam;
	frustum.front = fwd.Normalized();
	frustum.up = up.Normalized();
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;

	screenWidth = SCREEN_WIDTH;
	screenHeight = SCREEN_HEIGHT;

	float aspect = (float)screenWidth / (float)screenHeight;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);

	side = (frustum.WorldRight()).Normalized();

	Xaxis = math::float3(1, 0, 0);
	Yaxis = math::float3(0, 1, 0);
	Zaxis = math::float3(0, 0, 1);

	view = frustum.ViewMatrix();
	projection = frustum.ProjectionMatrix();

	pressingRightMouse = false;
	lastMouse = App->input->mouse_position;

	return true;
}

void ModuleCamera::ResizeFOV(unsigned & newWidth, unsigned & newHeight) {
	float aspect = (float)newWidth / (float)newHeight;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) *aspect);
	UpdateFrustum();
	screenWidth = newWidth;
	screenHeight = newHeight;
}

update_status   ModuleCamera::Update() {
	//keyboard lecture
	//const Uint8 *keyboard = NULL;
	//keyboard listeners
	if (App->input->keyboard[SDL_SCANCODE_LSHIFT]) {
		movementSpeed = speed2;
	}
	else movementSpeed = speed1;
	
	if (App->input->keyboard[SDL_SCANCODE_F]) FocusModel();
	
	// arrow rotations
	if (App->input->keyboard[SDL_SCANCODE_LEFT]) {
		Quat pitchRotMat = Quat::RotateAxisAngle(Yaxis, rotationSpeed * 10);
		fwd = (pitchRotMat * fwd).Normalized();
		side = (pitchRotMat * side).Normalized();
		up = (pitchRotMat * up).Normalized();
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_RIGHT]) {
		Quat pitchRotMat = Quat::RotateAxisAngle(Yaxis, -rotationSpeed * 10);
		fwd = (pitchRotMat * fwd).Normalized();
		side = (pitchRotMat * side).Normalized();
		up = (pitchRotMat * up).Normalized();
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_UP]) {
		Quat pitchRotMat = Quat::RotateAxisAngle(side, rotationSpeed * 10);
		fwd = (pitchRotMat * fwd).Normalized();
		up = (side.Cross(fwd)).Normalized();
		cameraChanged = true;
	}
	if (App->input->keyboard[SDL_SCANCODE_DOWN]) {
		Quat pitchRotMat = Quat::RotateAxisAngle(side, -rotationSpeed * 10);
		fwd = (pitchRotMat * fwd).Normalized();
		up = (side.Cross(fwd)).Normalized();
		cameraChanged = true;
	}

	if (App->input->mouse_buttons[SDL_BUTTON_RIGHT - 1] == KEY_DOWN) {
		// WASD movement
		if (App->input->keyboard[SDL_SCANCODE_E]) {
			camPos += Yaxis * movementSpeed;
			cameraChanged = true;
		}
		if (App->input->keyboard[SDL_SCANCODE_Q]) {
			camPos -= Yaxis * movementSpeed;
			cameraChanged = true;
		}
		if (App->input->keyboard[SDL_SCANCODE_W]) {
			camPos += fwd * movementSpeed;
			cameraChanged = true;
		}
		if (App->input->keyboard[SDL_SCANCODE_S]) {
			camPos -= fwd * movementSpeed;
			cameraChanged = true;
		}
		if (App->input->keyboard[SDL_SCANCODE_A]) {
			camPos -= side * movementSpeed;
			cameraChanged = true;
		}
		if (App->input->keyboard[SDL_SCANCODE_D]) {
			camPos += side * movementSpeed;
			cameraChanged = true;
		}

		//mouse rotation
		if (!pressingRightMouse) {
			lastMouse = App->input->mouse_position;
			pressingRightMouse = true;
		}
		else {
			actualMouse = App->input->mouse_position;
			Punt restaMouse = { actualMouse.x - lastMouse.x, actualMouse.y - lastMouse.y };
			float3 distance = camPos - App->modelLoader->modelPosition;
			Quat rot;
			// orbit
			if (App->input->keyboard[SDL_SCANCODE_LALT]) {				

				if (restaMouse.x != 0) {
					rot = Quat::RotateAxisAngle(Yaxis, -restaMouse.x * rotationSpeed);					
					distance = rot * distance;
					camPos = App->modelLoader->modelPosition + distance;
					LookAt(App->modelLoader->modelPosition);

				}	
				if (restaMouse.y != 0) {
					rot = Quat::RotateAxisAngle(side, -restaMouse.y * rotationSpeed);
					distance = rot * distance;
					camPos = App->modelLoader->modelPosition + distance;
					LookAt(App->modelLoader->modelPosition);

				}
			}
			else {		
				if (restaMouse.x != 0) {
					rot = Quat::RotateAxisAngle(Yaxis, -restaMouse.x * rotationSpeed);
					fwd = (rot * fwd).Normalized();
					side = (rot * side).Normalized();
					up = (rot * up).Normalized();
				}
				if (restaMouse.y != 0) {
					rot = Quat::RotateAxisAngle(side, -restaMouse.y * rotationSpeed);
					fwd = (rot * fwd).Normalized();
					up = (side.Cross(fwd)).Normalized();
				}

			}
			lastMouse = actualMouse;
			cameraChanged = true;
			
		}
	}
	else {
		pressingRightMouse = false;
	}
	
	
	if (cameraChanged) {
		UpdateFrustum();
		cameraChanged = false;
	}

	
	return UPDATE_CONTINUE;
}
bool            ModuleCamera::CleanUp() {
	return true;
}

void ModuleCamera::FocusModel() {
	camPos = {
		App->modelLoader->boundingBox->CenterPoint().x,
		App->modelLoader->boundingBox->CenterPoint().y,
		App->modelLoader->boundingBox->CenterPoint().z + App->modelLoader->boundingBox->Diagonal().Length()
	};

	LookAt(App->modelLoader->boundingBox->CenterPoint());
	UpdateFrustum();
}

void ModuleCamera::LookAt(math::float3& target) {

	float3 dir = (target - camPos).Normalized();
	float3x3 rot = float3x3::LookAt(fwd, dir, up, Yaxis);
	up = rot * up;
	fwd = rot * fwd;
	side = rot * side;

}


void ModuleCamera::UpdateFrustum() {
	frustum.pos = camPos;
	frustum.front = fwd.Normalized();
	frustum.up = up.Normalized();


	view = frustum.ViewMatrix();
	projection = frustum.ProjectionMatrix();
}



/*
void ModuleCamera::setFoV(float fov){

}

void ModuleCamera::SetAspectRatio(float aspect) {

}
*/