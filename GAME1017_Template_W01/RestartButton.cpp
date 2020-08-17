#include "RestartButton.h"
#include "StateManager.h"
#include "PlayState.h"
#include "StartScene.h"

void RestartButton::Execute()
{
	STMA::ChangeState(new PlayState());
}
