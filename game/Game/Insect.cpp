#include "GameClasses.h"

Insect::Insect(GameData* ptr, ActorType type, Vector2 pos, State state)
    :Militaty(ptr, type, pos, state)
{
    
}

void Insect::Update()
{
    switch (state)
    {
    case State::ATTACKING:
        if (target == nullptr)
        {
            //if target destroyed
            state = State::GOES;
        }
        else
        {
            //if not - hit target and reset cooldown
            Targeting();
            if (Vector2Distance(position, target->getPosition()) <= attackRange)
            {
                if (cooldownRemain == 0)
                    Attack();
            }
            else
                state = State::GOES;
        }

        break;
    case State::GOES:
        Move();
        game->revealTerritory(positionIndex, sightRange, side);

        //if no locked target - seek for it
        if (target == nullptr)
            SeekForEnemy();
        else
        {
            //if there is target and distance between it and actor less than attack distance
            if (Vector2Distance(position, target->getPosition())<=attackRange)
            {
                state = State::ATTACKING;
            }
        }

        break;
    }
    
    Reload();

    if (game->timeCount % 15 == 0)
        currentFrame++;
    if (currentFrame >= sprite.framesAmount)
        currentFrame = 0;
}

void Insect::Move()
{
    float** terrainMod = game->mapsPathfinding[type]["mapsTerrainMod"];
    Vector2 previousPosition = position;

    //TODO: move method to new Insect class?
    positionIndex = game->getTileIndex(position);

    if (target == nullptr)
    {
        //if no enemy in sight - moving on vector field
        velocityVector = game->vectorFields[type][positionIndex.x][positionIndex.y];
    }
    else
    {
        //if there is enemy - move toward him
        Vector2 targetLocation = target->getPosition();

        Vector2 buf = { static_cast<float>(targetLocation.x - position.x), static_cast<float>(targetLocation.y - position.y) };
        //if (buf.x != 0 || buf.y != 0)
        //velocityVector = { buf.x / fabsf(buf.x < buf.y ? buf.y : buf.x), buf.y / fabsf(buf.x < buf.y ? buf.y : buf.x) };
        velocityVector = { buf.x / Vector2Length(buf), buf.y / Vector2Length(buf) };
    }

    float speedMod = terrainMod[positionIndex.x][positionIndex.y];

    //creep speed mod
    if (game->isTileExpanded(positionIndex, side) && type != ActorType::FLYING_INSECT)
    {
        speedMod *= 1.25f;
    }

    velocityVector.x *= speed;
    velocityVector.y *= speed;

    //collision avoidance
    Vector2 ahead = { position.x + velocityVector.x * 4.f, position.y + velocityVector.y * 4.f };
    Vector2 steering = { 0.f, 0.f };

    //UNIT STEERING AHEAD
    GameActor* nearestObstacle = game->getNearestSpecificActor(ahead, game->getActorsInRadius(ahead, size), this, type);
    if (nearestObstacle != nullptr)
    {
        float R = sqrt(pow(ahead.x - nearestObstacle->getPosition().x, 2) + pow(ahead.y - nearestObstacle->getPosition().y, 2));
        if (R != 0.f)
        {
            steering.x += (ahead.x - nearestObstacle->getPosition().x) / R;
            steering.y += (ahead.y - nearestObstacle->getPosition().y) / R;
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
        if (checkingTile.x != -1)
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
    }

    //std::clamp <float>(steering.x, -1.f, 1.f);
    //std::clamp <float>(steering.y, -1.f, 1.f);

    velocityVector.x += steering.x;
    velocityVector.y += steering.y;

    velocityVector.x = std::clamp <float>(velocityVector.x, -speed, speed);
    velocityVector.y = std::clamp <float>(velocityVector.y, -speed, speed);

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
        if (checkingTile.x != -1)
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
    }

    //MAP BOUNDS CHECK
    if (position.x < 0 + size)
        position.x = 0 + size;
    if (position.y < 0 + size)
        position.y = 0 + size;
    if (position.x > game->getMaxWidth() * game->pixelsPerTile - size)
        position.x = game->getMaxWidth() * game->pixelsPerTile - size;
    if (position.y > game->getMaxHeight() * game->pixelsPerTile - size)
        position.y = game->getMaxHeight() * game->pixelsPerTile - size;


    angle = Vector2Angle(previousPosition, position);
}

void Insect::Attack()
{
    game->Hit(target, damage, type);
    cooldownRemain = cooldownDuration;
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

    DrawCircleLines(position.x, position.y, size, ORANGE);
    DrawCircle(position.x + velocityVector.x, position.y + velocityVector.y, 1, BLUE);
}

void Insect::Destroy()
{

}