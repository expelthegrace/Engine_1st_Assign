#ifndef __ModuleRenderExercise_h__
#define __ModuleRenderExercise_h__

#include "Module.h"
#include "MathGeoLib.h"


class ModuleRenderExercise : public Module
{
public:
    ModuleRenderExercise();
    ~ModuleRenderExercise();

	bool            Init    ();
	update_status   Update  ();
	bool            CleanUp ();

	void drawAxis();
	//void lookAt(const math::float3);

	//variables
	math::float4x4 model;

	

private:
    unsigned vbo        = 0;
	unsigned texture0;

};

#endif /* __ModuleRenderExercise_h__ */
