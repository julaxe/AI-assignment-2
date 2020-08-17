#include "EscapeButton.h"
#include "Engine.h"

void EscapeButton::Execute()
{
	Engine::Instance().Running() = false;
}
