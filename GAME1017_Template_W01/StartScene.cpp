#include "StartScene.h"
#include "Engine.h"
#include"TextureManager.h"
#include"FontManager.h"

StartScene::StartScene()
{
	
	TextureManager::RegisterTexture("Img/play.png", "playButton");
	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = 400;
	src.h = 100;

	SDL_FRect dst;
	dst.x = WIDTH / 2 - src.w / 2;
	dst.y = 2 * HEIGHT / 3;
	dst.w = src.w;
	dst.h = src.h;

	m_pPlayButton = new PlayButton(src, dst, Engine::Instance().GetRenderer(), TextureManager::GetTexture("playButton"));

	FontManager::RegisterFont("Img/LTYPE.TTF", "font", 20);

	//Labels
	m_pFont = new Label("font", WIDTH / 2 - 75, 10, "Lucas101289546", { 255,255,255,255 });
	m_pFont2 = new Label("font", WIDTH / 2 - 80, 30, "Julian101286557", { 255,255,255,255 });

	//Backgound
	TextureManager::RegisterTexture("Img/Background.jpg", "Background");
	m_pSprite = new Sprite({ 0,0,1920,1080 }, { 0,0,WIDTH,HEIGHT }, Engine::Instance().GetRenderer(), TextureManager::GetTexture("Background"));
}

void StartScene::HandleEvents()
{
}


void StartScene::Update()
{
	m_pPlayButton->Update();
}

void StartScene::Render()
{
	m_pSprite->Render();
	m_pPlayButton->Render();
	m_pFont->Render();
	m_pFont2->Render();
}

void StartScene::Enter()
{
}

void StartScene::Exit()
{
}

void StartScene::Resume()
{
}
