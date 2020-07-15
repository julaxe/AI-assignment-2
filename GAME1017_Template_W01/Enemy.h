#pragma once
#include "Sprite.h"
#include "Tile.h"
#include <array>
class Enemy : public AnimatedSprite
{
public:
	Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf);
	void Update(std::array<std::array<Tile*, COLS>, ROWS> m_level);
	void Render();
	void setAnimationState(AnimationState);
	void drawLOS();
private:
	SDL_Point m_velocity;

};