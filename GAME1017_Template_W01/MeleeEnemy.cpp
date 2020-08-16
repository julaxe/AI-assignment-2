#include "MeleeEnemy.h"
#include "LifeBar.h"
#include "SoundManager.h"
#include"DisplayManager.h"
#include "MathManager.h"
#include "CollisionManager.h"
#include "MeleeAtk.h"
#include "Enemy.h"
#include "Engine.h"
#define SPEED 2

MeleeEnemy::MeleeEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:Enemy(s, d, r, t, sstart, smin, smax, nf)
{
	setDestinations();
	m_MeleeTree = new DecisionTree();
	m_MoveToLOSTree = new DecisionTree();
	
	buildMoveToLOSTree();
	buildTree();
}
void MeleeEnemy::update()
{
	updatePosition();
	updateCollisionBox(40.0f, 40.0f);
	updateLOSToPlayerInsideRadius();
	updateLOS(DisplayManager::PlayerList());
	updateRAD();
	
	
	DEMA::DrawCircle(m_dst.x + m_dst.w * 0.5, m_dst.y + m_dst.h * 0.5, 50, { 255,20,20,255 }); //for meleeAtck
	
	switch (m_animationState)
	{
	case IDLE:
		break;
	case PATROL:
		
		m_MeleeTree->solveTree();
		Patrol();
		break;
	case RUNNING:
		m_MeleeTree->solveTree();
		MoveToPlayer();
		break;
	case MELEE:
		if (m_animationDone)
		{
			m_animationDone = false;
			setState(PATROL);
		}
		break;
	case MOVETOLOS:
		m_MeleeTree->solveTree();
		if(MoveToLOS())
			setState(MOVETOLOS);
		break;
	case LOOKINGPLAYER:
		m_MeleeTree->solveTree();
		LookPlayer();
		break;
	case DEATH:
		Die();
		break;
	case FLEE:
		if (MoveToFleeLocation())
		{
			setState(PATROL);
		}
	default:
		break;
	}
	if (getLife() <= 0 && m_animationState!= DEATH) {
		setState(DEATH);
		SoundManager::PlaySound("death", 0, -1);
	}
	Animate();
	for (auto s : UIList)
	{
		s->update(this);
	}
}

void MeleeEnemy::setState(AnimationState state)
{
	m_animationState = state;
	switch (m_animationState)
	{
	case IDLE:
		m_src = { 0 , 0 , 291,226 };
		m_spriteMax = 19;
		break;
	case PATROL:
		setDestinations();
		m_src = { 0 , 528,281,221 };
		m_spriteMax = 19;
		break;
	case MOVETOLOS:
		buildPathToLOS();
		m_src = { 0 , 528,281,221 };
		m_spriteMax = 19;
		break;
	case FLEE:
		buildPathToFlee();
		m_src = { 0 , 528,281,221 };
		m_spriteMax = 19;
		break;
	case MELEE:
		m_src = { 0,226,331,302 };
		m_spriteMax = 14;
		break;

		break;
	default:
		m_src = { 0 , 528,281,221 };
		m_spriteMax = 19;
		break;
	}
	m_sprite = 0;
}

void MeleeEnemy::setDestinations()
{
	m_path.clear();
	m_destinations.clear();
	m_destinations.push_back(LevelManager::m_level[2][2]->Node());
	m_destinations.push_back(LevelManager::m_level[8][2]->Node());
	m_destinations.push_back(LevelManager::m_level[8][8]->Node());
	m_destinations.push_back(LevelManager::m_level[2][8]->Node());
	m_path = LevelManager::calculatePathTo(this, getDestinations()[destinationNumber]);
	pathCounter = 0;
}

bool MeleeEnemy::closeCombatDistance()
{
	if (COMA::CircleCircleCheck(getPosition(), DisplayManager::PlayerList()[0]->getPosition(), 50, 20))
		return true;
	else
		return false;
}

void MeleeEnemy::MeleeAttack()
{
	setState(MELEE);
	SOMA::PlaySound("melee", 0, -1);
	SDL_Texture* MeleeTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/Empty.png");
	MeleeAtk* m_melee = new MeleeAtk({ 0,0,32,32 }, { 0,0,32,32 }, Engine::Instance().GetRenderer(), MeleeTexture, 0, false);
	m_melee->Execute(m_collisionBox, m_angle * 180 / M_PI);
	delete m_melee; //don't need this anymore till we press shoot again.
	m_melee = nullptr;
}
void MeleeEnemy::buildTree()
{
	//ACTIONS
	TreeNode* MeleeAtk = new TreeNode();
	MeleeAtk->Action = [this]() { MeleeAttack();  };

	TreeNode* LookToPlayer = new TreeNode();
	LookToPlayer->Action = [this]() {
		setState(LOOKINGPLAYER);
	};

	TreeNode* MoveToPlayer = new TreeNode();
	MoveToPlayer->Action = [this]() {
		if(m_animationState != RUNNING)
			setState(RUNNING);
	};

	TreeNode* MoveToLOS = new TreeNode();
	MoveToLOS->Action = [this]() { 
		if(m_animationState != MOVETOLOS)
			setState(MOVETOLOS);
	};

	TreeNode* Flee = new TreeNode();
	Flee->Action = [this]() { 
		if (m_animationState != FLEE)
			setState(FLEE); 
	};

	TreeNode* Patrol = new TreeNode();
	Patrol->Action = [this]() {
		if (m_animationState != PATROL)
			setState(PATROL); 
	};

	/////////////////////////////////////////////////////////////////////////////////
	TreeNode* canLook = new TreeNode();
	canLook->Condition = [this]() {
		return canLookPlayer();
	};
	canLook->Left = LookToPlayer;
	canLook->Right = MoveToLOS;

	TreeNode* inCCRange = new TreeNode();
	inCCRange->Condition = [this]() {
		return closeCombatDistance();
	};
	inCCRange->Left = MeleeAtk;
	inCCRange->Right = MoveToPlayer;

	TreeNode* inRadius = new TreeNode();
	inRadius->Condition = [this]() {
		return m_inRadius;
	};
	inRadius->Left = canLook;
	inRadius->Right = Patrol;

	TreeNode* inLOS = new TreeNode();
	inLOS->Condition = [this]() {
		return m_inLOS;
	};
	inLOS->Left = inCCRange;
	inLOS->Right = inRadius;

	TreeNode* NotlowHp = new TreeNode();
	NotlowHp->Condition = [this]() {
		if (m_Life > 25)
			return true;
		else
			return false;
	};
	NotlowHp->Left = inLOS;
	NotlowHp->Right = Flee;

	m_MeleeTree->setRoot(NotlowHp);

}

void MeleeEnemy::buildMoveToLOSTree()
{
	/*TreeNode* MoveToNodeWLOS = new TreeNode();
	MoveToNodeWLOS->Action = [this]() {
		if (MoveToLOS())
		{
			setState(MOVETOLOS);
		}
	};

	TreeNode* LookPlayer = new TreeNode();
	LookPlayer->Action = [this]() {
		Enemy::LookPlayer();
	};

	TreeNode* onNodeWLOS = new TreeNode();
	onNodeWLOS->Condition = [this]() {
		return OnTopOfNodeWLOS();
	};
	onNodeWLOS->Left = LookPlayer;
	onNodeWLOS->Right = MoveToNodeWLOS;

	m_MoveToLOSTree->setRoot(onNodeWLOS);*/

}
