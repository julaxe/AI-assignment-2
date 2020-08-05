#include "DestructableObj.h"
#include "TextureManager.h"
#include "DisplayManager.h"
#include "CollisionManager.h"
#include "EventManager.h"

DestructableObj::DestructableObj(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int m_numberofHits) : Sprite(s,d,r,t)
{
	this->m_numOfHits = m_numberofHits;
	src = s;
	m_angle = 0;
	this->isRunning() = true;
}

DestructableObj::~DestructableObj()
= default;

void DestructableObj::Render()
{
	SDL_RenderCopyExF(m_pRend, m_pText, &src, GetDstP(), m_angle * 180 / M_PI, 0, SDL_FLIP_NONE);
}

void DestructableObj::update()
{
	switch (m_numOfHits)
	{
	case 1:
	{
		src.x = 51;
		break;
	}
	case 2:
	{
		src.x = 102;
		break;
	}
	case 3:
	{
		src.x = 153;
		break;
	}
	case 4:
	{
		isRunning() = false;
		break;
	}

	default:
		break;
	}
}

void DestructableObj::updateCollision()
{
	
}
