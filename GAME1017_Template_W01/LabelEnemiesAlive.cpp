#include "LabelEnemiesAlive.h"
#include "DisplayManager.h"

void LabelEnemiesAlive::Update()
{
	str = "Number of enemies: " + std::to_string(DisplayManager::EnemiesList().size());
	Label::SetText(str.c_str());
}
