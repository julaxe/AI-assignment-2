#pragma once
#include "Sprite.h"
class MeleeAtk : public Sprite
{
public:
	MeleeAtk(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int angle, bool running);
	~MeleeAtk();

	void update();
	void Render();
	void Execute(SDL_FRect PlayerDst, int angle);

private:
	int timer;
};