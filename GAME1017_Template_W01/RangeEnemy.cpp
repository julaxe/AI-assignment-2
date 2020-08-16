#include "RangeEnemy.h"
#include "LifeBar.h"
#include "SoundManager.h"
#include"DisplayManager.h"
#include "CollisionManager.h"
#define SPEED 2

RangeEnemy::RangeEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:Enemy(s, d, r, t, sstart, smin, smax, nf)
{
	setDestinations();
}
void RangeEnemy::update()
{
	updatePosition();
	updateCollisionBox(40.0f, 40.0f);
	updateLOS(DisplayManager::PlayerList());
	updateRAD();

	switch (m_animationState)
	{
	case IDLE:
		if (getLife() <= 0) {
			setState(DEATH);
			SoundManager::PlaySound("death", 0, -1);
		}
		break;
	case PATROL:

		Patrol();
		if (getLife() <= 0) {
			setState(DEATH);
			SoundManager::PlaySound("death", 0, -1);
		}

		break;
	case MELEE:
		break;
	case SHOOTING:
		break;
	case DEATH:
		Die();
		break;
	default:
		break;
	}
	for (auto s : UIList)
	{
		s->update(this);
	}
}

void RangeEnemy::setState(AnimationState state)
{
	m_animationState = state;
	switch (m_animationState)
	{
	case IDLE:
		m_src = { 0 , 412 , 313,206 };
		m_spriteMax = 19;
		m_sprite = 0;
		break;
	case PATROL:
		m_src = { 0 , 0,313,206 };
		m_spriteMax = 19;
		m_sprite = 0;
		break;
	case MELEE:
		break;
	case SHOOTING:
		m_src = { 0 ,206,316,206 };
		m_spriteMax = 3;
		m_sprite = 0;
		break;
	default:
		break;
	}
}

void RangeEnemy::setDestinations()
{
	m_path.clear();
	m_destinations.push_back(LevelManager::m_level[4][6]->Node());
	m_destinations.push_back(LevelManager::m_level[7][6]->Node());
	m_destinations.push_back(LevelManager::m_level[7][9]->Node());
	m_destinations.push_back(LevelManager::m_level[4][9]->Node());
	m_path = LevelManager::calculatePathTo(this, getDestinations()[destinationNumber]);
	pathCounter = 0;
}

bool RangeEnemy::WeaponRangeDistance()
{
	if (COMA::CircleCircleCheck(getPosition(), DisplayManager::PlayerList()[0]->getPosition(), 500, 20))
		return true;
	else
		return false;
}

bool RangeEnemy::wasHit()
{
	return false;
}

void RangeEnemy::RangeAttack()
{
}

void RangeEnemy::buildTree()
{
	//Actions
	TreeNode* RangeAttack = new TreeNode();
	RangeAttack->Action = [this]() {std::cout << "Range attack \n";  };

	TreeNode* MoveToRange = new TreeNode();
	MoveToRange->Action = [this]() {std::cout << "Move to Range \n"; };

	TreeNode* MoveToLOS = new TreeNode();
	MoveToLOS->Action = [this]() {std::cout << "Move to LOS \n"; };

	TreeNode* MoveToCover = new TreeNode();
	MoveToCover->Action = [this]() {std::cout << "Move to Cover \n"; };

	TreeNode* Flee = new TreeNode();
	Flee->Action = [this]() {std::cout << "Flee \n"; };

	TreeNode* Patrol = new TreeNode();
	Patrol->Action = [this]() {std::cout << "Patrol \n"; };

	// Conditions

	TreeNode* inCCRange = new TreeNode();
	inCCRange->Condition = [this]() { return WeaponRangeDistance(); };
	inCCRange->Left = RangeAttack;
	inCCRange->Right = MoveToRange;

	TreeNode* inRadius = new TreeNode();
	inRadius->Condition = [this]() { return m_inRadius; };
	inRadius->Left = MoveToLOS;
	inRadius->Right = Patrol;

	TreeNode* inLOS = new TreeNode();
	inLOS->Condition = [this]() { return m_inLOS; };
	inLOS->Left = inCCRange;
	inLOS->Right = inRadius;

	TreeNode* NotHit = new TreeNode();
	NotHit->Condition = [this]() { return wasHit(); };
	NotHit->Left = inLOS;
	NotHit->Right = MoveToCover;

	TreeNode* NotlowHp = new TreeNode();
	NotlowHp->Condition = [this]() {
		if (m_Life > 25)
			return true;
		else
			return false;
	};
	NotlowHp->Left = NotHit;
	NotlowHp->Right = Flee;

	m_RangeTree->setRoot(NotlowHp);
}

void RangeEnemy::buildMoveToLOSTree()
{
	TreeNode* MoveToNodeWLOS = new TreeNode();
	MoveToNodeWLOS->Action = [this]() {
		MoveToLOS();
	};
	TreeNode* LookPlayer = new TreeNode();
	LookPlayer->Action = [this](){		
		Enemy::LookPlayer();
		};

	TreeNode* onNodeWLOS = new TreeNode();
	onNodeWLOS->Condition = [this]() {
		return OnTopOfNodeWLOS();
	};
	onNodeWLOS->Left = LookPlayer;
	onNodeWLOS->Right = MoveToNodeWLOS;

	m_MoveToLOSTree->setRoot(onNodeWLOS);
}
