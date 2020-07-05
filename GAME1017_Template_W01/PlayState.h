#pragma once
#include "States.h"
#include "Tile.h"
#include "Player.h"
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
	//std::array<std::array<Tile*, COLS>, ROWS>& GetLevel() { return m_level; }

	void CalculatePath();

private:
	SDL_Texture* m_pTileText, * m_pPlayerText;
	std::map<char, Tile*> m_tiles;
	std::array<std::array<Tile*, COLS>, ROWS> m_level; // Fixed-size STL array of Tile pointers.
	Player* m_pPlayer;
	Sprite* m_pBling;
	bool m_showCosts = false, m_hEuclid = true, m_showPath = false, m_moving = false;
	int m_pathCounter = 0;
	Label* m_pLabel1;
	Label* m_pLabel2;
	Label* m_pLabel3;
	Label* m_pLabel4;
	Label* m_pLabel5;
	Label* m_pLabel6;
};