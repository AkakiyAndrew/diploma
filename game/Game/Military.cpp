#include "GameClasses.h"

Militaty::Militaty(GameData* ptr, ActorType type, Vector2 pos, State state)
	:GameActor(ptr, type, pos, state)
{
	seekRange = ptr->militaryAttributes[type]["seekRange"];
    attackRange = ptr->militaryAttributes[type]["attackRange"];
    speed = ptr->militaryAttributes[type]["speed"];
    damage = ptr->militaryAttributes[type]["damage"];
    cooldownDuration = ptr->militaryAttributes[type]["cooldownDuration"];
    rotationSpeed = static_cast<float>(ptr->militaryAttributes[type]["rotationSpeed"])/100.f;

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

bool Militaty::Targeting()
{
    bool result = true;

    Vector2 targetPosition = target->getPosition();
    float destinationRotation = atan2f(position.y - targetPosition.y, position.x - targetPosition.x) + PI;
    float angleTan = angle * PI / 180.f;

    if (destinationRotation != angleTan)
    {
        if (fmaxf(destinationRotation - angleTan, angleTan - destinationRotation) < rotationSpeed)
        {
            angleTan = destinationRotation;
        }
        else
        {
            if (destinationRotation > angleTan)
            {
                if (angleTan < destinationRotation - PI)
                    angleTan -= rotationSpeed;
                else
                    angleTan += rotationSpeed;
            }
            else
            {
                if (angleTan > destinationRotation + PI)
                    angleTan += rotationSpeed;
                else
                    angleTan -= rotationSpeed;
            }

            if (angleTan > PI * 2.0f) angleTan = 0;
            if (angleTan < 0) angleTan = PI * 2.0f;
        }
        angle = angleTan * 180.0 / PI;

        result = destinationRotation == angleTan;
    }

    return result;
}

Militaty::~Militaty()
{

}