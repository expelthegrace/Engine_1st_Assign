#ifndef _MESHCOMP_
#define _MESHCOMP_

#include "Component.h"


struct Mesh {

	unsigned vbo;
	unsigned ibo;
	unsigned numVerticesMesh;
	unsigned numIndexesMesh;
	unsigned materialIndex;
	unsigned numVertices;
	unsigned numFaces;

};

class ComponentMesh :
	public Component
{
public:
	ComponentMesh();
	~ComponentMesh();


public:
	Mesh mesh;
	bool avaliable;


};

#endif

