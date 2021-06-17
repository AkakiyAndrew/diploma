#include "GameClasses.h"

Hive::Hive(GameData* ptr, ActorType type, Vector2 pos, State state)
	:Building(ptr, type, pos, state)
{
}

void Hive::Update()
{
	switch (state)
	{
	case State::ONLINE:

		//regeneration every second, if builded
		if (this->game->timeCount == 0)
			this->RestoreHP(1);

		Expand();
		break;
	default:
		break;
	}
}

void Hive::SpawnUnits()
{

}

void Hive::Draw()
{
	DrawCircle(position.x, position.y, size, RED);
	drawHP();
}

void Hive::Destroy()
{
}