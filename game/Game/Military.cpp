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
    TileIndex tile = game->getTileIndex(position);
    Vector2 velocityVector = game->vectorFields[type][tile.x][tile.y];
    this->position.x += speed * velocityVector.x;
    this->position.y += speed * velocityVector.y;
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
    DrawCircle(position.x, position.y, size * game->pixelsPerTile, ORANGE);
}

void LightInsect::Destroy()
{

}