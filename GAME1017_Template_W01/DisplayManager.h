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
<<<<<<< HEAD
	
protected:
=======

	static void deleteNullPtr(std::vector<Sprite*>& list)
	{
		list.erase(std::remove(list.begin(), list.end(), nullptr), list.end());
		list.shrink_to_fit();
	}

private:
>>>>>>> b96610e32eff313582d8fc4973937138464f725b
	static std::vector<Label*> listLabels;
	static std::vector<Sprite*> listOfAttacks;
	static std::vector<Sprite*> listOfEnemies;
	static std::vector<Sprite*> listOfPlayer;
};
