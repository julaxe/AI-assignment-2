#pragma once
#include "Label.h"
#include "States.h"
#include "RestartButton.h"
#include "EscapeButton.h"

class VictoryState : public State {
public:
	VictoryState();
	void HandleEvents() override;
	void Update() override;
	void Render() override;
	void Enter() override;
	void Exit() override;
	void Resume() override;
private:
	Label* m_pGameOver;
	RestartButton* m_pRestart;
	EscapeButton* m_pEscape;
};