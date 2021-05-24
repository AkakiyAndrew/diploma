#include "GameClasses.h"

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
    float** terrainMod = game->mapsPathfinding[type]["mapsTerrainMod"];
    //TODO: move method to new Insect class?
    TileIndex tile = game->getTileIndex(position);

    //collision avoidance
    Vector2 ahead = { position.x + velocityVector.x * 4.f, position.y + velocityVector.y * 4.f };

    DrawCircle(ahead.x, ahead.y, 1, RED);
    DrawCircleLines(ahead.x, ahead.y, size, RED);

    DrawCircle(position.x, position.y, 1, ORANGE);
    DrawCircleLines(position.x, position.y, size, ORANGE);

    DrawCircle(position.x + velocityVector.x, position.y + velocityVector.y, 1, BLUE);
}

void LightInsect::Destroy()
{

}