#include "PlayButton.h"
#include"StateManager.h"
#include"Engine.h"
#include"PlayState.h"

void PlayButton::Execute()
{
	std::cout << "Button Pressed" << std::endl;
	StateManager::ChangeState(new PlayState());
}
