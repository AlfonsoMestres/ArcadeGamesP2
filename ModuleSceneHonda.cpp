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
	ground.x = 8;
	ground.y = 376;
	ground.w = 848;
	ground.h = 63;

	// Background 
	background.x = 120;
	background.y = 128;
	background.w = 671;
	background.h = 200;

	// Roof
	roof.x = 91;
	roof.y = 8;
	roof.w = 765;
	roof.h = 49;

	bath_place.x = 165;
	bath_place.y = 66;
	bath_place.w = 335;
	bath_place.h = 51;

	bucket.x = 542;
	bucket.y = 85;
	bucket.w = 32;
	bucket.h = 26;

}

ModuleSceneHonda::~ModuleSceneHonda()
{}

// Load assets
bool ModuleSceneHonda::Start()
{
	LOG("Loading honda scene");

	graphics = App->textures->Load("honda_stage2.png");

	App->audio->PlayMusic("honda.ogg");
	App->player->Enable();

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
	App->renderer->Blit(graphics, -250, 175, &ground, 0.8f);
	App->renderer->Blit(graphics, -135, 0, &background, 0.7f); // wall
	App->renderer->Blit(graphics, -165, 0, &roof, 0.75f); // roof
	App->renderer->Blit(graphics, -20, 165, &bucket, 0.8f); // bath
	App->renderer->Blit(graphics, 25, 140, &bath_place, 0.75f); // bath


	return UPDATE_CONTINUE;
}