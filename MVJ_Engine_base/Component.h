#ifndef _COMPONENT_
#define _COMPONENT_

#include "Globals.h"
#include "GameObject.h"

class GameObject;

class Component
{
public:
	Component();

	virtual ~Component();

	virtual void Enable() {}
	virtual void Disable() {}
	virtual update_status Update() { 
		return UPDATE_CONTINUE; 
	}

public:
	type_comp type;
	bool active;
	GameObject* parent;

};

#endif

