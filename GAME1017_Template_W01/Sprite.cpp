#include "Sprite.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "DisplayManager.h"
#include "LevelManager.h"
#include "Engine.h"

void Sprite::MoveUpdate(float speedX, float speedY)
{
	m_dst.y += speedY;
	m_dst.x += speedX;
	m_collisionBox.y += speedY;
	m_collisionBox.x += speedX;
}

void Sprite::Move(float speedX, float speedY)
{
	MoveUpdate(speedX, 0);
	if (COMA::AABBCollisionWithTiles(this, LevelManager::m_obstacles) || COMA::AABBCollisionWithTiles(this, DisplayManager::DestructableObjList()))
		MoveUpdate(-speedX, 0);

	MoveUpdate(0, speedY);
	if (COMA::AABBCollisionWithTiles(this, LevelManager::m_obstacles) || COMA::AABBCollisionWithTiles(this, DisplayManager::DestructableObjList()))
		MoveUpdate(0, -speedY);
}
void Sprite::MovePlayer(float speedX, float speedY)
{
	MoveUpdate(speedX, 0);
	if (COMA::AABBCollisionWithTiles(this, LevelManager::m_obstacles) || COMA::AABBCollisionWithTiles(this, DisplayManager::DestructableObjList()) || checkLevelBorders())
		MoveUpdate(-speedX, 0);

	MoveUpdate(0, speedY);
	if (COMA::AABBCollisionWithTiles(this, LevelManager::m_obstacles) || COMA::AABBCollisionWithTiles(this, DisplayManager::DestructableObjList()) || checkLevelBorders())
		MoveUpdate(0, -speedY);
}

bool Sprite::checkLevelBorders()
{
	if (GetCollisionBox()->x + GetCollisionBox()->w > WIDTH)
	{
		return true;
	}
	return false;
}

void Sprite::updateAngleWithMouse()
{
	SDL_Point mousePos = EVMA::GetMousePos();
	m_angle = MAMA::AngleBetweenPoints(mousePos.y - (m_dst.y + m_dst.h * 0.5), mousePos.x - (m_dst.x + m_dst.w * 0.5));
}

bool Sprite::OnTopOfNodeWLOS()
{
	bool onTop = false;
	for (auto n : LevelManager::m_nodes)
	{
		if (n->Node()->inLOS())
		{
			SDL_FRect collisionBoxNode = { (float)n->Node()->Pt().x , (float)n->Node()->Pt().y, 5,5 };
			if (COMA::AABBCheck(collisionBoxNode, *GetCollisionBox()))
				onTop = true;
		}
	}
	return onTop;
}

void Sprite::drawLOS()
{
	if (m_inLOS)
	{
		DEMA::DrawLine(m_pos, LOSendPosition, { 255,20,20,255 });
	}
	else
	{
		DEMA::DrawLine(m_pos, LOSendPosition, { 255,255,255,255 });
	}
}

void Sprite::drawRadius(int radius)
{
	if (m_inRadius)
	{
		DEMA::DrawCircle(m_dst.x + m_dst.w * 0.5, m_dst.y + m_dst.h * 0.5, radius, { 255,20,20,255 });
	}
	else
	{
		DEMA::DrawCircle(m_dst.x + m_dst.w * 0.5, m_dst.y + m_dst.h * 0.5, radius, { 255,255,255,255 });
	}
}

bool Sprite::updateLOS(std::vector<Sprite*> list)
{
	bool changeLOS = false;
	SDL_Point direction = { cos(m_angle) * 2000, sin(m_angle) * 2000 };
	LOSendPosition = { (int)m_pos.x + direction.x, (int)m_pos.y + direction.y };
	if (COMA::CheckLOS(this, list))
	{
		if (!m_inLOS) 
			changeLOS = true;
		m_inLOS = true;
		return changeLOS;
	}
	m_inLOS = false;
	return false;
}
bool Sprite::updateRAD()
{
	bool changeRad = false;
	if (COMA::CircleCircleCheck(getPosition(), DisplayManager::PlayerList()[0]->getPosition(), 200, 20))
	{
		if(!m_inRadius)
			changeRad= true;
		m_inRadius = true;
		return changeRad;
	}
	m_inRadius = false;
	return false;
}
bool Sprite::updateLOSToPlayer()
{
	double dy = DisplayManager::PlayerList()[0]->getPosition().y - m_pos.y;
	double dx = DisplayManager::PlayerList()[0]->getPosition().x - m_pos.x;
	double angle = MAMA::AngleBetweenPoints(dy, dx);
	SDL_Point direction = { cos(angle) * 2000, sin(angle) * 2000 };
	LOSendPosition = { (int)m_pos.x + direction.x, (int)m_pos.y + direction.y };
	if (COMA::CheckLOS(this, DisplayManager::PlayerList()))
	{
		m_inLOS = true;
		return true;
	}
	m_inLOS = false;
	return false;
}

bool Sprite::updateLOSToPlayerInsideRadius()
{
	double dy = DisplayManager::PlayerList()[0]->getPosition().y - m_pos.y;
	double dx = DisplayManager::PlayerList()[0]->getPosition().x - m_pos.x;
	double angle = MAMA::AngleBetweenPoints(dy, dx);
	SDL_Point direction = { cos(angle) * 2000, sin(angle) * 2000 };
	LOSRadiusendPosition = { (int)m_pos.x + direction.x, (int)m_pos.y + direction.y };
	if (COMA::CheckLOSInsideRadius(this, DisplayManager::PlayerList()))
	{
		m_inLOSInRadius = true;
		return true;
	}
	m_inLOSInRadius = false;
	return false;
}

