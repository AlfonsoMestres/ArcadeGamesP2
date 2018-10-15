#include "Globals.h"
#include "Application.h"
#include "ModuleSceneKen.h"
#include "ModuleSceneHonda.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneKen::ModuleSceneKen(bool start_enabled) : Module(start_enabled)
{
	// ground
	ground.x = 8;
	ground.y = 391;
	ground.w = 896;
	ground.h = 72;

	// Red boat
	redBoat.x = 8;  
	redBoat.y = 25; 
	redBoat.w = 528;
	redBoat.h = 200;

	// Background / sky
	background.x = 72;
	background.y = 208;
	background.w = 768;
	background.h = 176;

	// flag animation
	flag.frames.push_back({848, 208, 40, 40});
	flag.frames.push_back({848, 256, 40, 40});
	flag.frames.push_back({848, 304, 40, 40});
	flag.speed = 0.1f;

	// boat girl animation
	boatGirl.frames.push_back({ 625, 80, 30, 55 });
	boatGirl.frames.push_back({ 625, 16, 30, 55 });
	boatGirl.frames.push_back({ 625, 80, 30, 55 });
	boatGirl.frames.push_back({ 625, 144, 30, 55 });
	boatGirl.speed = 0.06f;

	waveState = 0;
	incrWaveSize = 1;
}

ModuleSceneKen::~ModuleSceneKen()
{ }

// Load assets
bool ModuleSceneKen::Start()
{
	LOG("Loading ken scene");
	
	graphics = App->textures->Load("ken_stage.png");

	App->audio->PlayMusic("ken.ogg");
	App->player->Enable();

	return true;
}

// UnLoad assets
bool ModuleSceneKen::CleanUp()
{
	LOG("Unloading ken scene");

	App->textures->Unload(graphics);
	App->player->Disable();
	
	return true;
}

update_status ModuleSceneKen::PreUpdate() {
	
	if ((waveState >= waveLengh*3 && incrWaveSize > 0) || (waveState <= 0 && incrWaveSize < 0))
		incrWaveSize = -incrWaveSize;

	waveState += incrWaveSize;

	if (waveState <= waveLengh) {
		verticalMod = 0;
	} else if (waveState > waveLengh && waveState <= waveLengh*2) {
		verticalMod = -1;
	} else if (waveState > waveLengh *2) {
		verticalMod = -2;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		App->fade->FadeToBlack(App->scene_honda, this, 3.0f);
	}

	return UPDATE_CONTINUE;

}

// Update: draw background
update_status ModuleSceneKen::Update()
{
	// Draw everything -------------------------------------

	//Parallax 0: Background and flag
	App->renderer->Blit(graphics, 0, 0, &background, 0.7f); // sea and sky
	App->renderer->Blit(graphics, 560, 8, &(flag.GetCurrentFrame()), 0.7f); // flag animation

	//Parallax 1: Boat and girl
	App->renderer->Blit(graphics, 0, 0 + verticalMod, &redBoat, 0.85f); // red ship
	App->renderer->Blit(graphics, 193, 103 + verticalMod, &(boatGirl.GetCurrentFrame()), 0.85f); // girl in red ship
	
	App->renderer->Blit(graphics, 0, 175, &ground);

	return UPDATE_CONTINUE;
}