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
	virtual void update() = 0;
	virtual void Render() override;
	virtual void setState(AnimationState) = 0;
	virtual AnimationState getState();
	virtual void drawPath();
	virtual std::vector<PathConnection*>& getPath() { return m_path; }
	virtual std::vector<PathNode*> getDestinations() { return m_destinations; }
	virtual void setDestinations() = 0;


	virtual bool IsAlive() { return m_alive; }


protected:

	void Seeking(int x, int y);
	bool MoveToLOS();
	bool MoveToFleeLocation();
	bool canLookPlayer();
	bool LookPlayer();
	void Patrol();
	void Die();
	void MoveToPlayer();
	void buildPathToLOS();
	void buildPathToFlee();
	void FleeFromPlayer();

	std::vector<UIElements*> UIList;
	std::vector<PathConnection*> m_path;
	std::vector<PathNode*> m_destinations;
	std::vector<PathNode*> m_destLOS;
	std::vector<PathNode*> m_destFlee;
	
	int deathTimer;
	int destinationNumber;
	int pathCounter;

	bool m_alive;

};