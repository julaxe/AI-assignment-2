#pragma once
#include "Button.h"
class EscapeButton : public Button {
private:
public:
	EscapeButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t) : Button(src, dst, r, t) {}
	void Execute();
};