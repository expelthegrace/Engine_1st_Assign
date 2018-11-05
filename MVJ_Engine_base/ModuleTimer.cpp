#include "Globals.h"
#include "ModuleTimer.h"
#include "ModuleMenu.h"
#include "Application.h"


ModuleTimer::ModuleTimer()
{
}


ModuleTimer::~ModuleTimer()
{
}

bool ModuleTimer::Init() {
	deltaTime = 0;
	gameClock = 0;
	timeScale = 1.f;
	return true;
}

float ModuleTimer::Real_Time_Since_Startup() {
	return (float)SDL_GetTicks() * 1000;
}

update_status ModuleTimer::Update() {	
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();
	realDeltaTime = (double)((NOW - LAST)  * 1000 / (double)SDL_GetPerformanceFrequency());
	deltaTime = realDeltaTime * timeScale;

	gameClock += deltaTime ;


	return UPDATE_CONTINUE;
}
