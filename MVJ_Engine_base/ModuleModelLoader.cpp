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

	unsigned ibo;
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

		sprintf(b, "Generated mesh with %u vertices \n", numVerticesMesh[i]);
		App->menu->console.AddLog(b);
		sprintf(b, "Generated mesh with %u indexes \n \n", numIndexesMesh[i]);
		App->menu->console.AddLog(b);
		
	}
}

void ModuleModelLoader::GenerateMaterials(const aiScene* scene)
{
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		const aiMaterial* src_material = scene->mMaterials[i];
		unsigned dst_material;
		//Material dst_material;

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

bool ModuleModelLoader::Init() {

	numMeshes = 0;

	scene = aiImportFile("BakerHouse.fbx", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FixInfacingNormals);
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
		textures = new unsigned[numMeshes];
		materials = new unsigned[numMeshes];
		numVerticesMesh = new unsigned[numMeshes];
		numIndexesMesh = new unsigned[numMeshes];

		/*
		for (unsigned i = 0; i < numMeshes; ++i)
		{
			vbos[i] = GenerateMeshData(scene->mMeshes[i],i);
			ibos[i] = GenerateMeshIndexes(scene->mMeshes[i],i);

			
			sprintf(b, "Generated mesh with %u vertices \n", numVerticesMesh[i]);
			App->menu->console.AddLog(b);
			sprintf(b, "Generated mesh with %u indexes \n \n", numIndexesMesh[i]);
			App->menu->console.AddLog(b);
		}
		*/

		GenerateMeshes(scene);
		GenerateMaterials(scene);
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
