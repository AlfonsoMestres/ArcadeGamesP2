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
	playerPosition.x = 80;
	playerPosition.y = 110;

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
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 }); 
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 }); 
	throwHadouken.frames.push_back({ 357, 1539, 106, 96 });
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
	hadouken.frames.push_back({ 550, 1563, 56, 32 });
	hadouken.speed = 0.2f;

	specialXMod = 0;
	rngdAtkMaxDistTraveled = 0;
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
	onGoingAnimation = nullptr;
	onGoingSpecial = nullptr;
	App->textures->Unload(graphics);

	return true;
}

// TODO: If we want multiple special attacks to be displayed, we require to set up an array of them
// and start managing them through the list instead of the unique reference.
void ModulePlayer::ThrowRangedAttack(Animation& specialAnimation, int attackDuration) {
	onGoingSpecial = &specialAnimation;
	rngdAtkMaxDistTraveled = playerPosition.x + specialAnimation.GetCurrentFrame().w * attackDuration;
	specialAttack = onGoingSpecial->GetCurrentFrame();
	specPosition.x = playerPosition.x + throwHadouken.GetCurrentFrame().w;
	specPosition.y = playerPosition.y + throwHadouken.GetCurrentFrame().h * 0.25f;
}

void ModulePlayer::UpdateSpecialAttack() {
	if (onGoingSpecial->LastFrame()) {
		onGoingSpecial->InitAnimation();
	}

	specialAttack = onGoingSpecial->GetCurrentFrame();

	specPosition.x += 1.5f;
	
	if (specPosition.x >= rngdAtkMaxDistTraveled) {
		onGoingSpecial = nullptr;
	}
}

// TODO: To feel the gameplay smoother we need to wait one more frame after finishing the animation
// To make this happens without developing a full feature we repeated the last frame from the selected animations
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

	if (onGoingSpecial != nullptr) {
		UpdateSpecialAttack();
	}

	if (onGoingAnimation != nullptr)
		repeatUntilFinished();
	else
		switch (playerState) {
			case Standing:
				if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
					onGoingAnimation = &throwHadouken;
					nextPlayerState = Standing;
					ThrowRangedAttack(hadouken, 5);
				} else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
					onGoingAnimation = &lightPunch;
					nextPlayerState = Standing;
				} else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
					onGoingAnimation = &heavyPunch;
					nextPlayerState = Standing;
				} else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
					player = forward.GetCurrentFrame();
					playerPosition.x += speed;
				} else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP) {
					forward.InitAnimation();
				} else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
					player = backward.GetCurrentFrame();
					playerPosition.x -= speed;
				} else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP) {
					backward.InitAnimation();
				}  else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
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
	App->renderer->Blit(graphics, playerPosition.x, playerPosition.y, &player, 1.0f);

	if(onGoingSpecial != nullptr)
		App->renderer->Blit(graphics, specPosition.x, specPosition.y, &specialAttack, 1.0f);

	return UPDATE_CONTINUE;
}
