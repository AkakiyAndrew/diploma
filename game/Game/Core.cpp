#include "GameClasses.h"

void Core::Update()
{
	if (this->game->timeCount % 2 == 0) // expand every 1 s
	{
		Expand();
	}
}
void Core::Draw()
{
	DrawCircle(position.x, position.y, size, SKYBLUE);
}

Core::Core(GameData* ptr, ActorType type, Vector2 pos, State state)
	:Constructor::Constructor(ptr, type, pos, state)
{
}

void Core::Destroy()
{
}