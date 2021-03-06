#pragma once
#include <vector>
#include <map>
#include "Label.h"
#include "LabelEnemiesAlive.h"
#include "Sprite.h"
struct DisplayManager {
	static std::vector<Label*>& LabelList() { return listLabels; }
	static std::vector<Sprite*>& AttackList() { return listOfAttacks; }
	static std::vector<Sprite*>& EnemiesList() { return listOfEnemies; }
	static std::vector<Sprite*>& PlayerList() { return listOfPlayer; }
	static std::vector<Sprite*>& DestructableObjList() { return listOfDestructable; }
	template<typename T>
	static void draw(std::vector<T*> list)
	{
		for (auto o : list)
		{
			o->Render();
		}
	}
	template<typename T>
	static void update(std::vector<T*> list)
	{
		for (auto o : list)
		{
			o->update();
		}
	}
	template<typename T>
	static void drawDebug(std::vector<T*> list)
	{
		for (auto o : list)
		{
			DEMA::DrawRect(*(o->GetCollisionBox()), { 1.0f,1.0f,1.0f,1.0f });
		}
	}

	static void deleteAttacks()
	{
		for (int i = 0; i < listOfAttacks.size() ; i++ )
		{
			if (!listOfAttacks[i]->isRunning())
			{
				delete listOfAttacks[i];
				listOfAttacks[i] = nullptr;
			}
		}
		listOfAttacks.erase(std::remove(listOfAttacks.begin(), listOfAttacks.end(), nullptr), listOfAttacks.end());
		listOfAttacks.shrink_to_fit();
	}
	static void deleteBarrels()
	{
		for (int i = 0; i < listOfDestructable.size(); i++)
		{
			if (!listOfDestructable[i]->isRunning())
			{
				delete listOfDestructable[i];
				listOfDestructable[i] = nullptr;
			}
		}
		listOfDestructable.erase(std::remove(listOfDestructable.begin(), listOfDestructable.end(), nullptr), listOfDestructable.end());
		listOfDestructable.shrink_to_fit();
	}

	static void deleteNullPtr(std::vector<Sprite*>& list)
	{
		list.erase(std::remove(list.begin(), list.end(), nullptr), list.end());
		list.shrink_to_fit();
	}

	static int& getEnemiesKilled() { return enemiesKilled; }

private:
	static int enemiesKilled;
	static std::vector<Label*> listLabels;
	static std::vector<Sprite*> listOfAttacks;
	static std::vector<Sprite*> listOfEnemies;
	static std::vector<Sprite*> listOfPlayer;
	static std::vector<Sprite*> listOfDestructable;
};
