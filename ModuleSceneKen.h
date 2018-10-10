#ifndef __MODULESCENEKEN_H__
#define __MODULESCENEKEN_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleSceneKen : public Module
{
public:
	ModuleSceneKen( bool start_enabled = true);
	~ModuleSceneKen();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

public:
	int waveFpsLengh = 30, increasing, value;
	int verticalMod;
	SDL_Texture* graphics = nullptr;
	SDL_Rect ground;
	SDL_Rect redBoat;
	SDL_Rect background;
	Animation boatGirl;
	Animation flag;
};

#endif // __MODULESCENEKEN_H__