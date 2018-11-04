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


//li entra un punter a la mesh n
unsigned ModuleModelLoader::GenerateMeshData(const aiMesh* mesh, const unsigned iVbo) {
	
	numVertices += mesh->mNumVertices;

	unsigned vbo = 0;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3  * sizeof(float), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(float), &mesh->mVertices);

	/*
	math::float2* texture_coords = (math::float2*)glMapBufferRange(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices,
		sizeof(float) * 2 * mesh->mNumVertices, GL_MAP_WRITE_BIT);
	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		texture_coords[i] = math::float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}
	*/

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	

	materials[iVbo] = mesh->mMaterialIndex;
	numVerticesMesh[iVbo] = mesh->mNumVertices;
	
	return vbo;
}

unsigned ModuleModelLoader::GenerateMeshIndexes(const aiMesh* mesh, const unsigned iIbo) {
	
	numFaces += mesh->mNumFaces;

	assert(mesh->mFaces->mNumIndices == 3);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*mesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
		sizeof(unsigned)*mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3);

		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	numIndexesMesh[iIbo] = mesh->mNumFaces * 3;

	return ibo;
}

bool ModuleModelLoader::Init() {

	numMeshes = 0;

	scene = aiImportFile("BakerHouse.fbx", 0);
	const char* errorMesage;

	texture0 = App->textures->Load("Baker_house.png", false);

	if (scene == nullptr) {
		errorMesage = aiGetErrorString();
		sprintf(b, "Error loading model: %s", errorMesage);
		App->menu->console.AddLog(b);
	}
	else {
		numMeshes = scene->mNumMeshes;
		numVertices = 0;
		numFaces = 0;

		vbos = new unsigned[numMeshes];
		ibos = new unsigned[numMeshes];
		materials = new unsigned[numMeshes];
		numVerticesMesh = new unsigned[numMeshes];
		numIndexesMesh = new unsigned[numMeshes];


		for (unsigned i = 0; i < numMeshes; ++i)
		{
			vbos[i] = GenerateMeshData(scene->mMeshes[i],i);
			ibos[i] = GenerateMeshIndexes(scene->mMeshes[i],i);

			
			sprintf(b, "Generated mesh with %u vertices \n", numVerticesMesh[i]);
			App->menu->console.AddLog(b);
			sprintf(b, "Generated mesh with %u indexes \n \n", numIndexesMesh[i]);
			App->menu->console.AddLog(b);
		}
		
	}

	//Console data update
	
	sprintf(b, "------------------ Model loaded ------------------ \n");
	App->menu->console.AddLog(b);
	sprintf(b, "Number of meshes: %u \n", numMeshes);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of vertices: %u \n", numVertices);
	App->menu->console.AddLog(b);
	sprintf(b, "Number of faces: %u \n", numFaces);
	App->menu->console.AddLog(b);
	return true;
}
