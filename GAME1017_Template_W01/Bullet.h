#pragma once
#include "Sprite.h"
class Bullet : public Sprite
{
public:
	Bullet(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t,int angle, bool running);
	~Bullet();

	void update();
	void Render();
	void Execute(SDL_FRect PlayerDst, int angle);

private:
	bool m_muzzle = false;
	int timer;
};
