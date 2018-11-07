#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"
#include "ModuleRender.h"



ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[10] mouse_buttons;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	mouseWheel = 0;
	mouse_buttons = new Uint8[10];


	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate()
{
	SDL_PumpEvents();

	keyboard = SDL_GetKeyboardState(NULL);

	if (keyboard[SDL_SCANCODE_ESCAPE]) {
		return UPDATE_STOP;
	}
	
	mouseWheel = 0;
	static SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			App->exit = true;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				App->renderer->WindowResized(event.window.data1, event.window.data2);
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			mouse.x = event.motion.xrel;
			mouse.y = event.motion.yrel;
			mouse_position.x = event.motion.x;
			mouse_position.y = event.motion.y;
			break;

		case SDL_MOUSEWHEEL:
			mouseWheel = event.wheel.y;
			break;
		}
	}


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}