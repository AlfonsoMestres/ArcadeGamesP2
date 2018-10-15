#pragma once
#include <vector>

using namespace std;

class Animation
{
public:
	float speed;
	vector<SDL_Rect> frames;

private:
	float current_frame;

public:
	Animation() : frames(), speed(1.0f), current_frame(0.0f)
	{}

	inline void InitAnimation() {
		current_frame = 0.0f;
	}

	inline bool LastFrame() {
		return current_frame >= frames.size() - 1;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= frames.size())
			InitAnimation();
		return frames[(int)current_frame];
	}
};