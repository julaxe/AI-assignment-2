#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Sprite.h"
#include "Tile.h"
#include <array>
#include"UiElements.h"
enum class Direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};
class Player : public AnimatedSprite
{
public:
	Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf);
	void Update(std::array<std::array<Tile*, COLS>, ROWS> m_level);
	void Render();
	bool isMoving() { return m_isMoving; }
	int& getLife() { return m_Life; }

	void setAnimationState(AnimationState);
	void drawLOS();

private:
	SDL_Point m_velocity;
	bool m_dir;
	bool m_isMoving;
	void SetState(int s);
	int m_Life = 100;
	std::vector<UIElements*> UIList;

};

#endif

