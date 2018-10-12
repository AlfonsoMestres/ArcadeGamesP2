#include "Globals.h"
#include "Application.h"
#include "ModuleSceneHonda.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"


ModuleSceneHonda::ModuleSceneHonda(bool start_enabled) : Module(start_enabled)
{
	// ground
	ground.x = 7;
	ground.y = 377;
	ground.w = 848;
	ground.h = 64;

	// Background / sky
	background.x = 120;
	background.y = 129;
	background.w = 671;
	background.h = 201;

}

ModuleSceneHonda::~ModuleSceneHonda()
{}

// Load assets
bool ModuleSceneHonda::Start()
{
	LOG("Loading honda scene");

	graphics = App->textures->Load("honda_stage.png");

	App->audio->PlayMusic("honda.ogg");
	/*App->player->Enable();*/

	return true;
}

// UnLoad assets
bool ModuleSceneHonda::CleanUp()
{
	LOG("Unloading honda scene");

	App->textures->Unload(graphics);
	App->player->Disable();

	return true;
}

update_status ModuleSceneHonda::PreUpdate() {

	return UPDATE_CONTINUE;

}

// Update: draw background
update_status ModuleSceneHonda::Update()
{
	// Draw everything -------------------------------------

	//Parallax 0: Background
	App->renderer->Blit(graphics, 0, 0, &background, 0.8f); // wall
	App->renderer->Blit(graphics, 0, 175, &ground, 1.0f);

	return UPDATE_CONTINUE;
}