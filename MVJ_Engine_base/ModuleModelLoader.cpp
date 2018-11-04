﻿#include "ModuleModelLoader.h"


#include "GL/glew.h"
#include "SDL.h"
#include "MathGeoLib.h"
#include "ModuleMenu.h"
#include "Application.h"


ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}


//li entra un punter a la mesh n
unsigned ModuleModelLoader::GenerateMeshData(const aiMesh* mesh) {
	
	numVertices += mesh->mNumVertices;

	unsigned vbo = 0;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 5 * sizeof(float), NULL, GL_STREAM_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(float), &mesh->mVertices);
	//glBufferSubData(GL_ARRAY_BUFFER,  mesh->mNumVertices * 3 * sizeof(float), mesh->mNumVertices * 2 * sizeof(float), &mesh->mTextureCoords[0]);

	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(float), &mesh->mVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	

	return vbo;
}

unsigned ModuleModelLoader::GenerateMeshIndexes(const aiMesh* mesh) {
	
	numFaces += mesh->mNumFaces;

	assert(mesh->mFaces->mNumIndices == 3);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mNumFaces * 3 * sizeof(float), &mesh->mFaces->mIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return ibo;
}

bool ModuleModelLoader::Init() {

	numMeshes = 0;

	scene = aiImportFile("BakerHouse.fbx", 0);
	const char* errorMesage;

	if (scene == nullptr) {
		errorMesage = aiGetErrorString();
		LOG(errorMesage);
	}
	else {
		numMeshes = scene->mNumMeshes;
		numVertices = 0;
		numFaces = 0;

		vbos = new unsigned[scene->mNumMeshes];
		ibos = new unsigned[scene->mNumMeshes];

		for (unsigned i = 0; i < scene->mNumMeshes; ++i)
		{
			vbos[i] = GenerateMeshData(scene->mMeshes[i]);
			ibos[i] = GenerateMeshIndexes(scene->mMeshes[i]);
		}

	}

	//Console data update
	char* b = new char[50];
	sprintf(b, "Number of meshes: %u \n", numMeshes);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of vertices: %u \n", numVertices);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of faces: %u \n", numFaces);
	App->menu->console.AddLog(b);
	return true;
}