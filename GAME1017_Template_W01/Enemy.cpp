#include "Enemy.h"
#include "DebugManager.h"
#include "EventManager.h"
#include "MathManager.h"
#include "CollisionManager.h"
#include "LifeBar.h"
#include "LevelManager.h"
#define SPEED 2
Enemy::Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf) 
{
	m_velocity = { SPEED,SPEED };
	m_angle = 0;
	m_Life = 100;
	UIList.push_back(new LifeBar);

	setDestinations();
	destinationNumber = 0;
	pathCounter = 0;
	m_path = LevelManager::calculatePathTo(this, getDestinations()[destinationNumber]);
	

}

void Enemy::Update()
{
	switch (m_animationState)
	{
	case IDLE:
		break;
	case RUNNING:
		
		Patrol();
			
		break;
	case MELEE:
		break;
	case SHOOTING:
		break;
	default:
		break;
	}
	for (auto s : UIList)
	{
		s->update(this);
	}
}

void Enemy::Render()
{
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle*180/M_PI, 0, SDL_FLIP_NONE);
	for (auto s : UIList)
	{
		s->draw();
	}
}

void Enemy::setState(AnimationState state)
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

AnimationState Enemy::getState()
{
	return m_animationState;
}

void Enemy::drawLOS()
{
	SDL_Point PlayerPosition = { m_dst.x + m_dst.w * 0.5 ,m_dst.y + m_dst.h * 0.5 };
	SDL_Point direction = { cos(m_angle) * 800, sin(m_angle) * 800 };
	SDL_Point EndPosition = { PlayerPosition.x + direction.x, PlayerPosition.y + direction.y };
	DEMA::DrawLine(PlayerPosition, EndPosition, { 255,255,255,255 });
}

void Enemy::drawRadius()
{
	DEMA::DrawCircle(m_dst.x + m_dst.w * 0.5, m_dst.y + m_dst.h * 0.5, 200);
}

void Enemy::setDestinations()
{
	m_destinations.push_back(LevelManager::m_level[2][2]->Node());
	m_destinations.push_back(LevelManager::m_level[3][2]->Node());
	m_destinations.push_back(LevelManager::m_level[3][3]->Node());
	m_destinations.push_back(LevelManager::m_level[2][3]->Node());
}

void Enemy::Patrol()
{

	if (pathCounter == 0)
	{
		Seeking(m_path[pathCounter]->GetFromNode()->Pt().x, m_path[pathCounter]->GetFromNode()->Pt().y);
	}
	else
	{
		Seeking(m_path[pathCounter]->GetToNode()->Pt().x, m_path[pathCounter]->GetToNode()->Pt().y);
	}
	if (pathCounter > m_path.size() - 1)
	{
		pathCounter = 0;
		destinationNumber++;

		if (destinationNumber > getDestinations().size() - 1)
		{
			destinationNumber = 0;
		}
		std::cout << getDestinations()[destinationNumber]->Pt().x / 32 << " " << getDestinations()[destinationNumber]->Pt().y / 32 << std::endl;
		m_path = LevelManager::calculatePathTo(this, getDestinations()[destinationNumber]);
	}


}

void Enemy::Seeking(int x, int y)
{
	int dy = y - m_dst.y + m_dst.h * 0.5;
	int dx = x - m_dst.x + m_dst.w * 0.5;

	m_angle = MAMA::AngleBetweenPoints(dy, dx);
	
	m_dst.x = m_collisionBox.x;
	m_dst.y = m_collisionBox.y;
	m_collisionBox.x += m_velocity.x * cos(m_angle);
	m_collisionBox.y += m_velocity.y * sin(m_angle);
	
	if (dx == 0 && dy == 0)
	{
		pathCounter++;
	}
}


void Enemy::HardCodedPatrol()
{
	if (!COMA::AABBCollisionWithTiles(this, LevelManager::m_obstacles))
	{
		m_dst.x = m_collisionBox.x;
		m_dst.y = m_collisionBox.y;
		m_collisionBox.x += m_velocity.x * cos(m_angle * M_PI / 180);
		m_collisionBox.y += m_velocity.y * sin(m_angle * M_PI / 180);
	}
	else
	{
		m_angle += 5;
	}
}

