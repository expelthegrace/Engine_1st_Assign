#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleMenu;
class ModuleCamera;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleInput* input = nullptr;
    ModuleRenderExercise* exercise = nullptr;
	ModuleProgram* shaderProgram = nullptr;
	ModuleMenu* menu = nullptr;
	ModuleCamera* camera = nullptr;
private:
	std::list<Module*> modules;

};

extern Application* App;
