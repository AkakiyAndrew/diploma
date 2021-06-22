#include "GameClasses.h"

Turret::Turret(GameData* ptr, ActorType type, Vector2 pos, State state)
    :Militaty(ptr, type, pos, state), 
    Connectable(ptr, type)
{
    maxCharge = ptr->turretsAttributes[type]["maxCharge"];
    chargeRate = ptr->turretsAttributes[type]["chargeRate"];
    energyPerShot = ptr->turretsAttributes[type]["energyPerShot"];

    isMounted = true;
    charge = 0;
    chasisAngle = 0;
    chasisSprite = game->getUnitAnimation(ActorType::TURRET_CHASIS, State::CHANGING_MODE);
    chasisCurrentFrame = chasisSprite.framesAmount - 1;
}

void Turret::Recharge()
{
    if (charge < maxCharge)
    {   
        int spended = game->trySpendResources(std::min(maxCharge - charge, chargeRate), side);
        charge += spended;

        game->spendResources(spended, side);
        
    }
}

void Turret::Attack()
{
    //TODO::make different attacks for all turrets
    if (type == ActorType::HEAVY_TURRET)
        if (game->Hit(target, damage, type))
            target = nullptr;

    if (type == ActorType::LIGHT_TURRET) //attacks units in front of turret (triangle-AoE)
    {
        Vector2 targetPosition = { position.x + attackRange * cosf(angle * PI / 180.f),position.y + attackRange * sinf(angle * PI / 180.f) };//= target->getPosition();
        float s = 1 / sqrtf(3.f); //tan^-1 0.36 instead?
        Vector2 pLeft = {
            targetPosition.x + s * (position.y - targetPosition.y),
            targetPosition.y + s * (targetPosition.x - position.x)
        };
        Vector2 pRight = {
            targetPosition.x + s * (targetPosition.y - position.y),
            targetPosition.y + s * (position.x - targetPosition.x)
        };

        std::vector <GameActor*> buf_targets = game->getActorsInRadius(position, attackRange /*Vector2Distance(position, target->getPosition())*/);
        //TODO: seek for enemy that can be hited
        for (GameActor* actor : buf_targets)
        {
            if (actor->type != ActorType::FLYING_INSECT &&
                actor->side != side &&
                CheckCollisionPointTriangle(actor->getPosition(), position, pLeft, pRight))
            {
                if(game->Hit(actor, damage, type))
                    target = nullptr;
            }
        }
    }

    if (type == ActorType::AIRDEFENSE_TURRET)  //attacks units via AoE-attack (circle around target)
    {
        std::vector <GameActor*> buf_targets = game->getActorsInRadius(target->getPosition(), 16.f);
        for (GameActor* actor : buf_targets)
        {
            if (actor->type == ActorType::FLYING_INSECT)
            {
                if (game->Hit(actor, damage, type))
                    target = nullptr;
            }
        }
    }

    charge -= energyPerShot;
    cooldownRemain = cooldownDuration;
}

void Turret::Update()
{
    switch (state)
    {
    case State::ONLINE: //stationary mode, seeking for targets
        //if no locked target - seek for it
        if (target == nullptr)
            SeekForEnemy();
        else
        {
            //TODO: make check for angle (targeting return bool?)
            //if there is target and distance between it and actor less than attack distance AND eniugh energy
            if (Vector2Distance(position, target->getPosition()) <= attackRange && charge>= energyPerShot && Targeting())
            {
                state = State::ATTACKING;
                sprite = game->getUnitAnimation(type, state);
                currentFrame = 0;
            }
        }
        break;
    case State::UNDER_CONSTRUCTION:
        if (HP == maxHP)
        {
            state = State::ONLINE;
            sprite = game->getUnitAnimation(type, state);
            game->revealTerritory(positionIndex, sightRange, side);
        }
        break;

    case State::ATTACKING: //stationary mode, firing on targets
        if (target == nullptr)
        {
            //if target destroyed
            state = State::ONLINE;
        }
        else
        {
            if (Vector2Distance(position, target->getPosition()) <= attackRange && Targeting())
            {
                if (cooldownRemain == 0)
                {
                    if (type == ActorType::HEAVY_TURRET || type==ActorType::AIRDEFENSE_TURRET)
                    {
                        //for animation
                        attackProgressCounter++;
                        currentFrame = attackProgressCounter / (60 / sprite.framesAmount);
                        if (currentFrame >= sprite.framesAmount)
                        {
                            currentFrame--;
                        }

                        if (attackProgressCounter == 60)
                        {
                            Attack(); //hit target and reset cooldown
                            attackProgressCounter = 0;
                            currentFrame = 0;
                        }
                    }
                    else
                    {
                        Attack();
                        currentFrame = 0;
                    }
                }
            }
        }

        break;
    case State::GOES: //mobile mode, can't fire
        if (waypoints.size() != 0)
        {
            //can move only when charged
            if (charge != 0)
            {
                Move();
                chasisAngle = angle;
                game->revealTerritory(positionIndex, sightRange, side);
                if (positionIndex == waypoints[0])
                    waypoints.erase(waypoints.begin());
                //consume energy 
                if (game->timeCount % 10 == 0) 
                    charge--;

                //chasis animation
                if (game->timeCount % 15 == 0)
                    chasisCurrentFrame++;
                if (chasisCurrentFrame >= chasisSprite.framesAmount)
                    chasisCurrentFrame = 0;
            }
        }
        break;

    case State::CHANGING_MODE:
        modeProgressCounter--;

        //set animation frame
        if (isMounted) //if it was mounted
        {
            chasisCurrentFrame = chasisSprite.framesAmount - modeProgressCounter / (60 / chasisSprite.framesAmount);
        }
        else
        {
            chasisCurrentFrame = modeProgressCounter / (60 / chasisSprite.framesAmount);
        }
        
        if (modeProgressCounter == 0)
        {
            //if mode finally changed - check, which one mode it will be
            if (isMounted) //if it was mounted
            {
                state = State::GOES;
                chasisSprite = game->getUnitAnimation(ActorType::TURRET_CHASIS, state);
                Disconect();
            }
            else
                state = State::ONLINE;
        }
        break;
    }

    Reload();
    
    if (parent != nullptr) //if connected
    {
        //try to recharge, if construction complete and its not in mobile mode
        if (state != State::UNDER_CONSTRUCTION && state != State::GOES)
            Recharge();
    }
    else
        TryConnect(position, ID); //if not

    isInBattleCheck();
}

void Turret::Move()
{
    //!!!
    //TODO: add consuming energy from battery, moving by waypoints

    float** terrainMod = game->mapsPathfinding[type]["mapsTerrainMod"];
    Vector2 previousPosition = position;

    //TODO: move method to new Insect class?
    positionIndex = game->getTileIndex(position);


    //if there is enemy - move toward him
    Vector2 targetLocation = waypoints[0];

    Vector2 buf = { static_cast<float>(targetLocation.x - position.x), static_cast<float>(targetLocation.y - position.y) };
    velocityVector = { buf.x / Vector2Length(buf), buf.y / Vector2Length(buf) };
    
    float speedMod = 1.f;//terrainMod[positionIndex.x][positionIndex.y];

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

void Turret::ChangeMode()
{
    modeProgressCounter = 240; //4 sec.
    state = State::CHANGING_MODE;
    chasisSprite = game->getUnitAnimation(ActorType::TURRET_CHASIS, state);
}

void Turret::DrawChargeBar()
{
    DrawRectangle(position.x - size / 2, position.y + size + 2, size, 2, Color{165, 100, 25, 255});
    if (charge != 0)
        DrawRectangle(position.x - size / 2, position.y + size+2, size * ((float)charge / (float)maxCharge), 2, Color{ 255, 200, 15, 255 });
}

void Turret::Draw()
{
    //TODO: separately draw gun and chassis 
    //chassis
    DrawTexturePro(
        chasisSprite.frames[chasisCurrentFrame], //texture
        Rectangle{ 0,0, (float)chasisSprite.frames[chasisCurrentFrame].width, (float)chasisSprite.frames[chasisCurrentFrame].height }, //source
        Rectangle{ position.x, position.y, (float)size * 2.f, (float)size * 2.f }, //dest
        Vector2{ (float)(size), (float)(size) }, //origin
        (float)chasisAngle, //rotation
        WHITE);
    
    //if culminating attack animation
    if (state == State::ATTACKING && attackProgressCounter>50 && type == ActorType::HEAVY_TURRET)
        DrawLineEx(position, target->getPosition(), 1, RED); //laser beam

    if (type == ActorType::LIGHT_TURRET)
    {
        Vector2 targetPosition = { position.x + attackRange * cosf(angle * PI / 180.f),position.y + attackRange * sinf(angle * PI / 180.f) };//= target->getPosition();
        float s = 1 / sqrtf(3.f); //tan^-1 0.36 instead?
        Vector2 pLeft = {
            targetPosition.x + s * (position.y - targetPosition.y),
            targetPosition.y + s * (targetPosition.x - position.x)
        };
        Vector2 pRight = {
            targetPosition.x + s * (targetPosition.y - position.y),
            targetPosition.y + s * (position.x - targetPosition.x)
        };

        if(state==State::ATTACKING)
            DrawTriangle(position, pLeft, pRight, Fade(Color{240, 50,50,255}, 0.25));
        else
            if(state!=State::UNDER_CONSTRUCTION)
                DrawTriangle(position, pLeft, pRight, Fade(YELLOW, 0.05));
    }

    if (state == State::ATTACKING && attackProgressCounter > 55 && type == ActorType::AIRDEFENSE_TURRET)
    {
        Vector2 targetPos = target->getPosition();
        DrawCircleGradient(targetPos.x, targetPos.y, 16.f, Fade(RED, 0.5), Fade(ORANGE,0.5));
    }

    //gun
    if (state != State::UNDER_CONSTRUCTION)
    {
        DrawTexturePro(
            sprite.frames[currentFrame], //texture
            Rectangle{ 0,0, (float)sprite.frames[currentFrame].width, (float)sprite.frames[currentFrame].height }, //source
            Rectangle{ position.x, position.y, (float)size * 2.f, (float)size * 2.f }, //dest
            Vector2{ (float)(size), (float)(size) }, //origin
            (float)angle, //rotation
            WHITE);
    }

    drawHP();
    DrawChargeBar();
}

void Turret::Destroy() 
{

}