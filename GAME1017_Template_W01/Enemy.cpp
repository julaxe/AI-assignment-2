#include "Enemy.h"
#include "DebugManager.h"
#include "EventManager.h"
#include "MathManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "LifeBar.h"
#include "LevelManager.h"
#include "DisplayManager.h"
#include "Engine.h"
#define SPEED 2

Enemy::Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf) 
{
	m_velocity = { SPEED,SPEED };
	m_angle = 0;
	m_Life = 100;
	UIList.push_back(new LifeBar);
	destinationNumber = 0;
	pathCounter = 0;

	m_alive = true;
	deathTimer = 0;
}


void Enemy::Render()
{
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle*180/M_PI, 0, SDL_FLIP_NONE);
	for (auto s : UIList)
	{
		s->draw();
	}
}

AnimationState Enemy::getState()
{
	return m_animationState;
}


void Enemy::drawPath()
{
	for (auto p : m_path)
	{
		DebugManager::DrawLine(p->GetFromNode()->Pt(), p->GetToNode()->Pt(), { 255,0,0,255 });
	}
}


void Enemy::Patrol()
{

	
	Seeking(m_path[pathCounter]->GetToNode()->Pt().x, m_path[pathCounter]->GetToNode()->Pt().y);
	
	if (pathCounter > m_path.size() - 1)
	{
		pathCounter = 0;
		destinationNumber++;

		if (destinationNumber > getDestinations().size() - 1)
		{
			destinationNumber = 0;
		}
		m_path = LevelManager::calculatePathTo(this, getDestinations()[destinationNumber]);
	}


}

void Enemy::Seeking(int x, int y)
{
	double dy = y - m_dst.y - m_dst.h * 0.5;
	double dx = x - m_dst.x - m_dst.w * 0.5;
	double desiredAngle = MAMA::AngleBetweenPoints(dy, dx);
	float desiredVelocityX = m_velocity.x * cos(desiredAngle);
	float desiredVelocityY = m_velocity.y * sin(desiredAngle);

	double currentAngle = m_angle;
	float currentVelocityX = m_velocity.x * cos(currentAngle);
	float currentVelocityY = m_velocity.y * sin(currentAngle);

	double SeekForce = 0.2;
	float steeringVelX = MAMA::LerpD(currentVelocityX, desiredVelocityX, SeekForce);
	float steeringVelY = MAMA::LerpD(currentVelocityY, desiredVelocityY, SeekForce);

	m_angle = MAMA::LerpRad(currentAngle, desiredAngle, SeekForce);

	Move(steeringVelX, steeringVelY);
	
	if (abs(dx) < 10 && abs(dy) < 10)
	{
		pathCounter++;
	}
}


void Enemy::Die()
{
	int frameRate = 5;
	m_pText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/skeleton.png");
	if (deathTimer == 0)
	{
		m_src = { 0,0,64,64 };
	}
	if (deathTimer % frameRate == 0)
	{
		m_src.x += 64;
		if (m_src.x >= 64 * 3)
		{
			m_src.x = 0;
			m_src.y += 64;
			if (m_src.y >= 64 * 5)
			{
				m_alive = false;
			}
		}
	}
	deathTimer++;
}