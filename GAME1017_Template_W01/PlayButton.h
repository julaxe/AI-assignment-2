#pragma once
#include"Button.h"

class PlayButton : public Button
{
public:
	PlayButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t) : Button(src, dst, r, t) {};
	void Execute();

};
