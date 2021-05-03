#include "GameClasses.h"

void Tumor::Update()
{
	if (this->game->timeCount %2 == 0) // expand every 1 s
	{
		Expand();
	}
}
void Tumor::Draw()
{
	DrawCircle(position.x, position.y, size, RED);
}

Tumor::Tumor(GameData* ptr, ActorType type, Vector2 pos, State state) 
	:Building(ptr, type, pos, state)
{
}

void Tumor::Destroy()
{
}