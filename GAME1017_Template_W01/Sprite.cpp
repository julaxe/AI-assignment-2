#include "Sprite.h"
#include "CollisionManager.h"

void Sprite::Move(int speedX, int speedY)
{
	m_dst.y += speedY;
	m_collisionBox.y += speedY;
	m_dst.x += speedX;
	m_collisionBox.x += speedX;
}

void Sprite::drawLOS(Sprite* obj)
{
	SDL_Point direction = { cos(m_angle) * 2000, sin(m_angle) * 2000 };
	LOSendPosition = { (int)m_pos.x + direction.x, (int)m_pos.y + direction.y };
	if (COMA::CheckLOS(this, *(obj->GetCollisionBox())))
	{
		std::cout << "LOS!!" << std::endl;
	}
	DEMA::DrawLine(m_pos, LOSendPosition, { 255,255,255,255 });
}

void Sprite::drawRadius(int radius)
{
	DEMA::DrawCircle(m_dst.x + m_dst.w * 0.5, m_dst.y + m_dst.h * 0.5, radius);
}
