#include "Player.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "LifeBar.h"
#include "DebugManager.h"
#include "LevelManager.h"
#include "Engine.h"
#include "Bullet.h"
#include "MeleeAtk.h"
#include "SoundManager.h"
#include "DisplayManager.h"

#define SPEED 2


Player::Player(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)

	:AnimatedSprite(s, d, r, t, sstart, smin, smax, nf) {
	UIList.push_back(new LifeBar());
	m_velocity = { SPEED, SPEED };
	m_scale = 0.30f;
	m_Life = 100;
	setAnimationState(IDLE);
	
}


void Player::update()
{
	updatePosition();
	updateCollisionBox(40.0f, 40.0f);
	if (updateLOS(DisplayManager::EnemiesList()) && !LOSalert)
	{
		LOSalert = true;
		std::cout << "Enemy in LOS" << std::endl;
	}
	else if (!updateLOS(DisplayManager::EnemiesList()) && LOSalert)
	{
		LOSalert = false;
		std::cout << "Enemy OUT of LOS" << std::endl;
	}
	
	switch (m_animationState)
	{
	case IDLE:
		if (EVMA::KeyHeld(SDL_SCANCODE_W) || EVMA::KeyHeld(SDL_SCANCODE_S) ||
			EVMA::KeyHeld(SDL_SCANCODE_A) || EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_isMoving = true;
			setAnimationState(RUNNING);
		}
		break;
	case RUNNING:
		
		if (EVMA::KeyReleased(SDL_SCANCODE_W) || EVMA::KeyReleased(SDL_SCANCODE_S) ||
			EVMA::KeyReleased(SDL_SCANCODE_A) || EVMA::KeyReleased(SDL_SCANCODE_D))
		{
			setAnimationState(IDLE);
			m_isMoving = false;
			break; // Skip movement parsing below.
		}
		SOMA::PlaySound("running", 0, 0);
		//m_dst.x = m_collisionBox.x;
		//m_dst.y = m_collisionBox.y;
		if (EVMA::KeyHeld(SDL_SCANCODE_W))
		{
			Move(0, -SPEED);
			if (COMA::AABBCollisionWithTiles(this, LevelManager::m_obstacles))
				Move(0, SPEED);
				
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_S))
		{
			Move(0, SPEED);
			if (COMA::AABBCollisionWithTiles(this, LevelManager::m_obstacles))
				Move(0, -SPEED);
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			Move(-SPEED, 0);
			if (COMA::AABBCollisionWithTiles(this, LevelManager::m_obstacles))
				Move(SPEED, 0);
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			Move(SPEED, 0);
			if (COMA::AABBCollisionWithTiles(this, LevelManager::m_obstacles))
				Move(-SPEED, 0);
		}
		
		break;
	case MELEE:
		if (m_animationDone)
		{
			m_animationDone = false;
			setAnimationState(IDLE);
		}
		break;
	case SHOOTING:
		if (m_animationDone)
		{
			m_animationDone = false;
			setAnimationState(IDLE);
		}
		break;
	default:
		break;
	}
	
	Animate();
	for (auto s : UIList)
	{
		s->update(this);
	}
}

void Player::Render()
{

	SDL_Point mousePos = EVMA::GetMousePos();
	m_angle = MAMA::AngleBetweenPoints(mousePos.y - (m_dst.y + m_dst.h * 0.5), mousePos.x - (m_dst.x + m_dst.w * 0.5));
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle * 180 / M_PI , 0, SDL_FLIP_NONE);

	for (auto s : UIList)
	{
		s->draw();
	}
}


void Player::Shoot()
{
	SDL_Texture* BulletTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/bullet1.png");
	Bullet* m_shoot = new Bullet({ 0,0,813,306 }, { 0,0,20,8 }, Engine::Instance().GetRenderer(), BulletTexture, 0, false);
	m_shoot->Execute(m_collisionBox, m_angle * 180 / M_PI);
	delete m_shoot; //don't need this anymore till we press shoot again.
	m_shoot = nullptr;

}
void Player::MeleeAttack()
{
	SDL_Texture* MeleeTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/Empty.png");
	MeleeAtk* m_melee = new MeleeAtk({ 0,0,32,32 }, { 0,0,32,32 }, Engine::Instance().GetRenderer(), MeleeTexture, 0, false);
	m_melee->Execute(m_collisionBox, m_angle * 180 / M_PI);
	delete m_melee; //don't need this anymore till we press shoot again.
	m_melee = nullptr;
}

void Player::setAnimationState(AnimationState state)
{
	m_animationState = state;
	switch (m_animationState)
	{
	case IDLE:
		m_src = { 0 , 0 , 253,216 };
		m_dst.w = 253*m_scale;
		m_dst.h = 216 * m_scale;
		m_spriteMax = 19;
		m_sprite = 0;
		m_frameMax = 4;
		break;
	case RUNNING:
		m_src = { 0 , 474,260,222 };
		m_dst.w = 260 * m_scale;
		m_dst.h = 222 * m_scale;
		m_spriteMax = 19;
		m_sprite = 0;
		m_frameMax = 4;
		break;
	case MELEE:
		m_src = { 0,216,293,258 };
		m_dst.w = 293 * m_scale;
		m_dst.h = 258 * m_scale;
		m_spriteMax = 7;
		m_sprite = 0;
		m_frameMax = 3;
		break;
	case SHOOTING:
		m_src = { 0,696,257,217 };
		m_dst.w = 257 * m_scale;
		m_dst.h = 217 * m_scale;
		m_spriteMax = 2;
		m_sprite = 0;
		m_frameMax = 4;
		break;
	default:
		break;
	}
}




