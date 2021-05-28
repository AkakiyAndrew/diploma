#include "GameClasses.h"

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

	Expand();

	
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