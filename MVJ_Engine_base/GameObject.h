#ifndef _GAMEOBJ_
#define _GAMEOBJ_

class Component;

class GameObject
{
public:
	GameObject();
	~GameObject();

public:
	bool active;
	char* name;
	Component* components;
};

#endif

