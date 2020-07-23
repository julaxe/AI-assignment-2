#pragma once
#include "States.h"
#include "Tile.h"
#include "Player.h"
#include "Enemy.h"
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
	SDL_Texture* m_pPlayerText, *m_pEnemyText;

	Player* m_pPlayer;
	Enemy* m_pEnemy;
	bool m_debugMode = false;
	int m_pathCounter = 0;
	bool RadiusCollisionCheck;
};