#pragma once
#include "Module.h"
#include "Globals.h"

typedef unsigned __int8 Uint8;

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

struct Punt {
	int x, y;
};

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	const Uint8 *keyboard = NULL;
	Uint8 *mouse_buttons = NULL;

	Punt mouse_position;
	Punt mouse;

private:
};