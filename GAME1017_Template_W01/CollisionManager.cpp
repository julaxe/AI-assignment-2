#include "CollisionManager.h"
#include "DebugManager.h"
#include "Engine.h"
#include "StateManager.h"
#include <array>
#include "LevelManager.h"
#include "DisplayManager.h"


bool CollisionManager::AABBCheck(const SDL_FRect& object1, const SDL_FRect& object2)
{
	SDL_Rect temp1 = MAMA::ConvertFRect2Rect(object1);
	SDL_Rect temp2 = MAMA::ConvertFRect2Rect(object2);
	return SDL_HasIntersection(&temp1, &temp2);
}

bool CollisionManager::AABBCheck2(const SDL_FRect& object1, const SDL_FRect& object2)
{
	// prepare relevant variables
	SDL_Point p1 = { object1.x, object1.y }; //collision box to the top corner
	SDL_Point p2 = { object2.x, object2.y };
	const float p1Width = object1.w;
	const float p1Height = object1.h;
	const float p2Width = object2.w;
	const float p2Height = object2.h;

	if (
		p1.x < p2.x + p2Width &&
		p1.x + p1Width > p2.x &&
		p1.y < p2.y + p2Height &&
		p1.y + p1Height > p2.y
		)
	{

		return true;
	}
	return false;
}


bool CollisionManager::CircleCircleCheck(const SDL_FPoint object1, const SDL_FPoint object2, const double r1, const double r2)
{
	return (MAMA::Distance((double)object1.x, (double)object2.x, (double)object1.y, (double)object2.y) < (r1+r2));
}

bool CollisionManager::CircleAABBCheck(const SDL_FPoint object1, const double r, const SDL_FRect& object2)
{
	double x1 = (double)object1.x;
	double y1 = (double)object1.y;
	if (object1.x < object2.x) // Circle center, rect left edge.
		x1 = (double)object2.x;
	else if (object1.x > object2.x + object2.w)
		x1 = (double)object2.x + (double)object2.w;
	if (object1.y < object2.y)
		y1 = (double)object2.y;
	else if (object1.y > object2.y + object2.h)
		y1 = (double)object2.y + (double)object2.h;

	return CircleCircleCheck({(float)x1, (float)y1}, {(float)object1.x, (float)object1.y}, r);
}

bool CollisionManager::LinePointCheck(const SDL_FPoint object1_start, const SDL_FPoint object1_end, const SDL_FPoint object2)
{
	double distToStart = MAMA::Distance((double)object1_start.x, (double)object2.x, (double)object1_start.y, (double)object2.y);
	double distToEnd = MAMA::Distance((double)object1_end.x, (double)object2.x, (double)object1_end.y, (double)object2.y);

	double lineLength = MAMA::Distance((double)object1_start.x, (double)object1_end.x, (double)object1_start.y, (double)object1_end.y);

	double buffer = 0.2; // Extra distance since line thickness is one pixel.
	
	if (distToStart + distToEnd <= lineLength + buffer)
		return true;
	return false;
}


bool CollisionManager::AABBCollisionWithTilesUpdating(Sprite* obj1, std::vector<Tile*> obstacles)
{
	for (auto obj2 : obstacles)
	{
		// prepare relevant variables
		SDL_Point p1 = { obj1->GetCollisionBox()->x, obj1->GetCollisionBox()->y }; //collision box to the top corner
		SDL_Point p2 = { obj2->GetCollisionBox()->x, obj2->GetCollisionBox()->y };
		const float p1Width = obj1->GetCollisionBox()->w;
		const float p1Height = obj1->GetCollisionBox()->h;
		const float p2Width = obj2->GetCollisionBox()->w;
		const float p2Height = obj2->GetCollisionBox()->h;

		int angle;

		if (
			p1.x < p2.x + p2Width &&
			p1.x + p1Width > p2.x &&
			p1.y < p2.y + p2Height &&
			p1.y + p1Height > p2.y
			)
		{
			angle = MAMA::AngleBetweenPoints(p1, p2) * 180 / M_PI;
			if (angle > 45 && angle <= 135) //p2 is under p1
			{
				obj1->GetCollisionBox()->y -= obj1->GetVelocity().y;
				//object1->m_boundHit = BOTTOMBOUNDARY; //BOTTOMBOUNDARY
			}
			if (angle > -135 && angle <= -45) //p2 is on top of p1
			{
				obj1->GetCollisionBox()->y += obj1->GetVelocity().y;
				//object1->m_boundHit = TOPBOUNDARY; //TOPBOUNDARY
			}
			if (angle > -45 && angle <= 45) //p2 at the right of p1
			{
				obj1->GetCollisionBox()->x -= obj1->GetVelocity().x;
				//object1->m_boundHit = RIGHTBOUNDARY; //RIGHTBOUNDARY
			}
			if (angle > 135 || angle <= -135) //p2 at the left of p1
			{
				obj1->GetCollisionBox()->x += obj1->GetVelocity().x;
				//object1->m_boundHit = LEFTBOUNDARY; //LEFTBOUNDARY
			}

			return true;
		}
	}
	return false;
		
}
bool CollisionManager::AABBCollisionWithTiles(Sprite* obj1, std::vector<Sprite*> obstacles)
{
	for (auto obj2 : obstacles)
	{
		// prepare relevant variables
		SDL_Point p1 = { obj1->GetCollisionBox()->x, obj1->GetCollisionBox()->y }; //collision box to the top corner
		SDL_Point p2 = { obj2->GetCollisionBox()->x, obj2->GetCollisionBox()->y };
		const float p1Width = obj1->GetCollisionBox()->w;
		const float p1Height = obj1->GetCollisionBox()->h;
		const float p2Width = obj2->GetCollisionBox()->w;
		const float p2Height = obj2->GetCollisionBox()->h;

		if (
			p1.x < p2.x + p2Width &&
			p1.x + p1Width > p2.x &&
			p1.y < p2.y + p2Height &&
			p1.y + p1Height > p2.y
			)
		{

			return true;
		}
	}
	return false;

}

bool CollisionManager::LineRectCheck(const SDL_Point obj1, const SDL_FRect obj2)
{
	if (obj1.x >= obj2.x && obj1.x <= obj2.x + obj2.w &&
		obj1.y >= obj2.y && obj1.y <= obj2.y + obj2.h)
	{
		return true;
	}
	return false;
}

bool CollisionManager::CheckLOS(Sprite* obj1, std::vector<Sprite*> list /*= enemy or player*/)
{
	SDL_Point end = obj1->getLOSendPosition();
	int iterations = 500;
	for (int i = 1; i <= iterations; i++)
	{
		double factor = (double)i / (double)iterations;
		int dx = MAMA::LerpD(obj1->getPosition().x, end.x, factor);
		int dy = MAMA::LerpD(obj1->getPosition().y, end.y, factor);
		obj1->getLOSendPosition() = { dx, dy };
		if (LevelManager::m_level[dy / LevelManager::SIZEOFTILES][dx / LevelManager::SIZEOFTILES]->IsObstacle())
		{
			return false;
		}
		for (auto d : DisplayManager::DestructableObjList())
		{
			if (LineRectCheck(obj1->getLOSendPosition(), *(d->GetCollisionBox())))
			{
				return false;
			}
		}
		for (auto e : list)
		{
			if (LineRectCheck(obj1->getLOSendPosition(), *(e->GetCollisionBox())))
			{
				return true;
			}
		}
	}
	return false;
}

bool CollisionManager::CheckLOSInsideRadius(Sprite* obj1, std::vector<Sprite*> list /*= enemy or player*/)
{
	SDL_Point end = obj1->getLOSRadiusendPosition();
	int iterations = 500;
	for (int i = 1; i <= iterations; i++)
	{
		double factor = (double)i / (double)iterations;
		int dx = MAMA::LerpD(obj1->getPosition().x, end.x, factor);
		int dy = MAMA::LerpD(obj1->getPosition().y, end.y, factor);
		obj1->getLOSRadiusendPosition() = { dx, dy };
		if (LevelManager::m_level[dy / LevelManager::SIZEOFTILES][dx / LevelManager::SIZEOFTILES]->IsObstacle())
		{
			return false;
		}
		for (auto d : DisplayManager::DestructableObjList())
		{
			if (LineRectCheck(obj1->getLOSRadiusendPosition(), *(d->GetCollisionBox())))
			{
				return false;
			}
		}
		for (auto e : list)
		{
			if (LineRectCheck(obj1->getLOSRadiusendPosition(), *(e->GetCollisionBox())))
			{
				return true;
			}
		}
	}
	return false;
}
bool CollisionManager::CheckLOS(PathNode* obj1, std::vector<Sprite*> list /*= enemy or player*/)
{
	SDL_Point end = obj1->getLOSendPosition();
	int iterations = 500;
	for (int i = 1; i <= iterations; i++)
	{
		double factor = (double)i / (double)iterations;
		int dx = MAMA::LerpD(obj1->x, end.x, factor);
		int dy = MAMA::LerpD(obj1->y, end.y, factor);
		obj1->getLOSendPosition() = { dx, dy };
		if (LevelManager::m_level[dy / LevelManager::SIZEOFTILES][dx / LevelManager::SIZEOFTILES]->IsObstacle())
		{
			return false;
		}
		for (auto d : DisplayManager::DestructableObjList())
		{
			if (LineRectCheck(obj1->getLOSendPosition(), *(d->GetCollisionBox())))
			{
				return false;
			}
		}
		for (auto e : list)
		{
			if (LineRectCheck(obj1->getLOSendPosition(), *(e->GetCollisionBox())))
			{
				return true;
			}
		}
	}
	return false;
}
