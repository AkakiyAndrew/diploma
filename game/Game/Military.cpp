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
    float** terrainMod = game->mapsPathfinding[type]["mapsTerrainMod"];
    //TODO: move method to new Insect class?
    positionIndex = game->getTileIndex(position);

    game->revealTerritory(positionIndex, sightRange, side);

    velocityVector = game->vectorFields[type][positionIndex.x][positionIndex.y];
    float speedMod = terrainMod[positionIndex.x][positionIndex.y];

    //creep speed mod
    if (game->isTileExpanded(positionIndex, side) && type != ActorType::FLYING_INSECT)
    {
        speedMod *= 1.25f;
    }

    velocityVector.x *= speed;
    velocityVector.y *= speed;

    //collision avoidance
    Vector2 ahead = { position.x + velocityVector.x * 4.f, position.y + velocityVector.y  * 4.f };
    Vector2 steering = { 0.f, 0.f };

    //UNIT STEERING AHEAD
    GameActor* nearestObstacle = game->getNearestSpecificActor(ahead, game->getActorsInRadius(ahead, size), type, this);
    if (nearestObstacle != nullptr)
    {
        float R = sqrt(pow(ahead.x - nearestObstacle->getPosition().x, 2) + pow(ahead.y - nearestObstacle->getPosition().y, 2));
        if (R != 0.f)
        {
            steering.x += (ahead.x - nearestObstacle->getPosition().x)/R;
            steering.y += (ahead.y - nearestObstacle->getPosition().y)/R;
        }
    }

    //NEARBY UNITS COLLISION CHECK
    for (GameActor* colActor : game->getActorsInRadius(position, size * 2))
    {
        if (colActor != this)
        {
            if (CheckCollisionCircles(position, size, colActor->getPosition(), colActor->size))
            {
                float R = sqrt(pow(position.x - colActor->getPosition().x, 2) + pow(position.y - colActor->getPosition().y, 2));  //distance between circles
                if (R != 0.f)
                {
                    steering.x += (position.x - colActor->getPosition().x) / R;
                    steering.y += (position.y - colActor->getPosition().y) / R;
                }
            }
        }
    }

    //WALLS CHECKING AHEAD
    TileIndex tileAhead = game->getTileIndex(ahead);
    std::vector<TileIndex> nearTiles = game->getNeighborsAsVector(tileAhead.x, tileAhead.y);
    Rectangle tileBuf;

    for (TileIndex checkingTile : nearTiles)
    {
        tileBuf = Rectangle{ checkingTile.x * game->pixelsPerTile, checkingTile.y * game->pixelsPerTile, game->pixelsPerTile, game->pixelsPerTile };
        if (terrainMod[checkingTile.x][checkingTile.y] == -1.f && CheckCollisionCircleRec(ahead, size, tileBuf))
        {
            //if positionIndex near is collides with actor
            float R = sqrt(pow(ahead.x - (tileBuf.x + (tileBuf.width / 2.f)), 2) + pow(ahead.y - (tileBuf.y + (tileBuf.height / 2.f)), 2));
            if (R != 0.f)
            {
                steering.x += (ahead.x - (tileBuf.x + (tileBuf.width / 2.f))) / R;
                steering.y += (ahead.y - (tileBuf.y + (tileBuf.height / 2.f))) / R;
            }
        }
    }

    //std::clamp <float>(steering.x, -1.f, 1.f);
    //std::clamp <float>(steering.y, -1.f, 1.f);

    velocityVector.x+= steering.x;
    velocityVector.y+= steering.y;

    std::clamp <float>(velocityVector.x, -speed, speed);
    std::clamp <float>(velocityVector.y, -speed, speed);

    if (speedMod > 0)
    {
        velocityVector.x *= speedMod;
        velocityVector.y *= speedMod;
    }

    position.x += velocityVector.x;
    position.y += velocityVector.y;

    //WALLS COLLISION CHECK
    nearTiles = game->getNeighborsAsVector(positionIndex.x, positionIndex.y);

    for (TileIndex checkingTile : nearTiles)
    {
        tileBuf = Rectangle{ checkingTile.x * game->pixelsPerTile, checkingTile.y * game->pixelsPerTile, game->pixelsPerTile, game->pixelsPerTile };
        if (terrainMod[checkingTile.x][checkingTile.y] == -1.f && CheckCollisionCircleRec(position, size, tileBuf))
        {
            //if positionIndex near is collides with actor
            float R = sqrt(pow(position.x - (tileBuf.x + (tileBuf.width / 2.f)), 2) + pow(position.y - (tileBuf.y + (tileBuf.height / 2.f)), 2));
            if (R != 0.f)
            {
                position.x += (position.x - (tileBuf.x + (tileBuf.width / 2.f))) / R;
                position.y += (position.y - (tileBuf.y + (tileBuf.height / 2.f))) / R;
            }
        }
    }

    //MAP BOUNDS CHECK
    if (position.x < 0 + size)
        position.x = 0 + size;
    if (position.y < 0 + size)
        position.y = 0 + size;
    if (position.x > game->getMaxWidth()*game->pixelsPerTile - size)
        position.x = game->getMaxWidth() * game->pixelsPerTile - size;
    if (position.y > game->getMaxHeight() * game->pixelsPerTile - size)
        position.y = game->getMaxHeight() * game->pixelsPerTile - size;

    
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
    float** terrainMod = game->mapsPathfinding[type]["mapsTerrainMod"];
    //TODO: move method to new Insect class?
    TileIndex tile = game->getTileIndex(position);

    //collision avoidance
    Vector2 ahead = { position.x + velocityVector.x * 4.f, position.y + velocityVector.y  * 4.f };

    DrawCircle(ahead.x, ahead.y, 1, RED);
    DrawCircleLines(ahead.x, ahead.y, size, RED);

    DrawCircle(position.x, position.y, 1, ORANGE);
    DrawCircleLines(position.x, position.y, size, ORANGE);

    DrawCircle(position.x+velocityVector.x, position.y + velocityVector.y, 1, BLUE);
}

void LightInsect::Destroy()
{

}