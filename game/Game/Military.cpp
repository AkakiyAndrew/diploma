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

void Militaty::Move()
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
    Vector2 ahead = { position.x + velocityVector.x * 4.f, position.y + velocityVector.y  * 4.f };
    Vector2 steering = { 0.f, 0.f };

    //UNIT STEERING AHEAD
    GameActor* nearestObstacle = game->getNearestSpecificActor(ahead, game->getActorsInRadius(ahead, size), this, type);
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

    velocityVector.x+= steering.x;
    velocityVector.y+= steering.y;

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
    if (position.x > game->getMaxWidth()*game->pixelsPerTile - size)
        position.x = game->getMaxWidth() * game->pixelsPerTile - size;
    if (position.y > game->getMaxHeight() * game->pixelsPerTile - size)
        position.y = game->getMaxHeight() * game->pixelsPerTile - size;


    angle = Vector2Angle(previousPosition, position);
}

Militaty::~Militaty()
{

}