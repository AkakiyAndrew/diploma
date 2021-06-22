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
	std::vector<ActorType> unitTypes = { ActorType::LIGHT_INSECT, ActorType::HEAVY_INSECT, ActorType::FLYING_INSECT };
	int currentWeight = 0;
	int foodAmount = game->getAmountOfResources(side);

	for (ActorType unitType : unitTypes)
	{
		int unitCost = game->genericAttributes[unitType]["cost"];
		int amountToSpawn = (foodAmount * game->weights[currentWeight])/ unitCost;

		for (int i = 0; i < amountToSpawn; i++)
		{
			if (game->trySpendResources(unitCost, side) == unitCost)
			{
				game->addActor(unitType, position, State::GOES);
				game->spendResources(unitCost, side);
			}
		}

		currentWeight++;
	}
}

void Hive::Draw()
{
	DrawCircle(position.x, position.y, size, RED);
	drawHP();
}

void Hive::Destroy()
{
}