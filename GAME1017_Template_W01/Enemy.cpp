#include "Enemy.h"
#include "DebugManager.h"
#include "EventManager.h"
#include "MathManager.h"
#include "CollisionManager.h"
#include "LifeBar.h"
#define SPEED 2
Enemy::Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf) 
{
	m_velocity = { SPEED,SPEED };
	m_angle = 0;
	UIList.push_back(new LifeBar);
}

void Enemy::Update(std::vector<Tile*> obstacles)
{
	static int timer = 0;
	
	switch (m_animationState)
	{
	case IDLE:
		if (timer > 0)
		{
			setAnimationState(RUNNING);
			timer = 0;
		}
		break;
	case RUNNING:
		if (timer < 300)
		{
			if (!COMA::AABBCollisionWithTiles(this, obstacles))
			{
				m_dst.x = m_collisionBox.x;
				m_dst.y = m_collisionBox.y;
				m_collisionBox.x += m_velocity.x * cos(m_angle * M_PI / 180);
				m_collisionBox.y += m_velocity.y * sin(m_angle * M_PI / 180);
			}
			else
			{
				/*m_collisionBox.x -= m_velocity.x * cos(m_angle * M_PI / 180);
				m_collisionBox.y -= m_velocity.y * sin(m_angle * M_PI / 180);*/
				m_angle += 5;
			}
			
		}
		else 
		{
			setAnimationState(IDLE);
			timer = 0;
		}
		break;
	case MELEE:
		break;
	case SHOOTING:
		break;
	default:
		break;
	}
	timer++;
	for (auto s : UIList)
	{
		s->update(this);
	}
}

void Enemy::Render()
{
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, SDL_FLIP_NONE);
	for (auto s : UIList)
	{
		s->draw();
	}
}

void Enemy::setAnimationState(AnimationState state)
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

void Enemy::drawLOS()
{
	SDL_Point PlayerPosition = { m_dst.x + m_dst.w * 0.5 ,m_dst.y + m_dst.h * 0.5 };
	SDL_Point direction = { cos(m_angle * M_PI / 180) * 800, sin(m_angle * M_PI / 180) * 800 };
	SDL_Point EndPosition = { PlayerPosition.x + direction.x, PlayerPosition.y + direction.y };
	DEMA::DrawLine(PlayerPosition, EndPosition, { 255,255,255,255 });
}

void Enemy::drawRadius()
{
	DEMA::DrawCircle(m_dst.x + m_dst.w * 0.5, m_dst.y + m_dst.h * 0.5, 200);
}
