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

	unsigned GenerateMeshData(const aiMesh* mesh);
	unsigned GenerateMeshIndexes(const aiMesh* mesh);
	unsigned* vbos;
	unsigned* ibos;

	const aiScene* scene;

};

