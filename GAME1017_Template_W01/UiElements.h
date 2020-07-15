#pragma once
#ifndef __UI_ELEMENT__
#define __UI_ELEMENT__

#include "Sprite.h"
#include <vector>



class UIElements
{
public:
	explicit UIElements();
	~UIElements();

	// Inherited via GameObject
	virtual void draw();
	virtual void update(Sprite* player);
	virtual void clean();


private:

};
#endif
