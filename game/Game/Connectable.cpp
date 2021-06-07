#include "GameClasses.h"

Connectable::Connectable(GameData* ptr, ActorType type)
{
	gameConn = ptr;
	connectRange = gameConn->connectableAttributes[type]["connectRange"];
}

bool Connectable::TryConnect(Vector2 position, int ID)
{
	bool result = false;

	std::vector<GameActor*> buf = gameConn->getActorsInRadius(position, connectRange);
	for (GameActor* actor : buf)
	{
		//seek for first Core or Base, that operational and NOT the same unit which connecting, and connect to it
		if ((actor->type == ActorType::CORE || actor->type == ActorType::BASE) 
			&& actor->getState() == State::ONLINE
			&& actor->ID !=ID)
		{
			//request to add this unit to parent`s vector of connected units, and check if this unit parent for connected or not
			if (static_cast<Constructor*>(actor)->RequestAttachment(this))
			{
				parent = (Constructor*)actor;
				break;
			}
		}
	}

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