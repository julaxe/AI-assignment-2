#pragma once
#ifndef _SPRITE_H_
#define _SPRITE_H_


#include <iostream>
#include "DebugManager.h"
enum AnimationState {
	IDLE,
	RUNNING,
	MELEE,
	SHOOTING,
	DEATH
};
class Sprite // Inline class.
{
public: // Inherited and public.
	Sprite(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t)
		:m_src(s), m_dst(d), m_pRend(r), m_pText(t), m_angle(0.0), m_collisionBox(d), LOSalert(false)
	{
		updatePosition();
	}
	virtual void Render() {	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, SDL_FLIP_NONE); }
	virtual void update() {}


	SDL_FPoint& getPosition() { return m_pos; }
	SDL_Point& getLOSendPosition() { return LOSendPosition; }
	SDL_Rect* GetSrcP() { return &m_src; }
	SDL_FRect* GetDstP() { return &m_dst; }
	SDL_FRect* GetCollisionBox() { return &m_collisionBox; }
	double& GetAngle() { return m_angle; }
	SDL_FRect& GetVelocity() { return m_velocity; }
	void SetAngle(double a) { m_angle = a; }
	virtual int& getLife() { return m_Life; };
	bool& isRunning() { return m_running; }


	void updateCollisionBox(float width, float heigth) 
		{ m_collisionBox = { (float)m_dst.x + (float)(m_dst.w * 0.5 - width*0.5), (float)m_dst.y + (float)(m_dst.h * 0.5 - heigth*0.5), width, heigth }; }
	void updatePosition() { m_pos = { (float)(m_dst.x + m_dst.w * 0.5),(float)( m_dst.y + m_dst.h * 0.5) }; }
	void Move(int speedX, int speedY);


	void drawLOS();
	void drawRadius(int radius);
	bool updateLOS(std::vector<Sprite*> list);

protected: // Private BUT inherited.
	double m_angle;
	SDL_FPoint m_pos;
	SDL_Rect m_src;
	SDL_FRect m_dst;
	SDL_FRect m_velocity;
	SDL_FRect m_acceleration;
	SDL_FRect m_collisionBox;
	SDL_Renderer* m_pRend;
	SDL_Texture* m_pText;
	SDL_Point LOSendPosition;

	int m_Life;
	bool m_running;

	bool LOSalert;
private: // Private NOT inherited.
};

class AnimatedSprite : public Sprite// Also inline.
{
public:
	AnimatedSprite(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
		:Sprite(s, d, r, t), m_sprite(sstart), m_spriteMin(smin), m_spriteMax(smax), m_frameMax(nf) {}
	void Animate()
	{
		if (m_frame++ >= m_frameMax) // Post-increment ensures m_frame starts at 0.
		{
			m_frame = 0;
			if (m_sprite++ >= m_spriteMax)
			{
				m_sprite = m_spriteMin;
				m_animationDone = true;
			}
		}
		m_src.x = m_src.w * m_sprite;
	}
	virtual void update() = 0;
	virtual void Render() = 0;
	void drawCollisionRect()
	{
		DEMA::DrawRect(m_collisionBox, { 1.0f,1.0f,1.0f,1.0f });
	}
	

protected:
	int m_sprite,		// The current sprite index in row.
		m_spriteMin,	// The sprite index to start animation on.
		m_spriteMax,	// The sprite index to end animation on.
		m_frame = 0,	// Frame counter.
		m_frameMax;		// Number of frames to display each sprite.
	AnimationState m_animationState = IDLE;
	bool m_animationDone = false;
};

/* Note: A lot of animation methods use a modulus operation to wrap the frames
   around, but I am not. I may have different cycles on the same row and so I
   want total control over my animations without having it look too complex. */

#endif