#include "GameClasses.h"

Building::Building(GameData* ptr, ActorType type, Vector2 pos, State state)
    :GameActor(ptr, type, pos, state)
{
    //use GameData to set expansionRange, considering ActorType
    expansionRange = ptr->radius; // TODO: placeholder, remove later
    positionIndex = TileIndex{static_cast<int>(pos.x / ptr->pixelsPerTile), static_cast<int>(pos.y / ptr->pixelsPerTile) };

    expansionIndices = this->game->tilesInsideCircleOrdered(this->positionIndex, expansionRange);

    //create creep or zerolayer on position when spawned
    if(type == ActorType::TUMOR)
        ptr->mapExpansionCreep[positionIndex.x][positionIndex.y] = 2;
}

void Building::Expand()
{
    if (!this->expanded)
    {
        for (TileIndex tile : expansionIndices)
        {
            if (this->side == Side::INSECTS)
            {
                if (this->game->mapExpansionCreep[tile.x][tile.y] != 2)
                {
                    this->game->mapExpansionCreep[tile.x][tile.y] = 2;
                    return;
                }
            }
            //TODO: add MACHINES-side expansion
        }
        this->expanded = true;
    }
}

//для опухолей: проверяет, есть ли в области expansionRange свободное место от слизи (учитывая карту экспансии)
//спавнить за раз 2-4 тайла, каждому свой цикл проверок:
//проходить по спирали радиуса экспансии, проверяя является ли тайл пустым от слизи и имеется ли сосед со слизью

//убываение слизи: раз в n*m циклов проверять все тайлы слизи на наличие рядом источника слизи, если нет - проверять 