#include "PlayState.h"
#include "Engine.h"
#include "FontManager.h"
#include <iostream>
#include <fstream>
#include "MathManager.h"
#include "EventManager.h"
#include "PathManager.h"
#include "DebugManager.h"
#include "SoundManager.h"
#include "Enemy.h"
#include "LevelManager.h"
#include "DisplayManager.h"
#include <string>
#include "CollisionManager.h"

std::vector<Label*> DisplayManager::listLabels;
std::vector<Sprite*> DisplayManager::listOfAttacks;

PlayState::PlayState()
{
	//contructor.
}

PlayState::~PlayState()
{
	//destructor
}

void PlayState::HandleEvents()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_M))
	{
		m_pathCounter = 0;
		
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_H)) // ~ or ` key. Toggle debug mode.
	{
		m_debugMode = !m_debugMode;
		//Play sound effect
		if(m_debugMode)
		{
			SOMA::SetMusicVolume(2);
		}
		else {
			SOMA::SetMusicVolume(20);
		}

		SOMA::PlaySound("changeMode", 0, -1);

	}
	if (EVMA::KeyPressed(SDL_SCANCODE_K)) {
		m_pEnemy->getLife() -= 10;
	}
	if(EVMA::KeyPressed(SDL_SCANCODE_P)) {
		if (m_pEnemy->getState() == IDLE)
			m_pEnemy->setState(RUNNING);
		else
			m_pEnemy->setState(IDLE);
	}
	if (EVMA::MousePressed(1)) 
	{
		m_pPlayer->setAnimationState(MELEE);
		SOMA::PlaySound("melee", 0, -1);
		m_pPlayer->MeleeAttack();
	}
	if (EVMA::MousePressed(3)) 
	{
		m_pPlayer->setAnimationState(SHOOTING);
		m_pPlayer->Shoot();
		SOMA::PlaySound("shoot", 0, -1);
	}
	if (m_debugMode) {
		
	}
}

void PlayState::Render()
{
	LevelManager::drawLevel();
	if (m_debugMode)
	{
		LevelManager::drawDebug();
		m_pPlayer->drawLOS();
		m_pPlayer->drawCollisionRect();
		m_pEnemy->drawLOS();
		m_pEnemy->drawRadius();
		m_pEnemy->drawCollisionRect();
		DisplayManager::drawDebug(DisplayManager::AttackList());
	}
	m_pPlayer->Render();
	m_pEnemy->Render();

	DisplayManager::draw(DisplayManager::LabelList());
	DisplayManager::draw(DisplayManager::AttackList());
}

void PlayState::Update()
{
	m_pPlayer->Update();
	m_pEnemy->Update();
	

	DisplayManager::update(DisplayManager::AttackList());

	checkCollision();

	DisplayManager::deleteAttacks(); // delete bullets that collided before.

	if (m_pPlayer->isMoving())
	{
		SOMA::PlaySound("running", 0, 0);
	}	
}

void PlayState::Exit()
{
	LevelManager::clean();
}

void PlayState::Resume()
{
}

void PlayState::checkCollision()
{
	for (auto a : DisplayManager::AttackList())
	{
		if (a->isRunning())
		{
			if (COMA::AABBCheck(*(a->GetCollisionBox()), *(m_pEnemy->GetCollisionBox())))
			{
				m_pEnemy->getLife() -= 20;
				a->isRunning() = false;
			}
		}
	}
}
void PlayState::Enter()
{
	const int SIZEOFTILES = 32;

	FOMA::RegisterFont("img/ltype.ttf", "tile", 10);
	LevelManager::loadTiles();
	LevelManager::loadLevel();

	m_pPlayerText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/PlayerSpritesheet.png");
	m_pEnemyText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/EnemySpriteSheet.png");
	
	m_pPlayer = new Player({ 0 , 0 , 253,216 }, { (float)(16) * 32, (float)(12) * 32, 64, 64 }, Engine::Instance().GetRenderer(), m_pPlayerText, 0, 0, 19, 4);
	m_pEnemy = new Enemy({ 0 , 0 , 291,226 }, { (float)(16) * 32, (float)(4) * 32, 64, 64 }, Engine::Instance().GetRenderer(), m_pEnemyText, 0, 0, 19, 4);

	

	//Music
	SOMA::Load("Aud/Background.mp3", "Background", SOUND_MUSIC);
	//Sound Effects 
	SOMA::Load("Aud/changeMode.mp3", "changeMode", SOUND_SFX);
	SOMA::Load("Aud/running-footstep-sound-effect.mp3", "running", SOUND_SFX);
	SOMA::Load("Aud/Grunting-sound.mp3", "grunting", SOUND_SFX);
	SOMA::Load("Aud/shoot.mp3", "shoot", SOUND_SFX);
	SOMA::Load("Aud/melee.mp3", "melee", SOUND_SFX);
	//play music
	//play music
	SOMA::PlayMusic("Background", -1, 0);
	SOMA::SetAllVolume(20);
	//Labels
	DisplayManager::LabelList().push_back(new Label("font", 10, 10, "Press 'H' to Debug mode", { 255,255,255,255 }));
	DisplayManager::LabelList().push_back(new Label("font", 10, 30, "Press 'WASD' to Move around mode", { 255,255,255,255 }));
	DisplayManager::LabelList().push_back(new Label("font", 10, 50, "Press 'F' to print the path", { 255,255,255,255 }));
	DisplayManager::LabelList().push_back(new Label("font", 10, 70, "Press 'M' to move to the target", { 255,255,255,255 }));
	DisplayManager::LabelList().push_back(new Label("font", 10, 90, "Press 'Righ Click' Change the goal position in debug mode", { 255,255,255,255 }));
	DisplayManager::LabelList().push_back(new Label("font", 10, 110, "Press 'Left Click' Change the player position in debug mode", { 255,255,255,255 }));

}



