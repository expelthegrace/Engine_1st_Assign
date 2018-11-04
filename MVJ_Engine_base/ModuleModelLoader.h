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

	unsigned GenerateMeshData(const aiMesh* mesh, const unsigned iVbo);
	unsigned GenerateMeshIndexes(const aiMesh* mesh, const unsigned i);

	unsigned* vbos;
	unsigned* ibos;
	unsigned* materials;
	unsigned*  numVerticesMesh;
	unsigned*  numIndexesMesh;


	char* b = new char[50];

	unsigned numMeshes;
	unsigned numVertices;
	unsigned numFaces;

	unsigned texture0;

	const aiScene* scene;

};

