#include "GameClasses.h"

Base::Base(GameData* ptr, ActorType type, Vector2 pos, State state)
	:Constructor(ptr, type, pos, state)
{

}

void Base::Update()
{
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