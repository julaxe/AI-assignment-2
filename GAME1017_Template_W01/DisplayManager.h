#pragma once
#include <vector>
#include <map>
#include "Label.h"
#include "Sprite.h"
struct DisplayManager {
	static std::vector<Label*>& LabelList() { return listLabels; }
	static std::vector<Sprite*>& AttackList() { return listOfAttacks; }

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
			DEMA::DrawRect(*(o->GetDstP()), { 1.0f,1.0f,1.0f,1.0f });
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
	
private:
	static std::vector<Label*> listLabels;
	static std::vector<Sprite*> listOfAttacks;
};
