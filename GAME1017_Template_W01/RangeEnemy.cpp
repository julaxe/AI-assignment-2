#include "RangeEnemy.h"
#include "LifeBar.h"
#include "SoundManager.h"
#include"DisplayManager.h"
#define SPEED 2

RangeEnemy::RangeEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:Enemy(s, d, r, t, sstart, smin, smax, nf)
{
	RangeEnemy::setDestinations();
}
void RangeEnemy::update()
{
	updatePosition();
	updateCollisionBox(40.0f, 40.0f);
	if (updateLOS(DisplayManager::PlayerList()) && !LOSalert)
	{
		LOSalert = true;
		std::cout << "Player in LOS" << std::endl;
	}
	else if (!updateLOS(DisplayManager::PlayerList()) && LOSalert)
	{
		LOSalert = false;
		std::cout << "Player OUT of LOS" << std::endl;
	}

	switch (m_animationState)
	{
	case IDLE:
		if (getLife() <= 0) {
			setState(DEATH);
			SoundManager::PlaySound("death", 0, -1);
		}
		break;
	case RUNNING:

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
	case RUNNING:
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
	m_destinations.push_back(LevelManager::m_level[6][4]->Node());
	m_destinations.push_back(LevelManager::m_level[6][7]->Node());
	m_destinations.push_back(LevelManager::m_level[9][7]->Node());
	m_destinations.push_back(LevelManager::m_level[9][4]->Node());
}
