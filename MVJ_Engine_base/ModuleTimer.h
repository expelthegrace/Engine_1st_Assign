#ifndef _MODULETIMER_
#define _MODULETIMER_

#include "SDL.h"
#include "Module.h"


class ModuleTimer :
	public Module
{
public:
	ModuleTimer();
	~ModuleTimer();

	bool Init();
	update_status Update();

	float Real_Time_Since_Startup();

public:

	float timeScale;

	Uint64 LAST;
	Uint64 NOW;

	double gameClock; 
	double deltaTime;
	double realDeltaTime;
};

/*Frame_Count: app graphics frames since game start
Time: second since game start (Game Clock)
Time_Scale: scale at which time is passing (Game Clock)
Delta_Time: last frame time expressed in seconds (Game Clock)
Real_Time_Since_Startup: seconds since game start (Real Time Clock)
Real_Time_Delta_Time: last frame time expressed in seconds (Real Time Clock)*/

#endif