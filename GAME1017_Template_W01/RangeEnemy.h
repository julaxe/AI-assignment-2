#pragma once
#include "Enemy.h"
#include "DecisionTree.h"
class RangeEnemy : public Enemy
{
public:
	RangeEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf);
	void update() override;
	void setState(AnimationState state) override;
	void setDestinations() override;

private:
	bool WeaponRangeDistance();
	bool wasHit();
	void RangeAttack();
	void buildTree();
	void buildMoveToLOSTree();
	int m_shootTimer;
	DecisionTree* m_RangeTree;
	DecisionTree* m_MoveToLOSTree;
};
