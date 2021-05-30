#include "GameClasses.h"

Turret::Turret(GameData* ptr, ActorType type, Vector2 pos, State state)
    :Militaty(ptr, type, pos, state), 
    Connectable(ptr, type)
{
    isMounted = true;
    charge = 0;
}

void Turret::Recharge()
{
    if (charge < maxCharge)
    {   
        int spended = game->trySpendResources(std::min(maxCharge - charge, chargeRate), side);
        charge += spended;

        game->spendResources(spended, side);
        
    }
}

void Turret::Update()
{
    if (parent != nullptr)
        Recharge();
}
