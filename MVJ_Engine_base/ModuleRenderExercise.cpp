#include "Application.h"

#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"

#include "GL/glew.h"
#include "SDL.h"
#include "ModuleCamera.h"
#include "ModuleTextures.h"

ModuleRenderExercise::ModuleRenderExercise()
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}



bool ModuleRenderExercise::Init()
{
	model = math::float4x4::identity;

	float vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,

		// uvs
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
 
	texture0 = App->textures->Load(".\\lenna.png", false);

	return true;

}

update_status ModuleRenderExercise::Update()
{

	glUseProgram(App->shaderProgram->program);
	//projection matrix
	
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->program,
		"model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->program,
		"view"), 1, GL_TRUE, &App->camera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->shaderProgram->program,
		"proj"), 1, GL_TRUE, &App->camera->projection[0][0]);
	
	int zAxis = glGetUniformLocation(App->shaderProgram->program, "newColor");
	float white[4] = { 1, 1, 1, 1 };
	glUniform4fv(zAxis, 1, white);

	
	//lines
	glLineWidth(1.0f);

	glBegin(GL_LINES);
	float d = 200.f;

	for (float i = -d; i <= d; i += 1.0f) {
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
	

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	glUniform1i(glGetUniformLocation(App->shaderProgram->program, "texture0"), 0); // 0 is related to GL_TEXTURE0

    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(
            0,                  // attribute 0
            3,                  // number of componentes (3 floats)
            GL_FLOAT,           // data type
            GL_FALSE,           // should be normalized?
            0,                  // stride
            (void*)0            // array buffer offset
            );
	glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)(sizeof(float) * 3 * 6)    // 3 float 6 vertices for jump positions
	);

    glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	drawAxis();

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
    if(vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
    }

	return true;
}

void ModuleRenderExercise::drawAxis() {
	//x, red
	int xAxis = glGetUniformLocation(App->shaderProgram->program, "newColor");
	float red[4] = { 1, 0, 0, 1 };
	glUniform4fv(xAxis, 1, red);
	glLineWidth(2.5);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0); glVertex3f(5, 0, 0);
	glEnd();

	//y green
	int yAxis = glGetUniformLocation(App->shaderProgram->program, "newColor");
	float green[4] = { 0, 1, 0, 1 };
	glUniform4fv(yAxis, 1, green);

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0); glVertex3f(0, 5, 0);
	glEnd();

	//z blue
	int zAxis = glGetUniformLocation(App->shaderProgram->program, "newColor");
	float blue[4] = { 0, 0, 1, 1 };
	glUniform4fv(zAxis, 1, blue);

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0); glVertex3f(0, 0, 5);
	glEnd();
}
