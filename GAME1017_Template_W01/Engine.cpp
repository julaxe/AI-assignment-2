#include "Engine.h"
#include "CollisionManager.h"
#include "DebugManager.h"
#include "EventManager.h"
#include "FontManager.h"
#include "PathManager.h"
#include "SoundManager.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "StartScene.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

Engine::Engine():m_running(false){ cout << "Engine class constructed!" << endl; }

bool Engine::Init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	cout << "Initializing game..." << endl;
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window.
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				EVMA::Init();
				SOMA::Init();
				TEMA::Init();
			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	// Example specific initialization.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"); // Call this before any textures are created.
	
	// Start State
	StateManager::ChangeState(new StartScene);
	// Final engine initialization calls.
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	m_running = true; // Everything is okay, start the engine.
	cout << "Engine Init success!" << endl;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) // Engine has to sleep.
		SDL_Delay(m_fps - m_delta);
}

void Engine::HandleEvents()
{
	EVMA::HandleEvents();
}

void Engine::Update()
{
	StateManager::Update();
	/*// m_pPlayer->Update(); // Just stops MagaMan from moving.
	if (EVMA::KeyPressed(SDL_SCANCODE_GRAVE)) // ~ or ` key. Toggle debug mode.
		m_showCosts = !m_showCosts;
	if (EVMA::KeyPressed(SDL_SCANCODE_SPACE)) // Toggle the heuristic used for pathfinding.
	{
		m_hEuclid = !m_hEuclid;
		std::cout << "Setting " << (m_hEuclid?"Euclidian":"Manhattan") << " heuristic..." << std::endl;
	}
	if (EVMA::MousePressed(1) || EVMA::MousePressed(3)) // If user has clicked.
	{		
		int xIdx = (EVMA::GetMousePos().x / 32);
		int yIdx = (EVMA::GetMousePos().y / 32);
		if (m_level[yIdx][xIdx]->IsObstacle() || m_level[yIdx][xIdx]->IsHazard()) // Node() == nullptr;
			return; // We clicked on an invalid tile.
		if (EVMA::MousePressed(1)) // Move the player with left-click.
		{
			m_pPlayer->GetDstP()->x = (float)(xIdx * 32);
			m_pPlayer->GetDstP()->y = (float)(yIdx * 32);
		}
		else if (EVMA::MousePressed(3)) // Else move the bling with right-click.
		{
			m_pBling->GetDstP()->x = (float)(xIdx * 32);
			m_pBling->GetDstP()->y = (float)(yIdx * 32);
		}
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
				m_level[row][col]->m_lCost->SetText(to_string((int)(m_level[row][col]->Node()->H())).c_str());
			}
		}
		// Now we can calculate the path. Note: I am not returning a path again, only generating one to be rendered.
		PAMA::GetShortestPath(m_level[(int)(m_pPlayer->GetDstP()->y / 32)][(int)(m_pPlayer->GetDstP()->x / 32)]->Node(), 
			m_level[(int)(m_pBling->GetDstP()->y / 32)][(int)(m_pBling->GetDstP()->x / 32)]->Node());
	}*/
}

void Engine::Render() 
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	// Draw anew.
	/*for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			m_level[row][col]->Render(); // Render each tile.
			// Render the debug data...
			if (m_showCosts && m_level[row][col]->Node() != nullptr)
			{
				m_level[row][col]->m_lCost->Render();
				m_level[row][col]->m_lX->Render();
				m_level[row][col]->m_lY->Render();
				// I am also rendering out each connection in blue. If this is a bit much for you, comment out the for loop below.
				for (unsigned i = 0; i < m_level[row][col]->Node()->GetConnections().size(); i++)
				{
					DEMA::QueueLine({ m_level[row][col]->Node()->GetConnections()[i]->GetFromNode()->x + 16, m_level[row][col]->Node()->GetConnections()[i]->GetFromNode()->y + 16 },
						{ m_level[row][col]->Node()->GetConnections()[i]->GetToNode()->x + 16, m_level[row][col]->Node()->GetConnections()[i]->GetToNode()->y + 16 }, { 0,0,255,255 });
				}
			}
			
		}
	}
	m_pPlayer->Render();
	m_pBling->Render();
	PAMA::DrawPath(); // I save the path in a static vector to be drawn here.
	DEMA::FlushLines(); // And... render ALL the queued lines. Phew.*/
	// Draw anew.
	StateManager::Render();
	// Flip buffers.
	SDL_RenderPresent(m_pRenderer);
}

void Engine::Clean()
{
	/*cout << "Cleaning game." << endl;
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
	m_tiles.clear();*/
	// Finish cleaning.
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	DEMA::Quit();
	EVMA::Quit();
	FOMA::Quit();
	SOMA::Quit();
	STMA::Quit();
	TEMA::Quit();
	IMG_Quit();
	SDL_Quit();
}

int Engine::Run()
{
	if (m_running) // What does this do and what can it prevent?
		return -1; 
	if (Init("GAME1017 Engine Template", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;
	while (m_running) // Main engine loop.
	{
		Wake();
		HandleEvents();
		Update();
		Render();
		if (m_running)
			Sleep();
	}
	Clean();
	return 0;
}

Engine& Engine::Instance()
{
	static Engine instance; // C++11 will prevent this line from running more than once. Magic statics.
	return instance;
}

SDL_Renderer* Engine::GetRenderer() { return m_pRenderer; }
bool& Engine::Running() { return m_running; }
