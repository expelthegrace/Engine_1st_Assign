#pragma once
#include "Module.h"
#include "GL/glew.h"
class ModuleProgram :
	public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool            Init();
	update_status   Update();
	bool            CleanUp();

	void CreateProgram(GLuint& variable, char* vsName, char* fsName);

	GLuint programModel, programLines;
};

