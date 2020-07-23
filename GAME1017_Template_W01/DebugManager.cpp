#include "DebugManager.h"
#include "MathManager.h"
#include "Engine.h"

void DebugManager::DrawLine(const SDL_Point start, const SDL_Point end, const SDL_Color col)
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), col.r, col.g, col.b, col.a);
	SDL_RenderDrawLine(Engine::Instance().GetRenderer(), end.x, end.y, start.x, start.y);
}
void DebugManager::DrawLine(const SDL_FPoint start, const SDL_Point end, const SDL_Color col)
{
    SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), col.r, col.g, col.b, col.a);
    SDL_RenderDrawLine(Engine::Instance().GetRenderer(), end.x, end.y, (int)start.x, (int)start.y);
}

void DebugManager::QueueLine(const SDL_Point start, const SDL_Point end, const SDL_Color col)
{
	s_points.push_back(start);
	s_points.push_back(end);
	s_colors.push_back(col);
}

void DebugManager::FlushLines()
{
	for (unsigned i = 0, j = 0; i < s_points.size(); i+=2, j++)
		DrawLine(s_points[i], s_points[i+1], s_colors[j]);
	Quit(); // Clears out vectors.
}

void DebugManager::DrawRay(const SDL_Point start, const double angle, const double length, const SDL_Color col)
{ // Raw angle in radians required.
	double dX, dY;
	MAMA::SetDeltas(angle, dX, dY, length, length);
	//std::cout << start.x << ',' << start.y << '|' << start.x+dX << ',' << start.y+dY << std::endl;
	//DrawLine({(int)start.x, (int)start.y}, { (int)(start.x + dX), (int)(start.y + dY) }, col);
}
void DebugManager::DrawCircle(int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(Engine::Instance().GetRenderer(), centreX + x, centreY - y);
        SDL_RenderDrawPoint(Engine::Instance().GetRenderer(), centreX + x, centreY + y);
        SDL_RenderDrawPoint(Engine::Instance().GetRenderer(), centreX - x, centreY - y);
        SDL_RenderDrawPoint(Engine::Instance().GetRenderer(), centreX - x, centreY + y);
        SDL_RenderDrawPoint(Engine::Instance().GetRenderer(), centreX + y, centreY - x);
        SDL_RenderDrawPoint(Engine::Instance().GetRenderer(), centreX + y, centreY + x);
        SDL_RenderDrawPoint(Engine::Instance().GetRenderer(), centreX - y, centreY - x);
        SDL_RenderDrawPoint(Engine::Instance().GetRenderer(), centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}
void DebugManager::DrawRect(SDL_FRect rect ,SDL_FRect colour)
{
    int r = floor(colour.x * 255.0f);
    int g = floor(colour.y * 255.0f);
    int b = floor(colour.w * 255.0f);
    int a = floor(colour.h * 255.0f);

    SDL_Rect rectangle = { rect.x, rect.y, rect.w, rect.h };

    const auto renderer = /* TheGame::Instance()->getRenderer()*/ Engine::Instance().GetRenderer();

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawRect(renderer, &rectangle);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}


void DebugManager::Quit()
{
	s_points.clear();
	s_colors.clear();
	s_points.shrink_to_fit();
	s_colors.shrink_to_fit();
}

int DebugManager::s_debugMode = 0;
std::vector<SDL_Point> DebugManager::s_points;
std::vector<SDL_Color> DebugManager::s_colors;