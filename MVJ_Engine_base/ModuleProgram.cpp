#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleRenderExercise.h"

#include <vector>

ModuleProgram::ModuleProgram()
{
}


ModuleProgram::~ModuleProgram()
{
}

void ModuleProgram::CreateProgram(GLuint& variable, char* vsName, char* fsName) {
	char* dataVertex = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, vsName, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		dataVertex = (char*)malloc(size + 1);
		fread(dataVertex, 1, size, file);
		dataVertex[size] = 0;
		fclose(file);
	}

	char* dataFragment = nullptr;
	file = nullptr;
	fopen_s(&file, fsName, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		dataFragment = (char*)malloc(size + 1);
		fread(dataFragment, 1, size, file);
		dataFragment[size] = 0;
		fclose(file);
	}
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &dataVertex, 0);
	glCompileShader(vs);

	//case compilation error vs
	GLint params = GL_TRUE;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	GLint maxLength = 0;
	if (params == GL_FALSE) {
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vs, maxLength, &maxLength, &infoLog[0]);
		glDeleteShader(vs);
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &dataFragment, 0);
	glCompileShader(fs);
	params = GL_TRUE;
	maxLength = 0;
	//case compilation error fs
	glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (params == GL_FALSE) {
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fs, maxLength, &maxLength, &infoLog[0]);
		glDeleteShader(fs);
	}

	variable = glCreateProgram();
	glAttachShader(variable, vs);
	glAttachShader(variable, fs);
	glLinkProgram(variable);

	//case compilation error program
	GLint isLinked = 0;
	glGetProgramiv(variable, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(variable, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(variable, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(variable);
		glDeleteShader(vs);
		glDeleteShader(fs);

	}

	glDeleteShader(GL_VERTEX_SHADER);
	glDeleteShader(GL_FRAGMENT_SHADER);

	//glUseProgram(variable);
}


bool ModuleProgram::Init() {
	
	CreateProgram(programLines, "Default.vs", "Default.fs");
	CreateProgram(programModel, "ModelShader.vs", "ModelShader.fs");
	
	return true;
}
update_status   ModuleProgram::Update() {
	return UPDATE_CONTINUE;
}
bool ModuleProgram::CleanUp() {
	return true;
}
