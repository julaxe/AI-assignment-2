#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Tile.h"
#include <array>
#include"UiElements.h"

class Player : public AnimatedSprite
{
public:
	Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf);
	void Update();
	void Render();
	bool isMoving() { return m_isMoving; }
	int& getLife() { return m_Life; }

	void Shoot();
	void setAnimationState(AnimationState);
	void drawLOS();
	
	

private:
	bool m_isMoving;
	int m_Life = 100;
	std::vector<UIElements*> UIList;
	float m_scale;
	

};

#endif

