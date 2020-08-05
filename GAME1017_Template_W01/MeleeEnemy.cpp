#include "MeleeEnemy.h"
#include "LifeBar.h"
#include "SoundManager.h"
#include"DisplayManager.h"
#define SPEED 2

MeleeEnemy::MeleeEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:Enemy(s, d, r, t, sstart, smin, smax, nf)
{
	setDestinations();
	m_path = LevelManager::calculatePathTo(this, getDestinations()[destinationNumber]);
}
void MeleeEnemy::update()
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

void MeleeEnemy::setState(AnimationState state)
{
	m_animationState = state;
	switch (m_animationState)
	{
	case IDLE:
		m_src = { 0 , 0 , 291,226 };
		m_spriteMax = 19;
		m_sprite = 0;
		break;
	case RUNNING:
		m_src = { 0 , 528,281,221 };
		m_spriteMax = 19;
		m_sprite = 0;
		break;
	case MELEE:
		m_src = { 0,226,331,302 };
		m_spriteMax = 14;
		m_sprite = 0;
		break;
	case SHOOTING:

		break;
	default:
		break;
	}
}

void MeleeEnemy::setDestinations()
{
	m_destinations.push_back(LevelManager::m_level[2][2]->Node());
	m_destinations.push_back(LevelManager::m_level[8][2]->Node());
	m_destinations.push_back(LevelManager::m_level[8][8]->Node());
	m_destinations.push_back(LevelManager::m_level[2][8]->Node());
}
