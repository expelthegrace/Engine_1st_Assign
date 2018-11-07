#include "ModuleModelLoader.h"


#include "GL/glew.h"
#include "SDL.h"
#include "MathGeoLib.h"
#include "ModuleMenu.h"
#include "Application.h"
#include "ModuleTextures.h"


ModuleModelLoader::ModuleModelLoader()
{
}


ModuleModelLoader::~ModuleModelLoader()
{
}



void ModuleModelLoader::GenerateMeshes(const aiScene* scene)
{
	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* src_mesh = scene->mMeshes[i];

		numVertices += src_mesh->mNumVertices;
		numFaces += src_mesh->mNumFaces;

		unsigned* vboActual = &vbos[i];

		glGenBuffers(1, vboActual);
		glBindBuffer(GL_ARRAY_BUFFER, *vboActual);

		// Positions

		glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2)*src_mesh->mNumVertices, nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * src_mesh->mNumVertices, src_mesh->mVertices);

		// Texture coords

		math::float2* texture_coords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * src_mesh->mNumVertices,
			sizeof(float) * 2 * src_mesh->mNumVertices, GL_MAP_WRITE_BIT);
		for (unsigned i = 0; i < src_mesh->mNumVertices; ++i)
		{
			texture_coords[i] = math::float2(src_mesh->mTextureCoords[0][i].x, src_mesh->mTextureCoords[0][i].y);
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Indices

		unsigned* iboActual = &ibos[i];

		glGenBuffers(1, iboActual);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *iboActual);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*src_mesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);

		unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
			sizeof(unsigned)*src_mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

		for (unsigned i = 0; i < src_mesh->mNumFaces; ++i)
		{
			assert(src_mesh->mFaces[i].mNumIndices == 3);

			*(indices++) = src_mesh->mFaces[i].mIndices[0];
			*(indices++) = src_mesh->mFaces[i].mIndices[1];
			*(indices++) = src_mesh->mFaces[i].mIndices[2];
		}

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		textures[i] = src_mesh->mMaterialIndex;
		numVerticesMesh[i] = src_mesh->mNumVertices;
		numIndexesMesh[i] = src_mesh->mNumFaces * 3;

		/*
		sprintf(b, "Generated mesh with %u vertices \n", numVerticesMesh[i]);
		App->menu->console.AddLog(b);
		sprintf(b, "Generated mesh with %u indexes \n", numIndexesMesh[i]);
		App->menu->console.AddLog(b);
		*/
		
	}
}

void ModuleModelLoader::GenerateMaterials(const aiScene* scene)
{
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		const aiMaterial* src_material = scene->mMaterials[i];
		unsigned dst_material;

		aiString file;
		aiTextureMapping mapping;
		unsigned uvindex = 0;

		if (src_material->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS)
		{
			dst_material = App->textures->Load(file.data, false);
		}

		materials[i] = dst_material;
	}
}

bool ModuleModelLoader::LoadNewModel(char* path) {

	if (modelLoaded) {
		delete vbos;
		delete ibos;
		delete textures;
		delete materials;
		delete numVerticesMesh;
		delete numIndexesMesh;
	}

	numVertices = 0;
	numFaces = 0;

	scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FixInfacingNormals);
	const char* errorMesage;

	if (scene == nullptr) {
		errorMesage = aiGetErrorString();
		sprintf(b, "Error loading model: %s", errorMesage);
		App->menu->console.AddLog(b);
	}
	else {
		numMeshes = scene->mNumMeshes;

		vbos = new unsigned[numMeshes];
		ibos = new unsigned[numMeshes];
		textures = new unsigned[numMeshes];
		materials = new unsigned[numMeshes];
		numVerticesMesh = new unsigned[numMeshes];
		numIndexesMesh = new unsigned[numMeshes];

		
		GenerateMeshes(scene);
		GenerateMaterials(scene);
	}

	//Console data update

	sprintf(b, "------------------ Model loaded ------------------ \n");
	App->menu->console.AddLog(b);
	sprintf(b, "Model loaded with path: %s \n", path);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of meshes: %u \n", numMeshes);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of vertices: %u \n", numVertices);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of faces: %u \n", numFaces);
	App->menu->console.AddLog(b);

	modelLoaded = true;
	return true;

}

bool ModuleModelLoader::Init() {
	modelLoaded = false;
	LoadNewModel("BakerHouse.fbx");

	return true;
}
