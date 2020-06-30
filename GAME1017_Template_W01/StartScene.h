#pragma once
#include "Label.h"
#include "States.h"
#include "PlayButton.h"

class StartScene : public State {
public:
	StartScene();
	void HandleEvents() override;
	void Update() override;
	void Render() override;
	void Enter() override;
	void Exit() override;
	void Resume() override;
private:
	Sprite* m_pSprite;
	PlayButton* m_pPlayButton;
	Label* m_pFont;
	Label* m_pFont2;
};