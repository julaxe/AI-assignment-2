#include "Player.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "LifeBar.h"
#include "DebugManager.h"
#include "LevelManager.h"
#include "Engine.h"
#include "Bullet.h"

#define SPEED 2


Player::Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)

	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf) {
	UIList.push_back(new LifeBar());
	m_velocity = { SPEED, SPEED };
	m_scale = 0.30f;
	setAnimationState(IDLE);
}


void Player::Update()
{

	
	
	switch (m_animationState)
	{
	case IDLE:
		if (EVMA::KeyHeld(SDL_SCANCODE_W) || EVMA::KeyHeld(SDL_SCANCODE_S) ||
			EVMA::KeyHeld(SDL_SCANCODE_A) || EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_isMoving = true;
			setAnimationState(RUNNING);
		}
		break;
	case RUNNING:
		
		if (EVMA::KeyReleased(SDL_SCANCODE_W) || EVMA::KeyReleased(SDL_SCANCODE_S) ||
			EVMA::KeyReleased(SDL_SCANCODE_A) || EVMA::KeyReleased(SDL_SCANCODE_D))
		{
			setAnimationState(IDLE);
			m_isMoving = false;
			break; // Skip movement parsing below.
		}
		if (!COMA::AABBCollisionWithTiles(this, LevelManager::m_obstacles))
		{
			m_dst.x = m_collisionBox.x;
			m_dst.y = m_collisionBox.y;
			if (EVMA::KeyHeld(SDL_SCANCODE_W))
			{
				m_collisionBox.y += -SPEED;
			}
			else if (EVMA::KeyHeld(SDL_SCANCODE_S))
			{
				m_collisionBox.y += SPEED;
			}
			if (EVMA::KeyHeld(SDL_SCANCODE_A))
			{
				m_collisionBox.x += -SPEED;
			}
			else if (EVMA::KeyHeld(SDL_SCANCODE_D))
			{
				m_collisionBox.x += SPEED;
			}
		}
		
		break;
	case MELEE:
		if (m_animationDone)
		{
			m_animationDone = false;
			setAnimationState(IDLE);
		}
		break;
	case SHOOTING:
		if (m_animationDone)
		{
			m_animationDone = false;
			setAnimationState(IDLE);
		}
		break;
	default:
		break;
	}
	
	Animate();
	for (auto s : UIList)
	{
		s->update(this);
	}
}

void Player::Render()
{

	SDL_Point mousePos = EVMA::GetMousePos();
	m_angle = -90 + MAMA::AngleBetweenPoints(-mousePos.x + (m_dst.x + m_dst.w*0.5), -mousePos.y + (m_dst.y + m_dst.h * 0.5))*-180/M_PI;
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle , 0, SDL_FLIP_NONE);

	for (auto s : UIList)
	{
		s->draw();
	}
}


void Player::Shoot()
{
	SDL_Texture* BulletTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/bullet1.png");
	Bullet* m_shoot = new Bullet({ 0,0,813,306 }, { 0,0,20,8 }, Engine::Instance().GetRenderer(), BulletTexture, 0, false);
	m_shoot->Execute(m_collisionBox, m_angle);
	delete m_shoot; //don't need this anymore till we press shoot again.
	m_shoot = nullptr;

}

void Player::setAnimationState(AnimationState state)
{
	m_animationState = state;
	switch (m_animationState)
	{
	case IDLE:
		m_src = { 0 , 0 , 253,216 };
		m_dst.w = 253*m_scale;
		m_dst.h = 216 * m_scale;
		m_spriteMax = 19;
		m_sprite = 0;
		m_frameMax = 4;
		break;
	case RUNNING:
		m_src = { 0 , 474,260,222 };
		m_dst.w = 260 * m_scale;
		m_dst.h = 222 * m_scale;
		m_spriteMax = 19;
		m_sprite = 0;
		m_frameMax = 4;
		break;
	case MELEE:
		m_src = { 0,216,293,258 };
		m_dst.w = 293 * m_scale;
		m_dst.h = 258 * m_scale;
		m_spriteMax = 7;
		m_sprite = 0;
		m_frameMax = 3;
		break;
	case SHOOTING:
		m_src = { 0,696,257,217 };
		m_dst.w = 257 * m_scale;
		m_dst.h = 217 * m_scale;
		m_spriteMax = 2;
		m_sprite = 0;
		m_frameMax = 4;
		break;
	default:
		break;
	}
}

void Player::drawLOS()
{
	SDL_Point PlayerPosition = { m_dst.x + m_dst.w * 0.5 ,m_dst.y + m_dst.h * 0.5 };
	SDL_Point direction = { cos(m_angle*M_PI/180)*800, sin(m_angle * M_PI / 180)*800};
	SDL_Point EndPosition = {PlayerPosition.x + direction.x, PlayerPosition.y + direction.y};
	DEMA::DrawLine(PlayerPosition, EndPosition, { 255,255,255,255});
}



