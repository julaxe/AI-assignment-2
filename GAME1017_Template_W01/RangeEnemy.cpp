#include "RangeEnemy.h"
#include "LifeBar.h"
#include "SoundManager.h"
#include"DisplayManager.h"
#define SPEED 2

RangeEnemy::RangeEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:Enemy(s, d, r, t, sstart, smin, smax, nf)
{
	setDestinations();
	m_path = LevelManager::calculatePathTo(this, getDestinations()[destinationNumber]);
}
void RangeEnemy::update()
{
	updatePosition();
	updateCollisionBox(40.0f, 40.0f);
	updateLOS(DisplayManager::PlayerList());
	updateRAD();

	switch (m_animationState)
	{
	case IDLE:
		if (getLife() <= 0) {
			setState(DEATH);
			SoundManager::PlaySound("death", 0, -1);
		}
		break;
	case PATROL:

		Patrol();
		if (getLife() <= 0) {
			setState(DEATH);
			SoundManager::PlaySound("death", 0, -1);
		}

		break;
	case MELEE:
		break;
	case SHOOTING:
		break;
	case DEATH:
		Die();
		break;
	default:
		break;
	}
	for (auto s : UIList)
	{
		s->update(this);
	}
}

void RangeEnemy::setState(AnimationState state)
{
	m_animationState = state;
	switch (m_animationState)
	{
	case IDLE:
		m_src = { 0 , 412 , 313,206 };
		m_spriteMax = 19;
		m_sprite = 0;
		break;
	case PATROL:
		m_src = { 0 , 0,313,206 };
		m_spriteMax = 19;
		m_sprite = 0;
		break;
	case MELEE:
		break;
	case SHOOTING:
		m_src = { 0 ,206,316,206 };
		m_spriteMax = 3;
		m_sprite = 0;
		break;
	default:
		break;
	}
}

void RangeEnemy::setDestinations()
{
	m_destinations.push_back(LevelManager::m_level[4][6]->Node());
	m_destinations.push_back(LevelManager::m_level[7][6]->Node());
	m_destinations.push_back(LevelManager::m_level[7][9]->Node());
	m_destinations.push_back(LevelManager::m_level[4][9]->Node());
}
