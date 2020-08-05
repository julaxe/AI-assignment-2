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
	virtual void update() override;
	virtual void Render() override;
	virtual void setState(AnimationState);
	virtual AnimationState getState();
	virtual void drawPath();
	virtual std::vector<PathConnection*>& getPath() { return m_path; }
	virtual std::vector<PathNode*> getDestinations() { return m_destinations; }
	virtual void setDestinations();


	virtual bool IsAlive() { return m_alive; }


protected:

	void Seeking(int x, int y);
	void Patrol();
	void Die();

	std::vector<UIElements*> UIList;
	std::vector<PathConnection*> m_path;
	std::vector<PathNode*> m_destinations;
	
	int deathTimer;
	int destinationNumber;
	int pathCounter;

	bool m_alive;

};