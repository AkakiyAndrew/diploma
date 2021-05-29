#include "GameClasses.h"

Building::Building(GameData* ptr, ActorType type, Vector2 pos, State state)
    :GameActor(ptr, type, pos, state)
{
    expansionRange = ptr->buildingsAttributes[type]["expansionRange"];
    expansionTime = ptr->buildingsAttributes[type]["expansionTime"];

    //positionIndex = TileIndex{static_cast<int>(pos.x / ptr->pixelsPerTile), static_cast<int>(pos.y / ptr->pixelsPerTile) };

    expansionIndices = this->game->tilesInsideCircleOrdered(this->positionIndex, expansionRange);
    
    //only if it created operational
    if(state==State::ONLINE)
        markAreaExpand();
}

void Building::Expand()
{
    if (this->game->timeCount % expansionTime == 0)
    {
        if (!this->expanded)
        {
            //TODO: further optimization - remove expanded tile from vector, on Destroy call recalculate it again to set mapExpansionCreep
            for (TileIndex tile : expansionIndices)
            {
                if (this->side == Side::INSECTS)
                {
                    if (game->mapExpansionCreep[tile.x][tile.y] == ExpandState::AVAILABLE)
                    {
                        if (game->numOfExpansionTileAdjoin(tile.x, tile.y, this->side) > 0)
                        {
                            game->mapExpansionCreep[tile.x][tile.y] = ExpandState::EXPANDED;
                            game->creepTilesCount++;
                            return;
                        }
                    }
                }

                if (this->side == Side::MACHINES)
                {
                    if (game->mapExpansionEnergised[tile.x][tile.y] == ExpandState::AVAILABLE)
                    {
                        if (game->numOfExpansionTileAdjoin(tile.x, tile.y, this->side) > 0)
                        {
                            game->mapExpansionEnergised[tile.x][tile.y] = ExpandState::EXPANDED;
                            game->energisedTilesCount++;
                            return;
                        }
                    }
                }
            }
            this->expanded = true;
        }
    }
}

void Building::markAreaExpand()
{
    this->expanded = false;

    //create creep or energised on position to initiate expansion
    if (side == Side::INSECTS)
    {
        if(game->mapExpansionCreep[positionIndex.x][positionIndex.y]==ExpandState::AVAILABLE)
            game->creepTilesCount++;
        game->mapExpansionCreep[positionIndex.x][positionIndex.y] = ExpandState::EXPANDED;
    }
        
    if (side == Side::MACHINES)
    {
        if (game->mapExpansionEnergised[positionIndex.x][positionIndex.y] == ExpandState::AVAILABLE)
            game->energisedTilesCount++;
        game->mapExpansionEnergised[positionIndex.x][positionIndex.y] = ExpandState::EXPANDED;
    }
        

    for (TileIndex tile : expansionIndices)
    {
        if (this->side == Side::INSECTS)
        {
            if (game->mapExpansionCreep[tile.x][tile.y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
            {
                game->mapExpansionCreep[tile.x][tile.y] = ExpandState::EXPANDED;
            }
        }

        if (this->side == Side::MACHINES)
        {
            if (game->mapExpansionEnergised[tile.x][tile.y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
            {
                game->mapExpansionEnergised[tile.x][tile.y] = ExpandState::EXPANDED;
            }
        }
    }
}

void Building::markAreaFade()
{
    //setting expansion tiles around building to fade
    for (TileIndex tile : expansionIndices)
    {
        if (this->side == Side::INSECTS)
        {
            if (game->mapExpansionCreep[tile.x][tile.y] == ExpandState::EXPANDED)
            {
                game->mapExpansionCreep[tile.x][tile.y] = ExpandState::EXPANDED_WITHOUT_SOURCE;
            }
        }

        if (this->side == Side::MACHINES)
        {
            if (game->mapExpansionEnergised[tile.x][tile.y] == ExpandState::EXPANDED)
            {
                game->mapExpansionEnergised[tile.x][tile.y] = ExpandState::EXPANDED_WITHOUT_SOURCE;
            }
        }
    }

    if (this->side == Side::INSECTS)
        game->mapExpansionCreep[positionIndex.x][positionIndex.y] = ExpandState::EXPANDED_WITHOUT_SOURCE;

    if (this->side == Side::MACHINES)
        game->mapExpansionEnergised[positionIndex.x][positionIndex.y] = ExpandState::EXPANDED_WITHOUT_SOURCE;

    //TODO: possible optimizaton - recalculate expansion in specific range (with getActorsInRadius method or kinda like that)
    //recalculate expansion tiles to overlap faded tiles by existing buildings
    this->game->recalculateExpansion(this->side);
}

Building::~Building()
{
    markAreaFade();
}