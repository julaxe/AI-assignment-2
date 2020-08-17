#include "VictoryState.h"
#include "TextureManager.h"
#include "Engine.h"
#include "FontManager.h"

VictoryState::VictoryState()
{
	TextureManager::RegisterTexture("Img/buttons_map.png", "restart");
	TextureManager::RegisterTexture("Img/buttons_map.png", "escape");
	m_pRestart = new RestartButton({ 898,336,67,67 }, { 300,400,67,67 }, Engine::Instance().GetRenderer(), TextureManager::GetTexture("restart"));
	m_pEscape = new EscapeButton({ 0,464,67,67 }, { 400,400,67,67 }, Engine::Instance().GetRenderer(), TextureManager::GetTexture("escape"));
	FontManager::RegisterFont("Img/8-BIT WONDER.TTF", "title", 80);
	m_pGameOver = new Label("title", 200, 200, "VICTORY");
}

void VictoryState::HandleEvents()
{
}

void VictoryState::Update()
{
	if (m_pRestart->Update())
		return;
	m_pEscape->Update();
}

void VictoryState::Render()
{
	m_pGameOver->Render();
	m_pRestart->Render();
	m_pEscape->Render();
}

void VictoryState::Enter()
{
}

void VictoryState::Exit()
{
}

void VictoryState::Resume()
{
}

