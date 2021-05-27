#include "GameClasses.h"

Constructor::Constructor(GameData* ptr, ActorType type, Vector2 pos, State state)
    :Building(ptr, type, pos, state)
{

}

void Constructor::BuildOrRepair()
{
    if (target != nullptr)
    {
        //if have non-null pointer - just repair
        //repair
    }
    //seeking for target to repair must be in Update()!
}

void Constructor::UnAttach(Connectable* unit)
{
    //seek for unit to remove from vector
    std::vector<Connectable*>::iterator iter;
    for (iter = connectedUnits.begin(); iter != connectedUnits.end(); iter++)
    {
        if ((*iter) == unit)
        {
            connectedUnits.erase(iter);
            break;
        }
    }
}

Constructor::~Constructor()
{

}