#include "GameClasses.h"

Militaty::Militaty(GameData* ptr, ActorType type, Vector2 pos, State state)
	:GameActor(ptr, type, pos, state)
{
	seekRange = ptr->militaryAttributes[type]["seekRange"];
    attackRange = ptr->militaryAttributes[type]["attackRange"];
    speed = ptr->militaryAttributes[type]["speed"];
    damage = ptr->militaryAttributes[type]["damage"];
    cooldownDuration = ptr->militaryAttributes[type]["cooldownDuration"];
    rotationSpeed = ptr->militaryAttributes[type]["rotationSpeed"];

    cooldownRemain = cooldownDuration;
}

void Militaty::SeekForEnemy()
{
    target = nullptr;

    //what actors will be looking for
    Side enemy;
    if (side == Side::INSECTS)
        enemy = Side::MACHINES;
    else
        enemy = Side::INSECTS;

    std::vector<GameActor*> buf;

    for (GameActor* actor : game->getActorsInRadius(position, seekRange)) // TODO: seek for nearest?
    {
        if (actor->side == enemy 
            && game->isOnLineOfSight(positionIndex, actor->getPositionIndex(), type)
            /*&& game->getTileIndex(actor->getPosition())*/) //FOG OF WAR?
        {
            buf.push_back(actor);
            /*target = actor;
            break;*/
        }
    }

    if (buf.size() != 0)
        target = game->getNearestSpecificActor(position, buf, this);
}

void Militaty::Reload()
{
    if (cooldownRemain > 0)
        cooldownRemain--;
}

void Militaty::Targeting()
{
    Vector2 targetPosition = target->getPosition();
    int destinationRotation = atan2f(position.y - targetPosition.y, position.x - targetPosition.x) + PI;

    if (destinationRotation != angle)
    {
        if (destinationRotation > angle)
        {
            if (angle < destinationRotation - PI)
                angle -= rotationSpeed;
            else
                angle += rotationSpeed;
        }
        else //if (destinationRotation < topRotation)
        {
            if (angle > destinationRotation + PI)
                angle += rotationSpeed;
            else
                angle -= rotationSpeed;
        }

        if (angle > PI * 2.0f) angle = 0;
        if (angle < 0) angle = PI * 2.0f;
    }
}

Militaty::~Militaty()
{

}