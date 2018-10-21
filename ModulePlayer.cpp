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
	lightPunch.frames.push_back({ 18, 264, 66, 96 });
	lightPunch.frames.push_back({ 108, 264, 92, 96 });
	lightPunch.frames.push_back({ 18, 264, 66, 96 });
	lightPunch.speed = 0.2f;

	// Right Punch
	heavyPunch.frames.push_back({ 254, 269, 60, 96 });
	heavyPunch.frames.push_back({ 332, 269, 76, 96 });
	heavyPunch.frames.push_back({ 432, 264, 108, 96 });
	heavyPunch.frames.push_back({ 332, 269, 76, 96 });
	heavyPunch.frames.push_back({ 254, 269, 60, 96 });
	heavyPunch.speed = 0.2f;

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

	// Crouched Left Punch
	crouchedLightPunch.frames.push_back({ 24, 1309, 69, 96 });
	crouchedLightPunch.frames.push_back({ 118, 1309, 95, 96 });
	crouchedLightPunch.frames.push_back({ 24, 1309, 69, 96 });
	crouchedLightPunch.speed = 0.2f;

	// Crouched Right Punch
	crouchedHeavyPunch.frames.push_back({ 265, 1309, 64, 96 });
	crouchedHeavyPunch.frames.push_back({ 359, 1309, 66, 96 });
	crouchedHeavyPunch.frames.push_back({ 450, 1309, 92, 96 });
	crouchedHeavyPunch.frames.push_back({ 359, 1309, 64, 96 });
	crouchedHeavyPunch.speed = 0.2f;

	//HadoukenThrow
	throwHadouken.frames.push_back({ 34, 1539, 74, 96 });
	throwHadouken.frames.push_back({ 135, 1539, 85, 96 });
	throwHadouken.frames.push_back({ 244, 1539, 90, 96 });
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 }); // To maintain certain amount of time
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 }); // and avoid developing a feature for this
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 }); // we increase the last frame
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 });
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 });
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 });
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 });
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 });
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 });
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 });
	throwHadouken.speed = 0.25f;

	// Hadouken
	hadouken.frames.push_back({ 484, 1563, 56, 32 });
	hadouken.frames.push_back({ 550, 1563, 56, 32 });
	hadouken.speed = 0.2f;

}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("ryu4.png");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

void ModulePlayer::LaunchSpecialAttack(int timeGoing, bool hitSomething) {
	if ((waveState >= waveLengh * 3 && incrWaveSize > 0) || (waveState <= 0 && incrWaveSize < 0))
		incrWaveSize = -incrWaveSize;

	waveState += incrWaveSize;

	if (waveState <= waveLengh) {
		verticalMod = 0;
	} else {
		verticalMod = -1;
	}
}


// TODO: To feel the gameplay smoother we need to wait one more frame after finishing the animation
// Early solution: added the resversed frame animation so the animation loops in the attacks
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
	int speed = 1;

	// TODO: Implement multiple key press detection so we can introduce combos and hadoukens down, downright (down and right), right, heavyPunch1
	// https://stackoverflow.com/questions/1252976/how-to-handle-multiple-keypresses-at-once-with-sdl
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
					onGoingAnimation = &lightPunch;
					nextPlayerState = Standing;
				} else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
					onGoingAnimation = &heavyPunch;
					nextPlayerState = Standing;
				} else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
					onGoingAnimation = &throwHadouken;
					nextPlayerState = Standing;
				} else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
					onGoingAnimation = &crouch;
					nextPlayerState = Crouching;
				} else {
					player = idle.GetCurrentFrame();
				}
				break;
			case Crouching:
				if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
					onGoingAnimation = &crouchedHeavyPunch;
					nextPlayerState = Crouching;
				} else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
					onGoingAnimation = &crouchedLightPunch;
					nextPlayerState = Crouching;
				} else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
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

	if(onGoingSpecial)
		App->renderer->Blit(graphics, position.x, position.y, &specialAttack, 1.0f);

	return UPDATE_CONTINUE;
}
