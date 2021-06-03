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
            game->Hit(target, damage, type);
            cooldownRemain = cooldownDuration;
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
    //Move();
}

void Insect::Attack()
{

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