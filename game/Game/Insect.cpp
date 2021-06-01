#include "GameClasses.h"

Insect::Insect(GameData* ptr, ActorType type, Vector2 pos, State state)
    :Militaty(ptr, type, pos, state)
{

}

void Insect::Update()
{
    switch (state)
    {
    case State::ONLINE:
        state = State::GOES;
        sprite = game->getUnitAnimation(type, state);
        break;
    case State::ATTACKING:
        break;
    case State::GOES:
        Move();
        break;
    default:
        break;
    }
    
    if (game->timeCount % 15 == 0)
        currentFrame++;
    if (currentFrame >= sprite.framesAmount)
        currentFrame = 0;
    //Move();
}

void Insect::Draw()
{
    DrawTexturePro(
        sprite.frames[currentFrame], //texture
        Rectangle{ 0,0, (float)sprite.frames[currentFrame].width, (float)sprite.frames[currentFrame].height }, //source
        Rectangle{ position.x, position.y, (float)size*2.f, (float)size*2.f }, //dest
        Vector2{ (float)(size), (float)(size) }, //origin
        (float)angle, //rotation
        WHITE);

    //DrawCircle(position.x, position.y, size, ORANGE);

    //float** terrainMod = game->mapsPathfinding[type]["mapsTerrainMod"];
    ////TODO: move method to new Insect class?
    //TileIndex tile = game->getTileIndex(position);
    ////collision avoidance
    //Vector2 ahead = { position.x + velocityVector.x * 4.f, position.y + velocityVector.y * 4.f };
    //DrawCircle(ahead.x, ahead.y, 1, RED);
    //DrawCircleLines(ahead.x, ahead.y, size, RED);
    //DrawCircle(position.x, position.y, 1, ORANGE);
    DrawCircleLines(position.x, position.y, size, ORANGE);
    DrawCircle(position.x + velocityVector.x, position.y + velocityVector.y, 1, BLUE);
}

void Insect::Destroy()
{

}