#pragma once
#include "Sprite.h"
#include "Tile.h"
#include <array>
#include"UiElements.h"
class Enemy : public AnimatedSprite
{
public:
	Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf);
	void Update(std::vector<Tile*> obstacles);
	void Render();
	void setAnimationState(AnimationState);
	void drawLOS();

	void drawRadius();
	int& getLife() { return m_Life; }

private:
	int m_Life = 100;
	std::vector<UIElements*> UIList;

};