#include "Player.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "LifeBar.h"
#include "DebugManager.h"
#define SPEED 2


Player::Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf), m_dir(0) {
	UIList.push_back(new LifeBar());
}


void Player::Update(std::array<std::array<Tile*, COLS>, ROWS> m_level)
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
		if (EVMA::KeyHeld(SDL_SCANCODE_W))
		{
			if (m_dst.y > 0 && !COMA::PlayerCollision({ (int)m_dst.x, (int)(m_dst.y), (int)32, (int)32 }, 0, -SPEED, m_level))
			{
				m_dst.y += -SPEED;
			}
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_S))
		{
			if (m_dst.y < 768 - 32 && !COMA::PlayerCollision({ (int)m_dst.x, (int)(m_dst.y), (int)32, (int)32 }, 0, SPEED, m_level))
			{
				m_dst.y += SPEED;
			}
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			if (m_dst.x > 0  && !COMA::PlayerCollision({ (int)m_dst.x, (int)m_dst.y, (int)32, (int)32 }, -SPEED, 0, m_level))
			{
				m_dst.x += -SPEED;
			}
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			if (m_dst.x < 1024 - 32 &&  !COMA::PlayerCollision({ (int)m_dst.x, (int)m_dst.y, (int)32, (int)32 }, SPEED, 0, m_level))
			{
				m_dst.x += SPEED;
			}
		}
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


void Player::setAnimationState(AnimationState state)
{
	m_animationState = state;
	switch (m_animationState)
	{
	case IDLE:
		m_src = { 0 , 0 , 253,216 };
		m_spriteMax = 19;
		m_sprite = 0;
		break;
	case RUNNING:
		m_src = { 0 , 474,260,222 };
		m_spriteMax = 19;
		m_sprite = 0;
		break;
	case MELEE:
		m_src = { 0,216,293,258 };
		m_spriteMax = 14;
		m_sprite = 0;
		break;
	case SHOOTING:
		m_src = { 0,696,257,217 };
		m_spriteMax = 3;
		m_sprite = 0;
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

