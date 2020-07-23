#pragma once
#include "Sprite.h"
#include "Tile.h"
#include <array>
#include"UiElements.h"
#include "LevelManager.h"
class Enemy : public AnimatedSprite
{
public:
	Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf);
	void Update();
	void Render();
	void setState(AnimationState);
	AnimationState getState();
	void drawLOS();
	void drawRadius();
	void drawPath();
	std::vector<PathConnection*>& getPath() { return m_path; }
	std::vector<PathNode*> getDestinations() { return m_destinations; }
	void setDestinations();


private:

	void Seeking(int x, int y);
	void Patrol();
	void HardCodedPatrol();
	std::vector<UIElements*> UIList;
	std::vector<PathConnection*> m_path;
	std::vector<PathNode*> m_destinations;
	SDL_Point LOSendPosition;
	int destinationNumber;
	int pathCounter;

};