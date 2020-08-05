#pragma once
#include "Enemy.h"
#include "Tile.h"
#include <array>
#include"UiElements.h"
#include "LevelManager.h"
class RangeEnemy : public Enemy
{
public:
	RangeEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf);
	void update() override;
	void setState(AnimationState state) override;
	void setDestinations() override;
};
