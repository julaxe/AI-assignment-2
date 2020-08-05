#pragma once
#ifndef __DESTUCTABLE__
#define __DESTUCTABLE__

#include "Sprite.h"
#include <string>

class DestructableObj : public Sprite
{
public:
	DestructableObj(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int m_numberofHits);
	~DestructableObj();
	void Render() override;
	void update() override;
private:
	void updateCollision();
	int m_numOfHits;
	SDL_Rect src;
	SDL_FRect dst;
};



#endif // !__DESTUCTABLE__
