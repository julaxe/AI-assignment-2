#pragma once
#include <vector>
#include <map>
#include "Label.h"
#include "Sprite.h"
struct DisplayManager {
	static std::vector<Label*>& LabelList() { return listLabels; }
	static std::vector<Sprite*>& BulletList() { return listBulletList; }

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

	static void deleteBulllets()
	{
		for (int i = 0; i < listBulletList.size() ; i++ )
		{
			if (!listBulletList[i]->isRunning())
			{
				delete listBulletList[i];
				listBulletList[i] = nullptr;
			}
		}
		listBulletList.erase(std::remove(listBulletList.begin(), listBulletList.end(), nullptr), listBulletList.end());
		listBulletList.shrink_to_fit();
	}
	
private:
	static std::vector<Label*> listLabels;
	static std::vector<Sprite*> listBulletList;
};
