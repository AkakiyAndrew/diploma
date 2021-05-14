#include "GameClasses.h"

Militaty::Militaty(GameData* ptr, ActorType type, Vector2 pos, State state)
	:GameActor(ptr, type, pos, state)
{
	seekRange = ptr->militaryAttributes[type]["seekRange"];
    attackRange = ptr->militaryAttributes[type]["attackRange"];
    speed = ptr->militaryAttributes[type]["speed"];
    damage = ptr->militaryAttributes[type]["damage"];
    reloadCount = ptr->militaryAttributes[type]["reloadCount"];
    rotationSpeed = ptr->militaryAttributes[type]["rotationSpeed"];
}

Militaty::~Militaty()
{

}

void Militaty::Move()
{
    //TODO: move method to new Insect class?
    TileIndex tile = game->getTileIndex(position);
    Vector2 velocityVector = game->vectorFields[type][tile.x][tile.y];

    //unit and terrain speed modifications
    velocityVector.x *= speed * game->mapsPathfinding[type]["mapsTerrainMod"][tile.x][tile.y];
    velocityVector.y *= speed * game->mapsPathfinding[type]["mapsTerrainMod"][tile.x][tile.y];

    //creep speed mod
    if (game->isTileExpanded(tile, side) && type != ActorType::FLYING_INSECT)
    {
        velocityVector.x *= 1.25f;
        velocityVector.y *= 1.25f;
    }

    //collision avoidance
    Vector2 ahead = { position.x + velocityVector.x * (size * 2), position.y + velocityVector.y * (size * 2) };
    Vector2 steering = { 0.f, 0.f };

    //GameActor* nearestObstacle = game->getNearestSpecificActor(ahead, game->getActorsInRadius(ahead, size), type, this);
    //if (nearestObstacle != nullptr)
    //{
    //    if (CheckCollisionCircles(ahead, size, nearestObstacle->getPosition(), nearestObstacle->size))
    //    {
    //        float R = sqrt(pow(ahead.x - nearestObstacle->getPosition().x, 2) + pow(ahead.y - nearestObstacle->getPosition().y, 2));
    //        steering.x += (ahead.x - nearestObstacle->getPosition().x)/R;
    //        steering.y += (ahead.y - nearestObstacle->getPosition().y)/R;
    //    }
    //}

    //GameActor* nearestObstacle = game->getNearestSpecificActor(position, game->getActorsInRadius(position, size*2), type, this);
    for (GameActor* colActor : game->getActorsInRadius(position, size * 2))
    {
        if (colActor != this)
        {
            if (CheckCollisionCircles(position, size, colActor->getPosition(), colActor->size))
            {
                float R = sqrt(pow(position.x - colActor->getPosition().x, 2) + pow(position.y - colActor->getPosition().y, 2));  //distance between circles
                steering.x += (position.x - colActor->getPosition().x) / R;
                steering.y += (position.y - colActor->getPosition().y) / R;
                //steering.x += position.x - size - nearestObstacle->getPosition().x;
                //steering.y += position.y - size - nearestObstacle->getPosition().y;
            }
        }
    }
    //std::clamp <float>(steering.x, -1.f, 1.f);
    //std::clamp <float>(steering.y, -1.f, 1.f);

    velocityVector.x+= steering.x;
    velocityVector.y+= steering.y;

    std::clamp <float>(velocityVector.x, -speed, speed);
    std::clamp <float>(velocityVector.y, -speed, speed);

    position.x += velocityVector.x;
    position.y += velocityVector.y;
}

LightInsect::LightInsect(GameData* ptr, ActorType type, Vector2 pos, State state)
    :Militaty(ptr, type, pos, state)
{

}

void LightInsect::Update()
{
    Move();
}

void LightInsect::Draw()
{
    DrawCircle(position.x, position.y, size, ORANGE);
}

void LightInsect::Destroy()
{

}