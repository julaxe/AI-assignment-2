#pragma once
#ifndef _LABEL_EA_
#define _LABEL_EA_

#include "Label.h"


class LabelEnemiesAlive : public Label
{
public: // Public methods.
	LabelEnemiesAlive(std::string key, const float x, const float y, const char* str, const SDL_Color col = { 255,255,255,255 }) : Label(key, x, y, str, col) {};
	void Update();
	

private: // Private properties.

};

#endif
#pragma once
