#pragma once
#ifndef __LIFE_BAR__
#define __LIFE_BAR__

#include "UIElements.h"

class LifeBar : public UIElements
{
public:
	LifeBar();
	~LifeBar();

	virtual void draw();
	virtual void update(Sprite* player);
	virtual void clean();
private:
	Sprite* m_pLgreen;
	Sprite* m_pLred;
	float greenx;
	float dstx;
	float dsty;
};

#endif