#pragma once
#include "Enemy.h"
#include "DecisionTree.h"
class MeleeEnemy : public Enemy
{
public:
	MeleeEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf);
	void update() override;
	void setState(AnimationState state) override;
	void setDestinations() override;

private:
	bool closeCombatDistance();
	void MeleeAttack();
	void buildTree();
	void buildMoveToLOSTree();
	DecisionTree* m_MeleeTree;
	DecisionTree* m_MoveToLOSTree;
	
};
