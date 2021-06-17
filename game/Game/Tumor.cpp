#include "GameClasses.h"

Tumor::Tumor(GameData* ptr, ActorType type, Vector2 pos, State state)
	:Building(ptr, type, pos, state)
{
}

void Tumor::Update()
{
	switch (state)
	{
	case State::ONLINE:

		//regeneration every second, if builded
		if (this->game->timeCount == 0)
			this->RestoreHP(1);

		Expand();
		break;
	case State::UNDER_CONSTRUCTION:
		//building 
		if (this->game->timeCount % 5 == 0)
			RestoreHP(1);
		if (HP == maxHP)
		{
			//if it ready - mark area around to expand and create initiate expansion tile
			state = State::ONLINE;
			markAreaExpand();
		}

		break;
	default:
		break;
	}
}
void Tumor::Draw()
{
	DrawCircle(position.x, position.y, size, RED);
	if (state == State::UNDER_CONSTRUCTION)
		DrawCircle(position.x, position.y, size / 2, ORANGE);
	else
		DrawCircle(position.x, position.y, size / 2, PURPLE);
	drawHP();
}

void Tumor::Destroy()
{
}