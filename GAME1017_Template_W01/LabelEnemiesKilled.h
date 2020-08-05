#pragma once
#ifndef _LABEL_EK_
#define _LABEL_EK_

#include "Label.h"


class LabelEnemiesKilled : public Label
{
public: // Public methods.
	LabelEnemiesKilled(std::string key, const float x, const float y, const char* str, const SDL_Color col = { 255,255,255,255 }) : Label(key, x, y, str, col) {};
	void update();
	
private: // Private properties.
	
};

#endif
#pragma once
