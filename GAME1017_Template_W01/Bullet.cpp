#include "Bullet.h"
#include "CollisionManager.h"
#include "DisplayManager.h"
#include "LevelManager.h"
#include "Engine.h"

Bullet::Bullet(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int angle, bool running) : Sprite(s,d,r,t)
{
	m_running = running;
	m_angle = angle * M_PI / 180;
	m_velocity = { 4.0f,4.0f,0,0 };
	timer = 0;
	m_muzzle = false;
}

Bullet::~Bullet()
{
}

void Bullet::update()
{
	if (m_running)
	{
		m_dst.x += m_velocity.x * cos(m_angle);
		m_dst.y += m_velocity.y * sin(m_angle);

		m_collisionBox = m_dst;
		m_collisionBox.h = 10;
		m_collisionBox.w = 10;

		if (CollisionManager::AABBCollisionWithTiles(this, LevelManager::m_obstacles))
		{
			m_running = false;
		}
	}
}

void Bullet::Render()
{
	
	if (m_running)
	{
		SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle*180/M_PI, 0, SDL_FLIP_NONE);
		/*if (!m_muzzle)
		{
			timer++;
			SDL_Texture* muzzleText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/muzzle_flash_01.png");
			SDL_Rect* srcMuzzle = new SDL_Rect({ 0,0,256,256 });
			SDL_FRect* dstMuzzle = new SDL_FRect({ m_dst.x,m_dst.y,100,100 });
			SDL_RenderCopyExF(m_pRend, muzzleText, srcMuzzle, dstMuzzle, m_angle * 180 / M_PI, 0, SDL_FLIP_NONE);
			if (timer < 3) m_muzzle = true;
		}*/
	}
}

void Bullet::Execute(SDL_FRect PlayerDst, int angle)
{
	//math to put the bullet in front of the gun.
	float PosX = (PlayerDst.x + PlayerDst.w * 0.5) + 0.5*PlayerDst.w*cos(angle*M_PI/180) +  0.2*PlayerDst.w*cos((angle+90)*M_PI/180);
	float PosY = (PlayerDst.y + PlayerDst.h * 0.5) + 0.5*PlayerDst.h*sin(angle * M_PI / 180) + 0.2 * PlayerDst.h * sin((angle + 90) * M_PI / 180); ;
	
	m_dst.x = PosX;
	m_dst.y = PosY;
	
	DisplayManager::AttackList().push_back(new Bullet(m_src, m_dst, m_pRend, m_pText, angle, true));
}
