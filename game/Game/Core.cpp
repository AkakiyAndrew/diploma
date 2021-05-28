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
		}
			
		break;
	default:
		break;
	}

	//Expand();
	
}

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
	
	//game->mapExpansionCreep[positionIndex.x][positionIndex.y] = ExpandState::EXPANDED_WITHOUT_SOURCE;

}

void Core::Draw()
{
	DrawCircle(position.x, position.y, size, SKYBLUE);
	if (parent != nullptr)
	{
		DrawLineEx(position, parent->getPosition(), 2, WHITE);
		DrawCircle(position.x, position.y, size/2, GREEN);
	}
	else
		DrawCircle(position.x, position.y, size/2, RED);

}

void Core::Destroy()
{

}