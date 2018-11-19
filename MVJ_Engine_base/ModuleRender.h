#ifndef _MODULERENDER_
#define _MODULERENDER_

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib.h"
#include <vector>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ComponentMesh;
class GameObject;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);

	ComponentMesh* CreateComponentMesh( GameObject* my_go);
	ComponentMesh* CreateComponentMesh( GameObject* my_go,int idMesh, char* path);

	math::float4x4 model; 

	bool renderTexture, showGrid;

	void* context;

private:
	std::vector<ComponentMesh*> meshComponents;
};

#endif