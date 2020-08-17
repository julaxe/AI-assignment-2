#pragma once
#include "Button.h"
class RestartButton : public Button {
private:
public:
	RestartButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t) : Button(src, dst, r, t) {};
	void Execute();
};