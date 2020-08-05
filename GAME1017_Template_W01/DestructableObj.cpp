#include "DestructableObj.h"
#include "TextureManager.h"
#include "CollisionManager.h"

DestructableObj::DestructableObj(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int m_numberofHits) : Sprite(s,d,r,t)
{
	this->m_numOfHits = m_numberofHits;
	src = s;

}

DestructableObj::~DestructableObj()
= default;

void DestructableObj::Render()
{
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

	default:
		break;
	}
}

void DestructableObj::updateCollision()
{
	
}
