#include "GameClasses.h"

Base::Base(GameData* ptr, ActorType type, Vector2 pos, State state)
	:Constructor(ptr, type, pos, state)
{

}

void Base::Update()
{
	//switch (state)
	//{
	//case State::ONLINE:
	//	if (parent != nullptr)
	//	{
	//		//if connected - routine work
	//		if (this->game->timeCount % 2 == 0) // expand every 1 s
	//		{
	//			Expand();
	//		}
	//	}
	//	else
	//	{
	//		//try to connect, if cant - change state to OFFLINE and release expanded area free
	//		if (TryConnect() != true)
	//		{
	//			state = State::OFFLINE;
	//			markAreaFade();
	//		}
	//	}
	//	break;
	//case State::OFFLINE:
	//	//constant trying to connect, if it possible - remark expansion area
	//	if (TryConnect() == true)
	//	{
	//		state = State::ONLINE;
	//		markAreaExpand();
	//	}
	//	break;
	//case State::UNDER_CONSTRUCTION:
	//	if (HP == maxHP)
	//	{
	//		state = State::ONLINE;
	//		markAreaExpand();
	//	}
	//		
	//	break;
	//default:
	//	break;
	//}

	Expand();

	SeekForTarget();

	isInBattleCheck();
	if (game->timeCount == 0)
	{
		BuildOrRepair();
	}
}
void Base::Draw()
{
	DrawCircle(position.x, position.y, size, BLUE);
	drawHP();
	DrawBuildingRay();
}

void Base::Destroy()
{
}