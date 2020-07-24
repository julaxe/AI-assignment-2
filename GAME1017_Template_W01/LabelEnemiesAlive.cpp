#include "LabelEnemiesAlive.h"
#include "DisplayManager.h"

void LabelEnemiesAlive::update()
{
	str = "Enemies: " + std::to_string(DisplayManager::EnemiesList().size());
	Label::SetText(str.c_str());
}
