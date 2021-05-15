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
    TileIndex tile = game->getTileIndex(position);
    Vector2 velocityVector = game->vectorFields[type][tile.x][tile.y];
    float speedMod = terrainMod[tile.x][tile.y];

    //creep speed mod
    if (game->isTileExpanded(tile, side) && type != ActorType::FLYING_INSECT)
    {
        speedMod *= 1.25f;
    }

    velocityVector.x *= speed;
    velocityVector.y *= speed;

    //collision avoidance
    Vector2 ahead = { position.x + velocityVector.x * 2, position.y + velocityVector.y  * 2 };
    Vector2 steering = { 0.f, 0.f };

    //WALLS COLLISION CHECK
    std::vector<TileIndex> nearTiles = game->getNeighborsAsVector(tile.x, tile.y);
    Rectangle tileBuf;
    
    for (TileIndex checkingTile : nearTiles)
    {
        tileBuf = Rectangle{ checkingTile.x * game->pixelsPerTile, checkingTile.y * game->pixelsPerTile, game->pixelsPerTile, game->pixelsPerTile };
        if (terrainMod[checkingTile.x][checkingTile.y] == -1.f && CheckCollisionCircleRec(position, size, tileBuf))
        {
            //if tile near is collides with actor
            float R = sqrt(pow(position.x - (tileBuf.x + (tileBuf.width / 2.f)), 2) + pow(position.y - (tileBuf.y + (tileBuf.height / 2.f)), 2));
            if (R != 0.f)
            {
                position.x += (position.x - (tileBuf.x + (tileBuf.width / 2.f))) / R;
                position.y += (position.y - (tileBuf.y + (tileBuf.height / 2.f))) / R;
            }
        }
    }

    TileIndex tileAhead = game->getTileIndex(ahead);
    if (terrainMod[tileAhead.x][tileAhead.y]==-1.f)
    {
        position.x += ahead.x - (tileAhead.x * game->pixelsPerTile + game->pixelsPerTile / 2);
        position.y += ahead.y - (tileAhead.y * game->pixelsPerTile + game->pixelsPerTile / 2);
    }

    //UNIT STEERING AHEAD
    GameActor* nearestObstacle = game->getNearestSpecificActor(ahead, game->getActorsInRadius(ahead, size), type, this);
    if (nearestObstacle != nullptr)
    {
        steering.x += ahead.x - nearestObstacle->getPosition().x;
        steering.y += ahead.y - nearestObstacle->getPosition().y;
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

    std::clamp <float>(steering.x, -1.f, 1.f);
    std::clamp <float>(steering.y, -1.f, 1.f);

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
    DrawCircle(position.x, position.y, size, ORANGE);
}

void LightInsect::Destroy()
{

}