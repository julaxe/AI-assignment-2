#pragma once
#include <map>
#include <array>
#include "Tile.h"
struct LevelManager {
	//functions
	static void loadTiles();
	static void loadLevel();
	static void drawLevel();
	static void drawDebug();
	static void clean();
	static std::vector<PathConnection*> calculatePathTo(AnimatedSprite* obj, PathNode* goal);

	//members

	static float SIZEOFTILES;
	static bool m_hEuclid;
	static SDL_Texture* m_pTileText;
	static std::ifstream inFile;
	static std::array<std::array<Tile*, COLS>, ROWS> m_level;
	static std::vector<Tile*> m_obstacles;
	static std::map<char, Tile*> m_tiles;

};