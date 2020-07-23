#pragma once
#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include <iostream>
#include <SDL.h>
#include "MathManager.h"
#include "Tile.h"

class CollisionManager
{
public:
	static bool AABBCheck(const SDL_FRect& object1, const SDL_FRect& object2);

	static bool CircleCircleCheck(const SDL_FPoint object1, const SDL_FPoint object2, const double r1, const double r2 = 0.0);
	static bool CircleAABBCheck(const SDL_FPoint object1, const double r, const SDL_FRect& object2);

	static bool LinePointCheck(const SDL_FPoint object1_start, const SDL_FPoint object1_end, const SDL_FPoint object2);

	static bool AABBCollisionWithTilesUpdating(Sprite* obj1, std::vector<Tile*> obstacles);

	static bool AABBCollisionWithTiles(Sprite* obj1, std::vector<Tile*> obstacles);

	static SDL_FPoint CheckLOS(SDL_FPoint start, SDL_FPoint end, SDL_FRect obj);
private:
	CollisionManager() {}
};

typedef CollisionManager COMA;

#endif