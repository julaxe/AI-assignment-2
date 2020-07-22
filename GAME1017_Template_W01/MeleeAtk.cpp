#include "MeleeAtk.h"
#include "DisplayManager.h"
MeleeAtk::MeleeAtk(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int angle, bool running) : Sprite(s,d,r,t)
{
	m_running = running;
	m_angle = angle * M_PI / 180;
	timer = 0;
}

MeleeAtk::~MeleeAtk()
{
}

void MeleeAtk::update()
{
	timer++;
	if (timer > 30)
	{
		m_running = false;
	}
}

void MeleeAtk::Render()
{
}

void MeleeAtk::Execute(SDL_FRect PlayerDst, int angle)
{
	//math to put the bullet in front of the gun.
	float PosX = (PlayerDst.x + PlayerDst.w * 0.25) + PlayerDst.w * cos(angle * M_PI / 180) * 1.2;
	float PosY = (PlayerDst.y + PlayerDst.h * 0.25) + PlayerDst.h * sin(angle * M_PI / 180) * 1.2;

	m_dst.x = PosX;
	m_dst.y = PosY;
	
	DisplayManager::AttackList().push_back(new MeleeAtk(m_src, m_dst, m_pRend, m_pText, angle, true));
}
