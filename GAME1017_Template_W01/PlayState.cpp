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

#include <string>

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
		CalculatePath();
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
	if (EVMA::KeyPressed(SDL_SCANCODE_P)) {
		m_pPlayer->getLife() -= 10;
		SOMA::PlaySound("grunting", 0, -1);
	}
	if (m_debugMode) {
		
	}
}

void PlayState::Render()
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			m_level[row][col]->Render(); // Render each tile.

		}
	}
	if (m_debugMode)
	{
		m_pPlayer->drawLOS();
		m_pEnemy->drawLOS();
		m_pEnemy->drawRadius();
	}
	m_pPlayer->Render();
	m_pEnemy->Render();
	m_pLabel1->Render();
	m_pLabel2->Render();
	m_pLabel3->Render();
	m_pLabel4->Render();
	m_pLabel5->Render();
	m_pLabel6->Render();
	
}

void PlayState::Update()
{
	m_pPlayer->Update(m_level);
	m_pEnemy->Update(m_level);
	if (m_pPlayer->isMoving())
	{
		SOMA::PlaySound("running", 0, -1);
	}
	else
	{
		SOMA::StopSound(-1);
	}
	
}

void PlayState::Enter()
{
	const int SIZEOFTILES = 32;
	m_pTileText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/Tiles.png");
	m_pPlayerText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/PlayerSpritesheet.png");
	m_pEnemyText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/EnemySpriteSheet.png");
	FOMA::RegisterFont("img/ltype.ttf", "tile", 10);
	m_pPlayer = new Player({ 0 , 0 , 253,216 }, { (float)(16) * 32, (float)(12) * 32, 64, 64 }, Engine::Instance().GetRenderer(), m_pPlayerText, 0, 0, 19, 4);
	m_pEnemy = new Enemy({ 0 , 0 , 291,226 }, { (float)(16) * 32, (float)(4) * 32, 64, 64 }, Engine::Instance().GetRenderer(), m_pEnemyText, 0, 0, 19, 4);
	std::ifstream inFile("Dat/Tiledata.txt");
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
	inFile.open("Dat/Level1.txt");
	if (inFile.is_open())
	{ // Build the level from Tile prototypes.
		char key;
		for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{
				inFile >> key;
				m_level[row][col] = m_tiles[key]->Clone(); // Prototype design pattern used.
				m_level[row][col]->GetDstP()->x = (float)(32 * col);
				m_level[row][col]->GetDstP()->y = (float)(32 * row);
				// Instantiate the labels for each tile.
				m_level[row][col]->m_lCost = new Label("tile", m_level[row][col]->GetDstP()->x + 4, m_level[row][col]->GetDstP()->y + 18, " ", { 0,0,0,255 });
				m_level[row][col]->m_lX = new Label("tile", m_level[row][col]->GetDstP()->x + 18, m_level[row][col]->GetDstP()->y + 2, std::to_string(col).c_str(), { 0,0,0,255 });
				m_level[row][col]->m_lY = new Label("tile", m_level[row][col]->GetDstP()->x + 2, m_level[row][col]->GetDstP()->y + 2, std::to_string(row).c_str(), { 0,0,0,255 });
				// Construct the Node for a valid tile.
				if (!m_level[row][col]->IsObstacle() && !m_level[row][col]->IsHazard())
					m_level[row][col]->m_node = new PathNode((int)(m_level[row][col]->GetDstP()->x), (int)(m_level[row][col]->GetDstP()->y));
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
	//Music
	SOMA::Load("Aud/Background.mp3", "Background", SOUND_MUSIC);
	//Sound Effects 
	SOMA::Load("Aud/changeMode.mp3", "changeMode", SOUND_SFX);
	SOMA::Load("Aud/running-footstep-sound-effect.mp3", "running", SOUND_SFX);
	SOMA::Load("Aud/Grunting-sound.mp3", "grunting", SOUND_SFX);
	//play music
	SOMA::PlayMusic("Background", -1, 0);
	SOMA::SetAllVolume(20);
	//Labels
	m_pLabel1 = new Label("font", 10, 10, "Press 'H' to Debug mode", { 255,255,255,255 });
	m_pLabel2 = new Label("font", 10, 30, "Press 'WASD' to Move around mode", { 255,255,255,255 });
	m_pLabel3 = new Label("font", 10, 50, "Press 'F' to print the path", { 255,255,255,255 });
	m_pLabel4 = new Label("font", 10, 70, "Press 'M' to move to the target", { 255,255,255,255 });
	m_pLabel5 = new Label("font", 10, 90, "Press 'Righ Click' Change the goal position in debug mode", { 255,255,255,255 });
	m_pLabel6 = new Label("font", 10, 110, "Press 'Left Click' Change the player position in debug mode", { 255,255,255,255 });
}

void PlayState::Exit()
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

void PlayState::Resume()
{
}

void PlayState::CalculatePath()
{
	for (int row = 0; row < ROWS; row++) // "This is where the fun begins."
	{ // Update each node with the selected heuristic and set the text for debug mode.
		for (int col = 0; col < COLS; col++)
		{
			if (m_level[row][col]->Node() == nullptr)
				continue;
			if (m_hEuclid)
				m_level[row][col]->Node()->SetH(PAMA::HEuclid(m_level[row][col]->Node(), m_level[(int)(m_pBling->GetDstP()->y / 32)][(int)(m_pBling->GetDstP()->x / 32)]->Node()));
			else
				m_level[row][col]->Node()->SetH(PAMA::HManhat(m_level[row][col]->Node(), m_level[(int)(m_pBling->GetDstP()->y / 32)][(int)(m_pBling->GetDstP()->x / 32)]->Node()));
			m_level[row][col]->m_lCost->SetText(std::to_string((int)(m_level[row][col]->Node()->H())).c_str());
		}
	}
	// Now we can calculate the path. Note: I am not returning a path again, only generating one to be rendered.
	PAMA::GetShortestPath(m_level[(int)((m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h/2) / 32)][(int)((m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w / 2) / 32)]->Node(),
		m_level[(int)(m_pBling->GetDstP()->y / 32)][(int)(m_pBling->GetDstP()->x / 32)]->Node());
}

