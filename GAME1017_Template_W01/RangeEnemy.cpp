#include "RangeEnemy.h"
#include "LifeBar.h"
#include "SoundManager.h"
#include"DisplayManager.h"
#include "CollisionManager.h"
#include "Engine.h"
#include "Bullet.h"
#define SPEED 2

RangeEnemy::RangeEnemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf)
	:Enemy(s, d, r, t, sstart, smin, smax, nf)
{
	setDestinations();
	m_RangeTree = new DecisionTree();
	buildTree();
	setState(PATROL);
	m_shootTimer = 0;
}
void RangeEnemy::update()
{
	updatePosition();
	updateCollisionBox(40.0f, 40.0f);
	updateLOS(DisplayManager::PlayerList());
	updateLOSToPlayerInsideRadius();
	updateRAD();

	switch (m_animationState)
	{
	case PATROL:

		m_RangeTree->solveTree();
		Patrol();
		break;
	case RUNNING:
		m_RangeTree->solveTree();
		MoveToPlayer();
		break;
	case MOVETOLOS:
		if (MoveToLOS())
			setState(MOVETOLOS);
		m_RangeTree->solveTree();
		break;
	case MOVETONOLOS:
		m_RangeTree->solveTree();
		if (MoveToLOS()) { //same method as LOS
			setState(MOVETONOLOS);
			m_gettingHit = false;
		}
		break;
	case LOOKINGPLAYER:
		m_RangeTree->solveTree();
		LookPlayer();
		break;
	case SHOOTING:
		if (m_shootTimer > 30)
		{
			RangeAttack();
			m_shootTimer = 0;
		}
		m_RangeTree->solveTree();
		m_shootTimer++;
		break;
	case FLEE:
		if (MoveToFleeLocation())
			setState(PATROL);
		break;
	case DEATH:
		Die();
		break;
	default:
		break;
	}
	if (getLife() <= 0 && m_animationState != DEATH) {
		setState(DEATH);
		SoundManager::PlaySound("death", 0, -1);
	}
	for (auto s : UIList)
	{
		s->update(this);
	}
	Animate();
}

void RangeEnemy::setState(AnimationState state)
{
	m_animationState = state;
	switch (m_animationState)
	{
	case IDLE:
		m_src = { 0 , 412 , 313,206 };
		m_spriteMax = 19;
		break;
	case PATROL:
		setDestinations();
		m_src = { 0 , 0,313,206 };
		m_spriteMax = 19;
		break;
	case MOVETOLOS:
		buildPathToLOS();
		m_src = { 0 , 0,313,206 };
		m_spriteMax = 19;
		break;
	case MOVETONOLOS:
		buildPathToNOLOS();
		m_src = { 0 , 0,313,206 };
		m_spriteMax = 19;
		break;
	case FLEE:
		buildPathToFlee();
		m_src = { 0 , 0,313,206 };
		m_spriteMax = 19;
		break;
	case SHOOTING:
		m_src = { 0 ,206,316,206 };
		m_spriteMax = 2;
		break;
	default:
		m_src = { 0 , 0,313,206 };
		m_spriteMax = 19;
		break;
	}
	m_sprite = 0;
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
	if (COMA::CircleCircleCheck(getPosition(), DisplayManager::PlayerList()[0]->getPosition(), 200, 20))
		return true;
	else
		return false;
}

bool RangeEnemy::wasHit()
{
	return m_gettingHit;
}

void RangeEnemy::RangeAttack()
{	
	setState(SHOOTING);
	SDL_Texture* BulletTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/bullet1.png");
	Bullet* m_shoot = new Bullet({ 0,0,813,306 }, { 0,0,20,8 }, Engine::Instance().GetRenderer(), BulletTexture, 0, false);
	m_shoot->Execute(m_collisionBox, m_angle * 180 / M_PI);
	delete m_shoot; //don't need this anymore till we press shoot again.
	m_shoot = nullptr;
}

void RangeEnemy::buildTree()
{
	//Actions
	TreeNode* RangeAttack = new TreeNode();
	RangeAttack->Action = [this]() {
		if (m_animationState != SHOOTING)
			setState(SHOOTING);
	};

	TreeNode* LookToPlayer = new TreeNode();
	LookToPlayer->Action = [this]() {
		setState(LOOKINGPLAYER);
	};

	TreeNode* MoveToRange = new TreeNode();
	MoveToRange->Action = [this]() {
		if (m_animationState != RUNNING)
			setState(RUNNING);
	};

	TreeNode* MoveToLOS = new TreeNode();
	MoveToLOS->Action = [this]() {
		if (m_animationState != MOVETOLOS)
			setState(MOVETOLOS);
	};

	TreeNode* MoveToCover = new TreeNode();
	MoveToCover->Action = [this]() {
		if (m_animationState != MOVETONOLOS)
			setState(MOVETONOLOS);
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

	// Conditions

	TreeNode* canLook = new TreeNode();
	canLook->Condition = [this]() {
		return canLookPlayer();
	};
	canLook->Left = LookToPlayer;
	canLook->Right = MoveToLOS;

	TreeNode* inCCRange = new TreeNode();
	inCCRange->Condition = [this]() { return WeaponRangeDistance(); };
	inCCRange->Left = RangeAttack;
	inCCRange->Right = MoveToRange;

	TreeNode* inRadius = new TreeNode();
	inRadius->Condition = [this]() { return m_inRadius; };
	inRadius->Left = canLook;
	inRadius->Right = Patrol;

	TreeNode* inLOS = new TreeNode();
	inLOS->Condition = [this]() { return m_inLOS; };
	inLOS->Left = inCCRange;
	inLOS->Right = inRadius;

	TreeNode* NotHit = new TreeNode();
	NotHit->Condition = [this]() { return wasHit(); };
	NotHit->Left = MoveToCover;
	NotHit->Right = inLOS;

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
	/*TreeNode* MoveToNodeWLOS = new TreeNode();
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

	m_MoveToLOSTree->setRoot(onNodeWLOS);*/
}
