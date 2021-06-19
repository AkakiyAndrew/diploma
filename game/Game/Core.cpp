#include "GameClasses.h"

Core::Core(GameData* ptr, ActorType type, Vector2 pos, State state)
	:Constructor(ptr, type, pos, state),
	Connectable(ptr, type)
{
	
}

void Core::Update()
{
	switch (state)
	{
	case State::ONLINE:
		if (parent != nullptr)
		{
			//if connected - routine work
			Expand();

			SeekForTarget();

			if (game->timeCount == 0)
			{
				BuildOrRepair();
			}
		}
		else
		{
			//try to connect, if cant - change state to OFFLINE and release expanded area free
			if (TryConnect(position, ID) != true)
			{
				state = State::OFFLINE;
				markAreaFade();
			}
		}
		break;
	case State::OFFLINE:
		//constant trying to connect, if it possible - remark expansion area
		if (TryConnect(position, ID) == true)
		{
			state = State::ONLINE;
			markAreaExpand();
		}
		break;
	case State::UNDER_CONSTRUCTION:
		if (HP == maxHP)
		{
			state = State::ONLINE;
			markAreaExpand();
			game->revealTerritory(positionIndex, sightRange, side);
		}
			
		break;
	default:
		break;
	}
	isInBattleCheck();
}

//dark magic
void Core::Disconect()
{
	if (parent != nullptr)
		parent->UnAttach(this);
	parent = nullptr;

	std::vector<Connectable*> buf = connectedUnits;
	for (Connectable* unit : buf)
	{
		unit->Disconect();
	}
	state = State::OFFLINE;
	markAreaFade();
}

void Core::Draw()
{
	DrawCircle(position.x, position.y, size, SKYBLUE);

	//draw connection line
	if (parent != nullptr)
	{
		DrawLineEx(position, parent->getPosition(), 2, WHITE);
	}
	
	if(state==State::ONLINE)
		DrawCircle(position.x, position.y, size / 2, GREEN);

	if (state == State::OFFLINE)
		DrawCircle(position.x, position.y, size / 2, ORANGE);

	if (state == State::UNDER_CONSTRUCTION)
		DrawCircle(position.x, position.y, size / 2, RED);

	drawHP();
	DrawBuildingRay();
}

void Core::Destroy()
{

}