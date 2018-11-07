#pragma once
#include "Module.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>


class ModuleModelLoader :
	public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool            Init();
	//update_status   Update();
	//bool            CleanUp();


	bool LoadNewModel(char* path);

	//carlos
	void            GenerateMeshes(const aiScene* scene);
	void            GenerateMaterials(const aiScene* scene);


	unsigned* vbos;
	unsigned* ibos;
	unsigned* textures;
	unsigned* materials;
	unsigned*  numVerticesMesh;
	unsigned*  numIndexesMesh;


	char* b = new char[50];

	unsigned numMeshes;
	unsigned numVertices;
	unsigned numFaces;

	bool modelLoaded;

	const aiScene* scene;

};

