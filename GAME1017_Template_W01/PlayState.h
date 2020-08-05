#pragma once
#include "States.h"
#include "Tile.h"
#include "Player.h"
#include "Enemy.h"
#include "DestructableObj.h"
#include <array>
#include <map>



class PlayState : public State {
public:
	PlayState();
	~PlayState();
	void HandleEvents() override;
	void Render() override;
	void Update() override;
	void Enter() override;
	void Exit() override;
	void Resume() override;
	
private:
	void checkCollision();
	void deleteDeathEnemies();
	SDL_Texture* m_pPlayerText, *m_pEnemyText, *m_barrelText;

	//Player* m_pPlayer;
	bool m_debugMode = false;
	int m_pathCounter = 0;
	bool RadiusCollisionCheck;
	
};