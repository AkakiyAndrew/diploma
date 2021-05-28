#include "GameClasses.h"

Constructor::Constructor(GameData* ptr, ActorType type, Vector2 pos, State state)
    :Building(ptr, type, pos, state)
{
    buildRate = ptr->constructorsAttributes[type]["buildRate"];
    buildRange = ptr->constructorsAttributes[type]["buildRange"];
}

void Constructor::BuildOrRepair()
{
    //if have target
    if (target != nullptr)
    {
        //repair, if target fully repaired - nullify pointer
        //TODO: consider using energy
        target->RestoreHP(buildRate);
        if (target->getHP() == target->maxHP)
            target = nullptr;
    }
    //seeking for target to repair must be in Update()!
}

bool Constructor::RequestAttachment(Connectable* unit)
{
    bool result = true;
    
    for (Connectable* connected : connectedUnits)
    {
        if (connected == unit)
        {
            result = false;
            break;
        }
    }

    if (result == true)
        connectedUnits.push_back(unit);

    return result;
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
    std::vector<Connectable*> buf = connectedUnits;
    for (Connectable* unit : buf)
    {
        unit->Disconect();
    }
}