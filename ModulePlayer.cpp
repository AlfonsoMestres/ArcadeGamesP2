#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{
	position.x = 80;
	position.y = 110;

	// idle animation (arcade sprite sheet)
	idle.frames.push_back({7, 8, 60, 96});
	idle.frames.push_back({94, 8, 60, 96});
	idle.frames.push_back({184, 8, 60, 96});
	idle.frames.push_back({278, 8, 60, 96});
	idle.frames.push_back({366, 8, 60, 96});
	idle.speed = 0.1f;
	
	// walk backward animation (arcade sprite sheet)
	backward.frames.push_back({542, 127, 62, 94});
	backward.frames.push_back({628, 127, 62, 94});
	backward.frames.push_back({713, 127, 62, 94});
	backward.frames.push_back({797, 127, 62, 94});
	backward.frames.push_back({883, 127, 62, 94});
	backward.frames.push_back({974, 127, 62, 94});
	backward.speed = 0.1f;

	//walk forward animation
	forward.frames.push_back({ 9, 127, 63, 94 });
	forward.frames.push_back({ 78, 127, 63, 94 });
	forward.frames.push_back({ 162, 127, 63, 94 });
	forward.frames.push_back({ 259, 127, 63, 94 });
	forward.frames.push_back({ 352, 127, 63, 94 });
	forward.frames.push_back({ 432, 127, 63, 94 });
	forward.speed = 0.1f;

	// Left Punch
	punchL.frames.push_back({ 18, 264, 66, 96 });
	punchL.frames.push_back({ 108, 264, 92, 96 });
	punchL.speed = 0.15f;

	// Right Punch
	punchR.frames.push_back({ 254, 269, 60, 96 });
	punchR.frames.push_back({ 332, 269, 76, 96 });
	punchR.frames.push_back({ 432, 264, 108, 96 });
	punchR.speed = 0.15f;

	// Jump Normal
	jumpNormal.frames.push_back({ 9, 127, 56, 60 });
	jumpNormal.frames.push_back({ 78, 127, 56, 106 });
	jumpNormal.frames.push_back({ 162, 127, 50, 90 });
	jumpNormal.frames.push_back({ 259, 127, 56, 78 });
	jumpNormal.frames.push_back({ 352, 127, 63, 94 });
	jumpNormal.frames.push_back({ 432, 127, 63, 94 });
	jumpNormal.speed = 0.1f;

	// Crouch
	crouch.frames.push_back({ 32, 1200, 60, 96 });
	crouch.frames.push_back({ 115, 1200, 76, 96 });
	crouch.frames.push_back({ 196, 1200, 108, 96 });
	crouch.speed = 0.4f;

}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("ryu4.png"); // arcade version

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// TODO: To feel the gameplay smoother we need to wait one more frame after finishing the animation
void ModulePlayer::repeatUntilFinished() {
	if (onGoingAnimation->LastFrame()) {
		onGoingAnimation->InitAnimation();
		playerState = nextPlayerState;
		onGoingAnimation = nullptr;
	} else {
		player = onGoingAnimation->GetCurrentFrame();
	}
}

// Update
update_status ModulePlayer::PreUpdate()
{
	// debug camera
	int speed = 1;

	if (onGoingAnimation != nullptr)
		repeatUntilFinished();
	else
		switch (playerState) {
			case Standing:
				if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
					player = forward.GetCurrentFrame();
					position.x += speed;
				} else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP) {
					forward.InitAnimation();
				} else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
					player = backward.GetCurrentFrame();
					position.x -= speed;
				} else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP) {
					backward.InitAnimation();
				} else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
					onGoingAnimation = &punchR;
					nextPlayerState = Standing;
				} else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
					onGoingAnimation = &punchL;
					nextPlayerState = Standing;
				} else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
					onGoingAnimation = &crouch;
					nextPlayerState = Crouching;
				} else {
					player = idle.GetCurrentFrame();
				}
				break;
			case Crouching:
				if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
					crouch.InitAnimation();
					playerState = Standing;
				} 
				break;
			case Jumping:
				break;
			default:
				player = idle.GetCurrentFrame();
				break;
		}




	return UPDATE_CONTINUE;
}

update_status ModulePlayer::Update()
{
	App->renderer->Blit(graphics, position.x, position.y, &player, 1.0f);
	return UPDATE_CONTINUE;
}
