#include "ComponentMesh.h"



ComponentMesh::ComponentMesh()
{
	type = MESH;
	active = true;
	// falta el pare
	avaliable = true;
}


ComponentMesh::~ComponentMesh()
{

	delete mesh;
}
