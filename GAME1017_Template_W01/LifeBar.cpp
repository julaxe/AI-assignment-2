#include "LifeBar.h"
#include "TextureManager.h"
#include "Engine.h"

LifeBar::LifeBar()
{
	TextureManager::RegisterTexture ("Img/EnemyLifeBarRed.png", "LifeBarRed");
	TextureManager::RegisterTexture("Img/EnemyLifeBarGreen.png", "LifeBarGreen");
}

LifeBar::~LifeBar()
= default;

void LifeBar::draw()
{
	m_pLred = new Sprite({ 0,0,10,100 }, { dstx, dsty, 100,2 }, Engine::Instance().GetRenderer(), TextureManager::GetTexture("LifeBarRed"));
	m_pLgreen = new Sprite({ 0,0,10,100 }, { dstx, dsty,greenx , 2 }, Engine::Instance().GetRenderer(), TextureManager::GetTexture("LifeBarGreen"));
	m_pLred->Render();
	m_pLgreen->Render();
}

void LifeBar::update(Sprite * a)
{
	dstx = a->GetDstP()->x - 20;
	dsty = a->GetDstP()->y - 10;

	greenx = a->getLife();

}

void LifeBar::clean()
{
}
