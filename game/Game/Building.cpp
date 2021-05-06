#include "GameClasses.h"

Building::Building(GameData* ptr, ActorType type, Vector2 pos, State state)
    :GameActor(ptr, type, pos, state)
{
    //use GameData to set expansionRange, considering ActorType
    expansionRange = ptr->buildingsAttributes[type]["expansionRange"]; // TODO: placeholder, remove later

    positionIndex = TileIndex{static_cast<int>(pos.x / ptr->pixelsPerTile), static_cast<int>(pos.y / ptr->pixelsPerTile) };

    expansionIndices = this->game->tilesInsideCircleOrdered(this->positionIndex, expansionRange);

    markExpandArea();

    //create creep or zerolayer on position when spawned
    if(type == ActorType::TUMOR || type == ActorType::HIVE)
        ptr->mapExpansionCreep[positionIndex.x][positionIndex.y] = ExpandState::EXPANDED;
}

void Building::Expand()
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
                    if (game->numOfExpansionTileAdjoin(tile.x, tile.y, this->side)>0)
                    {
                        game->mapExpansionCreep[tile.x][tile.y] = ExpandState::EXPANDED;
                        return;
                    }
                }
            }
            //TODO: add MACHINES-side expansion
        }
        this->expanded = true;
    }
}

void Building::markExpandArea()
{
    this->expanded = false;
    for (TileIndex tile : expansionIndices)
    {
        if (this->side == Side::INSECTS)
        {
            if (game->mapExpansionCreep[tile.x][tile.y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
            {
                game->mapExpansionCreep[tile.x][tile.y] = ExpandState::EXPANDED;
            }
        }
        //TODO: add MACHINES-side expansion
    }
}

Building::~Building()
{
    for (TileIndex tile : expansionIndices)
    {
        if (this->side == Side::INSECTS)
        {
            if (game->mapExpansionCreep[tile.x][tile.y] == ExpandState::EXPANDED)
            {
                game->mapExpansionCreep[tile.x][tile.y] = ExpandState::EXPANDED_WITHOUT_SOURCE;
            }
        }
        //TODO: add MACHINES-side expansion
    }
    game->mapExpansionCreep[positionIndex.x][positionIndex.y] = ExpandState::EXPANDED_WITHOUT_SOURCE;
    //TODO: possible optimizaton - recalculate expansion in specific range (with getActorsInRadius method or kinda like that)
    this->game->recalculateExpansion(this->side);
}

//для опухолей: проверяет, есть ли в области expansionRange свободное место от слизи (учитывая карту экспансии)
//спавнить за раз 2-4 тайла, каждому свой цикл проверок:
//проходить по спирали радиуса экспансии, проверяя является ли тайл пустым от слизи и имеется ли сосед со слизью

//убываение слизи: раз в n*m циклов проверять все тайлы слизи на наличие рядом источника слизи, если нет - проверять 