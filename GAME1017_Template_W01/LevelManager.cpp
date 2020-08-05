#include "LevelManager.h"
#include <fstream>
#include "Engine.h"
#include "MathManager.h"
#include "PathManager.h"
#include "DisplayManager.h"
#include "CollisionManager.h"

//init of static variables
float LevelManager::SIZEOFTILES = 64;
bool LevelManager::m_hEuclid = true;

SDL_Texture* LevelManager::m_pTileText;
std::ifstream LevelManager::inFile;
std::array<std::array<Tile*, COLS>, ROWS> LevelManager::m_level;
std::vector<Sprite*> LevelManager::m_obstacles;
std::vector<Tile*> LevelManager::m_nodes;
std::map<char, Tile*> LevelManager::m_tiles;


void LevelManager::loadTiles()
{
	inFile.open("Dat/Tiledata.txt");
	m_pTileText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/Tiles.png");
	if (inFile.is_open())
	{ // Create map of Tile prototypes.
		char key;
		int x, y;
		bool o, h;
		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> o >> h;
			m_tiles.emplace(key, new Tile({ x * 32, y * 32, 32, 32 }, { 0,0,SIZEOFTILES,SIZEOFTILES }, Engine::Instance().GetRenderer(), m_pTileText, o, h));
		}
	}
	inFile.close();
}

void LevelManager::loadLevel()
{
	inFile.open("Dat/Level2.txt");
	if (inFile.is_open())
	{ // Build the level from Tile prototypes.
		char key;
		for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{
				inFile >> key;
				m_level[row][col] = m_tiles[key]->Clone(); // Prototype design pattern used.
				m_level[row][col]->GetDstP()->x = (float)(SIZEOFTILES * col);
				m_level[row][col]->GetDstP()->y = (float)(SIZEOFTILES * row);
				m_level[row][col]->GetCollisionBox()->x = (float)(SIZEOFTILES * col);
				m_level[row][col]->GetCollisionBox()->y = (float)(SIZEOFTILES * row);
				// Instantiate the labels for each tile.
				
				// Construct the Node for a valid tile.
				if (!m_level[row][col]->IsObstacle() && !m_level[row][col]->IsHazard())
					m_level[row][col]->m_node = new PathNode((int)(m_level[row][col]->GetDstP()->x + m_level[row][col]->GetDstP()->w*0.5), (int)(m_level[row][col]->GetDstP()->y + m_level[row][col]->GetDstP()->h*0.5));
				if (m_level[row][col]->IsObstacle())
				{
					m_obstacles.push_back(m_level[row][col]);
				}
				else
				{
					m_nodes.push_back(m_level[row][col]);
				}
			}
		}
	}
	inFile.close();
	// Now build the graph from ALL the non-obstacle and non-hazard tiles. Only N-E-W-S compass points.
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			if (m_level[row][col]->Node() == nullptr) // Now we can test for nullptr.
				continue; // An obstacle or hazard tile has no connections.
			// Make valid connections. Inside map and a valid tile.
			if (row - 1 != -1 && m_level[row - 1][col]->Node() != nullptr) // Tile above. 
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row - 1][col]->Node(),
					MAMA::Distance(m_level[row][col]->Node()->x, m_level[row - 1][col]->Node()->x, m_level[row][col]->Node()->y, m_level[row - 1][col]->Node()->y)));
			if (row + 1 != ROWS && m_level[row + 1][col]->Node() != nullptr) // Tile below.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row + 1][col]->Node(),
					MAMA::Distance(m_level[row][col]->Node()->x, m_level[row + 1][col]->Node()->x, m_level[row][col]->Node()->y, m_level[row + 1][col]->Node()->y)));
			if (col - 1 != -1 && m_level[row][col - 1]->Node() != nullptr) // Tile to Left.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row][col - 1]->Node(),
					MAMA::Distance(m_level[row][col]->Node()->x, m_level[row][col - 1]->Node()->x, m_level[row][col]->Node()->y, m_level[row][col - 1]->Node()->y)));
			if (col + 1 != COLS && m_level[row][col + 1]->Node() != nullptr) // Tile to right.
				m_level[row][col]->Node()->AddConnection(new PathConnection(m_level[row][col]->Node(), m_level[row][col + 1]->Node(),
					MAMA::Distance(m_level[row][col]->Node()->x, m_level[row][col + 1]->Node()->x, m_level[row][col]->Node()->y, m_level[row][col + 1]->Node()->y)));
		}
	}
}

void LevelManager::drawLevel()
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			m_level[row][col]->Render(); // Render each tile.

		}
	}
}

void LevelManager::drawDebug()
{
	
	for (int i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i]->Node()->inLOS())
		{
			DEMA::DrawRect({ (float)m_nodes[i]->Node()->Pt().x , (float)m_nodes[i]->Node()->Pt().y, 5,5 }, { 1.0f,0.0f,0.0f,1.0f });
		}
		else
		{
			DEMA::DrawRect({ (float)m_nodes[i]->Node()->Pt().x , (float)m_nodes[i]->Node()->Pt().y, 5,5 }, { 1.0f,1.0f,1.0f,1.0f });
		}
	}
}

void LevelManager::updateNodes()
{
	for (int i = 0; i < m_nodes.size(); i++) 
	{
		m_nodes[i]->Node()->updateLOSwithPlayer();
		for (auto o : DisplayManager::DestructableObjList())
		{
			if (COMA::AABBCheck({ (float)m_nodes[i]->Node()->Pt().x , (float)m_nodes[i]->Node()->Pt().y, 5,5 }, *(o->GetCollisionBox())))
			{
				m_nodes[i]->Node()->isObstacle() = true;
			}
			else
			{
				m_nodes[i]->Node()->isObstacle() = false;
			}
		}
	}
}

void LevelManager::clean()
{
	std::cout << "Cleaning game." << std::endl;
	// Example-specific cleaning.
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			delete m_level[row][col];
			m_level[row][col] = nullptr; // Wrangle your dangle.
		}
	}
	for (auto const& i : m_tiles)
		delete m_tiles[i.first];
	m_tiles.clear();
}

std::vector<PathConnection*> LevelManager::calculatePathTo(AnimatedSprite* obj, PathNode* goal)
{
	for (int row = 0; row < ROWS; row++) // "This is where the fun begins."
	{ // Update each node with the selected heuristic and set the text for debug mode.
		for (int col = 0; col < COLS; col++)
		{
			if (m_level[row][col]->Node() == nullptr || m_level[row][col]->Node()->isObstacle())
				continue;
			if (m_hEuclid)
				m_level[row][col]->Node()->SetH(PAMA::HEuclid(m_level[row][col]->Node(), m_level[(int)(goal->Pt().y / SIZEOFTILES)][(int)(goal->Pt().x / SIZEOFTILES)]->Node()));
			else
				m_level[row][col]->Node()->SetH(PAMA::HManhat(m_level[row][col]->Node(), m_level[(int)(goal->Pt().y / SIZEOFTILES)][(int)(goal->Pt().x / SIZEOFTILES)]->Node()));
		}
	}
	// Now we can calculate the path. Note: I am not returning a path again, only generating one to be rendered.

	return PAMA::GetShortestPath(m_level[(int)((obj->GetDstP()->y + obj->GetDstP()->h / 2) / SIZEOFTILES)][(int)((obj->GetDstP()->x + obj->GetDstP()->w / 2) / SIZEOFTILES)]->Node(),
		m_level[(int)(goal->Pt().y / SIZEOFTILES)][(int)(goal->Pt().x / SIZEOFTILES)]->Node());
}
