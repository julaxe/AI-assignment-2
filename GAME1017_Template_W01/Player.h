#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <array>
#include"UiElements.h"

class Player : public AnimatedSprite
{
public:
	Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf);
	void update() override;
	void Render() override;
	bool isMoving() { return m_isMoving; }

	void Shoot();
	void MeleeAttack();
	void setAnimationState(AnimationState);
	

private:
	void ckeckLevelBorders();
	bool m_isMoving;
	std::vector<UIElements*> UIList;
	float m_scale;
	

};

#endif

