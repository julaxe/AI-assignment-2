#pragma once
#ifndef _LABEL_H_
#define _LABEL_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Label
{
public: // Public methods.
	Label(std::string key, const float x, const float y, const char* str, const SDL_Color col = { 255,255,255,255 });
	~Label();
	virtual void Render();
	virtual void update();
	virtual void SetText(const char* c);
	virtual void SetPos(const float x, const float y);
	virtual void SetColor(const SDL_Color& col);
	virtual void UseFont(std::string key);

protected: // Private properties.
	TTF_Font* m_Font;
	SDL_Color m_TextColor;
	SDL_FRect m_rTextRect;
	SDL_Texture* m_pTexture;
	std::string str;
	char m_String[256];

};

#endif