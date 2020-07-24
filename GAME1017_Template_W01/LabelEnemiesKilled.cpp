#include "LabelEnemiesKilled.h"
#include "DisplayManager.h"
void LabelEnemiesKilled::update()
{
	str = "Kills: " + std::to_string(DisplayManager::getEnemiesKilled());
	Label::SetText(str.c_str());
}
