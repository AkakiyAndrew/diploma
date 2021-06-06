#include "GameClasses.h"

Constructor::Constructor(GameData* ptr, ActorType type, Vector2 pos, State state)
    :Building(ptr, type, pos, state)
{
    buildPower = ptr->constructorsAttributes[type]["buildPower"];
    buildRange = ptr->constructorsAttributes[type]["buildRange"];
}

void Constructor::BuildOrRepair()
{
    //if have target
    if (target != nullptr)
    {
        //repair, if target fully repaired - nullify pointer
        if (!target->inBattle)
        {
            //determine, how much resources can be spend
            int canSpend = game->trySpendResources(buildPower, side);

            if (canSpend != 0)
            {
                //technical singularity dark magic
                int restored = target->RestoreHP(target->maxHP / (target->cost / canSpend));
                if (restored != 0)
                    game->spendResources(target->maxHP / restored, side);

                //if target hp restored, nullify pointer
                if (target->getHP() == target->maxHP)
                    target = nullptr;
            }
        }
        else
        {
            target = nullptr;
            SeekForTarget();
        }
    }
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

void Constructor::SeekForTarget()
{
    if (target == nullptr)
    {
        for (GameActor* unit : game->getActorsInRadius(position, buildRange))
        {
            if (!unit->inBattle) //check, if this unit attacked
            {
                if (unit->getHP() != unit->maxHP && unit->side == this->side)
                {
                    target = unit;
                    break;
                }
            }
        }
    }
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

void Constructor::DrawBuildingRay()
{
    if (target != nullptr)
    {
        Vector2 offset;
        Vector2 targetPosition = target->getPosition();

        switch (game->timeCount % 9)
        {
        case 0:
            offset = { -1.f,-1.f };
            break;
        case 1:
            offset = { 0.f,-1.f };
            break;
        case 2:
            offset = { 0.f,-1.f };
            break;
        case 3:
            offset = { -1.f,0.f };
            break;
        case 4:
            offset = { 1.f,0.f };
            break;
        case 5:
            offset = { -1.f,1.f };
            break;
        case 6:
            offset = { 0.f,1.f };
            break;
        case 7:
            offset = { 1.f,1.f };
            break;
        default:
            offset = { 0.f,0.f };
            break;
        }

        targetPosition.x += (size / 2) * offset.x;
        targetPosition.y += (size / 2) * offset.y;

        DrawLineEx(position, targetPosition, 2, Color{ 45, 165,235,255 });
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