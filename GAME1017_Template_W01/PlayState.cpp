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
#include "LabelEnemiesAlive.h"
#include "LabelEnemiesKilled.h"

std::vector<Label*> DisplayManager::listLabels;
std::vector<Sprite*> DisplayManager::listOfAttacks;
std::vector<Sprite*> DisplayManager::listOfEnemies;
std::vector<Sprite*> DisplayManager::listOfPlayer;
int DisplayManager::enemiesKilled = 0;

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
		for (auto e : DisplayManager::EnemiesList())
		{
			e->getLife() -= 10;
			SoundManager::PlaySound("grunting", 0, -1);
		}
	}
	if(EVMA::KeyPressed(SDL_SCANCODE_P)) {
		for (auto e : DisplayManager::EnemiesList())
		{
			if (dynamic_cast<Enemy*>(e)->getState() == IDLE)
				dynamic_cast<Enemy*>(e)->setState(RUNNING);
			else
				dynamic_cast<Enemy*>(e)->setState(IDLE);
		}
	}
	if (EVMA::MousePressed(1)) 
	{
		dynamic_cast<Player*>(DisplayManager::PlayerList()[0])->setAnimationState(MELEE);
		SOMA::PlaySound("melee", 0, -1);
		dynamic_cast<Player*>(DisplayManager::PlayerList()[0])->MeleeAttack();
	}
	if (EVMA::MousePressed(3)) 
	{
		dynamic_cast<Player*>(DisplayManager::PlayerList()[0])->setAnimationState(SHOOTING);
		dynamic_cast<Player*>(DisplayManager::PlayerList()[0])->Shoot();
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
		
		for (auto e : DisplayManager::EnemiesList())
		{
			e->drawLOS();
			e->drawRadius(200);
			//e->drawPath();
		}

		DisplayManager::PlayerList()[0]->drawLOS();
		DisplayManager::drawDebug(DisplayManager::AttackList());
		DisplayManager::drawDebug(DisplayManager::PlayerList());
		DisplayManager::drawDebug(DisplayManager::EnemiesList());
	}
	
	DisplayManager::draw(DisplayManager::PlayerList());
	DisplayManager::draw(DisplayManager::EnemiesList());
	DisplayManager::draw(DisplayManager::LabelList());
	DisplayManager::draw(DisplayManager::AttackList());
}

void PlayState::Update()
{
	
	DisplayManager::update(DisplayManager::LabelList());

	DisplayManager::update(DisplayManager::PlayerList());

	DisplayManager::update(DisplayManager::EnemiesList());

	DisplayManager::update(DisplayManager::AttackList());

	checkCollision();

	DisplayManager::deleteAttacks(); // delete bullets that collided before.
	deleteDeathEnemies();


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
			for (auto e : DisplayManager::EnemiesList())
			{
				if (COMA::AABBCheck(*(a->GetCollisionBox()), *(e->GetCollisionBox())))
				{
					e->getLife() -= 20;
					SoundManager::PlaySound("grunting", 0, -1);
					a->isRunning() = false;
				}

			}
		}
	}
	if (!RadiusCollisionCheck)
	{
		for (auto e : DisplayManager::EnemiesList())
		{
			if (COMA::CircleCircleCheck(e->getPosition(), DisplayManager::PlayerList()[0]->getPosition(), 200, 20) )
			{
				RadiusCollisionCheck = true;
				std::cout << "Radius COLLISION!" << std::endl;
			}
		}
	}
	else
	{
		for (auto e : DisplayManager::EnemiesList())
		{
			if (!COMA::CircleCircleCheck(e->getPosition(), DisplayManager::PlayerList()[0]->getPosition(), 200, 20))
			{
				RadiusCollisionCheck = false;
				std::cout << "No Radius COLLISION!" << std::endl;
			}
		}
	}
	
}
void PlayState::deleteDeathEnemies()
{
	for (int i= 0; i < DisplayManager::EnemiesList().size(); i++)
	{
		if (!dynamic_cast<Enemy*>(DisplayManager::EnemiesList()[i])->IsAlive())
		{
			delete DisplayManager::EnemiesList()[i];
			DisplayManager::EnemiesList()[i] = nullptr;
			DisplayManager::getEnemiesKilled() += 1;
		}
	}
	DisplayManager::deleteNullPtr(DisplayManager::EnemiesList());
}
void PlayState::Enter()
{

	RadiusCollisionCheck = false;
	FOMA::RegisterFont("img/ltype.ttf", "tile", 10);
	LevelManager::loadTiles();
	LevelManager::loadLevel();

	m_pPlayerText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/PlayerSpritesheet.png");
	m_pEnemyText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/EnemySpriteSheet.png");
	
	DisplayManager::PlayerList().push_back(new Player({ 0 , 0 , 253,216 }, { (float)(16) * 32, (float)(14) * 32, 64, 64 }, Engine::Instance().GetRenderer(), m_pPlayerText, 0, 0, 19, 4));
	DisplayManager::EnemiesList().push_back(new Enemy({ 0 , 0 , 291,226 }, { (float)(16) * 32, (float)(4) * 32, 64, 64 }, Engine::Instance().GetRenderer(), m_pEnemyText, 0, 0, 19, 4));

	

	//Music
	SOMA::Load("Aud/Background.mp3", "Background", SOUND_MUSIC);
	//Sound Effects 
	SOMA::Load("Aud/changeMode.mp3", "changeMode", SOUND_SFX);
	SOMA::Load("Aud/running-footstep-sound-effect.mp3", "running", SOUND_SFX);
	SOMA::Load("Aud/Grunting-sound.mp3", "grunting", SOUND_SFX);
	SOMA::Load("Aud/shoot.mp3", "shoot", SOUND_SFX);
	SOMA::Load("Aud/melee.mp3", "melee", SOUND_SFX);
	SOMA::Load("Aud/Death.mp3", "death", SOUND_SFX);
	//play music
	//play music
	SOMA::PlayMusic("Background", -1, 0);
	SOMA::SetAllVolume(20);
	//Labels
	
	
	DisplayManager::LabelList().push_back(new LabelEnemiesAlive("font", 10, 10, "Enemies: ", {255,255,255,255}));
	DisplayManager::LabelList().push_back(new LabelEnemiesKilled("font", 300, 10, "Kills: ", { 255,255,255,255 }));
}



