#include "GameClasses.h"

Connectable::Connectable(GameData* ptr, ActorType type)
{
	game = ptr;
	connectRange = game->connectableAttributes[type]["connectRange"];
}

bool Connectable::TryConnect(Vector2 position, int ID)
{
	bool result = false;

	//???
	//seek for Core
	std::vector<GameActor*> buf = game->getActorsInRadius(position, connectRange * game->pixelsPerTile);

	for (GameActor* actor : buf)
	{
		//seek for first Core or Base, that operational, and connect to it
		if ((actor->type == ActorType::CORE || actor->type == ActorType::BASE) 
			&& actor->getState() == State::ONLINE
			&& actor->ID !=ID)
		{
			if (static_cast<Constructor*>(actor)->RequestAttachment(this))
			{
				parent = (Constructor*)actor;
				break;
			}
		}
	}

	//if(parent==nullptr)
	//	//if cant find, seek for Base
	//	parent = (Constructor*)game->getNearestSpecificActor(position, game->getActorsInRadius(position, connectRange), ActorType::BASE, (GameActor*)this);

	if (parent != nullptr)
		result = true;

	return result;
}

void Connectable::Disconect()
{
	if (parent != nullptr)
		parent->UnAttach(this);
	parent = nullptr;
}

Connectable::~Connectable()
{
	Disconect();
}