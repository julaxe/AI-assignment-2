#pragma once
#include "Label.h"
#include "States.h"
#include "PlayButton.h"

class StartScene : public State {
private:
	//Sprite* m_pSprite;
	PlayButton* m_pPlayButton;
	Label* m_pFont;
	Label* m_pFont2;
public:
	StartScene();
	void Update();
	void Render();
	void Enter();
	void Exit();
	void Resume();
};