#include "Player.h"
#include "CollisionManager.h"
#include "EventManager.h"
#define SPEED 2

Player::Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf), m_state(idle), m_dir(0) {}

void Player::Update(std::array<std::array<Tile*, COLS>, ROWS> m_level)
{
	switch (m_state)
	{
	case idle:
		if (EVMA::KeyHeld(SDL_SCANCODE_W) || EVMA::KeyHeld(SDL_SCANCODE_S) ||
			EVMA::KeyHeld(SDL_SCANCODE_A) || EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			SetState(running);
		}
		break;
	case running:
		if (EVMA::KeyReleased(SDL_SCANCODE_W) || EVMA::KeyReleased(SDL_SCANCODE_S) ||
			EVMA::KeyReleased(SDL_SCANCODE_A) || EVMA::KeyReleased(SDL_SCANCODE_D))
		{
			SetState(idle);
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
				m_dir = 1;
			}
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			if (m_dst.x < 1024 - 32 &&  !COMA::PlayerCollision({ (int)m_dst.x, (int)m_dst.y, (int)32, (int)32 }, SPEED, 0, m_level))
			{
				m_dst.x += SPEED;
				m_dir = 0;
			}
		}
		break;
	}
	Animate();
}

void Player::Render()
{
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, static_cast<SDL_RendererFlip>(m_dir));
}

bool Player::Move(SDL_Point p)
{
	m_velocity = { 2,2 };
	if (m_dst.x != p.x || m_dst.y != p.y)
	{
		if (m_dst.x > p.x)
		{
			m_velocity.x *= -1;
			m_dir = 1;
		}
		else if (m_dst.x == p.x) m_velocity.x = 0;
		else m_dir = 0;

		if (m_dst.y > p.y) m_velocity.y *= -1;
		else if (m_dst.y == p.y) m_velocity.y = 0;
	}
	else
	{
		SetState(idle);
		return true;
	}

	m_dst.x += m_velocity.x;
	m_dst.y += m_velocity.y;
	if(m_state != running)
		SetState(running);
	return false;
}

void Player::SetState(int s)
{
	m_state = static_cast<state>(s);
	m_frame = 0;
	if (m_state == idle)
	{
		m_sprite = m_spriteMin = m_spriteMax = 0;
	}
	else // Only other is running for now...
	{
		m_sprite = m_spriteMin = 1;
		m_spriteMax = 4;
	}
}
