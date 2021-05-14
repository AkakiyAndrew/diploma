#include "GameClasses.h"

#define TILE_DRAWING

GameData::GameData()
{
    screenSize = Vector2{ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };

    camera.target = { screenSize.x / 2, screenSize.y / 2 };
    camera.offset = { screenSize.x / 2, screenSize.y / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    palette = new Color[]{ DARKBLUE, BLUE, YELLOW, GREEN, DARKGREEN, GRAY, BLACK };

    //tileset loading
    tileset = new Image[8];

    const char* filenames[] = { 
        "textures\\tileset\\LAKE.bmp",
        "textures\\tileset\\SWAMP.bmp",
        "textures\\tileset\\SAND.bmp",
        "textures\\tileset\\PLAIN.bmp",
        "textures\\tileset\\TREE.bmp",
        "textures\\tileset\\STONE.bmp",
        "textures\\tileset\\MOUNTAIN.bmp",
        "textures\\tileset\\CREEP.png"
    };

    for (int i = 0; i < 8; i++)
    {
        tileset[i] = LoadImage(filenames[i]);
        ImageFormat(&tileset[i], UNCOMPRESSED_R5G6B5);
    }

    for (int i = 0; i < 8; i++)
    {
        tilesetTex[i] = LoadTextureFromImage(tileset[i]);
    }

    for (int i = 0; i < 7; i++)
        UnloadImage(tileset[i]);

    Image creep = LoadImage("textures\\source\\creep_1.png");
    creepTexture = LoadTextureFromImage(creep);
    UnloadImage(creep);
    //TODO: texture files loading, units properties

    //GENERIC ACTORS ATTRIBUTES
    genericAttributes[ActorType::TUMOR] = std::map<std::string, int>{
        {"maxHP", 100},
        {"size", 16},
        {"cost", 10},
        {"sightRange", 10}
    };

    genericAttributes[ActorType::LIGHT_INSECT] = std::map<std::string, int>{
        {"maxHP", 50},
        {"size", 5},
        {"cost", 5},
        {"sightRange", 8}
    };

    //BUILDING ACTORS ATTRIBUTES
    buildingsAttributes[ActorType::TUMOR] = std::map<std::string, int>{
        {"expansionRange", 16},
    };

    //MILITARY ACTORS ATTRIBUTES
    militaryAttributes[ActorType::LIGHT_INSECT] = std::map<std::string, int>{
        {"seekRange", 8},
        {"attackRange", 2},
        {"speed", 1},
        {"damage", 7},
        {"reloadCount", 20},
        {"rotationSpeed", 4},
    };
}

bool GameData::isMapLoaded()
{
    return this->mapTerrain != nullptr;
}

void GameData::clearMap()
{
    std::vector<unsigned int> IDs;

    for (GameActor *actor : this->unitsList)
    {
        IDs.push_back(actor->ID);
    }
    
    for (unsigned int ID : IDs)
    {
        removeActor(ID);
    }

}

TerrainType GameData::getTerrainType(int x, int y)
{
    return mapTerrain[y * mapWidth + x];
}

void GameData::setTerrain(Terrain terr)
{
    //TODO: make check for existence of path from Base to Hive (if not - create it)
    //TODO: MAKE NEW FUNCTION TO REDRAW PREVIEW TEXTURE, CALL IT ONLY FEW TIMES IN UPDATE
    if (isMapLoaded())
    {
        UnloadTexture(terrainTexture);

        //expansion
        for (int x = 0; x < mapWidth; x++)
                delete[] mapExpansionCreep[x];
        delete[] mapExpansionCreep;

        //pathfinding
        ActorType types[] = { ActorType::LIGHT_INSECT, ActorType::HEAVY_INSECT, ActorType::FLYING_INSECT };
        std::vector<std::string> matrices = { "mapsHeat", "mapsTerrainMod", "mapsDamage" };

        for (ActorType type : types)
        {
            //support matrices
            for (std::string matrixName : matrices)
            {
                for (int x = 0; x < mapWidth; x++)
                    delete[] mapsPathfinding[type][matrixName][x];
                delete[] mapsPathfinding[type][matrixName];
            }

            //vector matrix
            for (int x = 0; x < mapWidth; x++)
                delete[] vectorFields[type][x];
            delete[] vectorFields[type];
        }

        //neighbors matrix
        for (int x = 0; x < mapWidth; x++)
            delete[] neighborsIndices[x];
        delete[] neighborsIndices;
    }

    this->mapHeight = terr.height;
    this->mapWidth = terr.width;
    this->mapTerrain = terr.map;

    mapSize.x = mapWidth * pixelsPerTile;
    mapSize.y = mapHeight * pixelsPerTile;

    //expansion maps memory allocation
    mapExpansionCreep = new unsigned char* [mapWidth];
    for (int x = 0; x < mapWidth; x++)
        mapExpansionCreep[x] = new unsigned char[mapHeight];

    //expansion pre-calculation
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            switch (getTerrainType(x, y))
            {
            //if cant expand in this tile
            case TerrainType::LAKE:
            case TerrainType::SWAMP:
            case TerrainType::MOUNTAIN:
                mapExpansionCreep[x][y] = ExpandState::UNAVAILABLE;
                break;
            //if expansion can be provide
            case TerrainType::SAND:
            case TerrainType::PLAIN:
            case TerrainType::TREE:
            case TerrainType::STONE:
            case TerrainType::ASH:
                mapExpansionCreep[x][y] = ExpandState::AVAILABLE;
                break;
            //just in case
            default:
                mapExpansionCreep[x][y] = ExpandState::UNAVAILABLE;
                break;
            }
        }
    }

    //tiles neighbors allocation 
    neighborsIndices = new NeighborsIndex * [mapWidth];
    for (int x = 0; x < mapWidth; x++)
        neighborsIndices[x] = new NeighborsIndex[mapHeight];
    //neighbors pre - calculation
    for (int x = 0; x < mapWidth; x++)
        for (int y = 0; y < mapHeight; y++)
            neighborsIndices[x][y] = getNeighbors(x, y);

    //vector pathfinding matrix allocation
    ActorType types[] = { ActorType::LIGHT_INSECT, ActorType::HEAVY_INSECT, ActorType::FLYING_INSECT };
    std::vector<std::string> matrices = { "mapsHeat", "mapsTerrainMod", "mapsDamage" };

    for (ActorType type : types)
    {
        //memory allocation, support matrices
        for (std::string matrixName : matrices)
        {
            mapsPathfinding[type][matrixName] = new float* [mapWidth];
            for (int x = 0; x < mapWidth; x++)
                mapsPathfinding[type][matrixName][x] = new float[mapHeight];
        }
        //vector matrices
        vectorFields[type] = new Vector2* [mapWidth];
        for (int x = 0; x < mapWidth; x++)
            vectorFields[type][x] = new Vector2[mapHeight];

        //terrain speed modifier, individual for each Insect's unit type
        float** mapTerrainMod = mapsPathfinding[type]["mapsTerrainMod"];
        for (int x = 0; x < mapWidth; x++)
        {
            for (int y = 0; y < mapHeight; y++)
            {
                if (type == ActorType::LIGHT_INSECT)
                {
                    switch (getTerrainType(x, y))
                    {
                        //cant walk in this tile
                    case TerrainType::LAKE:
                    case TerrainType::MOUNTAIN:
                        mapTerrainMod[x][y] = -1.f;
                        break;
                        //can, but much slower
                    case TerrainType::SWAMP:
                        mapTerrainMod[x][y] = 0.5f;
                        break;
                    case TerrainType::SAND:
                        mapTerrainMod[x][y] = 0.8f;
                        break;
                    case TerrainType::PLAIN:
                    case TerrainType::TREE:
                    case TerrainType::STONE:
                    case TerrainType::ASH:
                        mapTerrainMod[x][y] = 1.f;
                        break;
                        //just in case
                    default:
                        mapTerrainMod[x][y] = -1.f;
                        break;
                    }
                }

                if (type == ActorType::HEAVY_INSECT)
                {
                    switch (getTerrainType(x, y))
                    {
                        //cant walk in this tile
                    case TerrainType::MOUNTAIN:
                        mapTerrainMod[x][y] = -1.f;
                        break;
                        //can, but much slower
                    case TerrainType::LAKE:
                    case TerrainType::SWAMP:
                        mapTerrainMod[x][y] = 0.5f;
                        break;
                    case TerrainType::SAND:
                    case TerrainType::TREE:
                        mapTerrainMod[x][y] = 0.7f;
                        break;
                    case TerrainType::PLAIN:
                    case TerrainType::STONE:
                    case TerrainType::ASH:
                        mapTerrainMod[x][y] = 1.f;
                        break;
                        //just in case
                    default:
                        mapTerrainMod[x][y] = -1.f;
                        break;
                    }
                }

                if (type == ActorType::FLYING_INSECT)
                {
                    switch (getTerrainType(x, y))
                    {
                        //can fly through any tile:
                    case TerrainType::LAKE:
                    case TerrainType::MOUNTAIN:
                    case TerrainType::SWAMP:
                    case TerrainType::SAND:
                    case TerrainType::PLAIN:
                    case TerrainType::TREE:
                    case TerrainType::STONE:
                    case TerrainType::ASH:
                        mapTerrainMod[x][y] = 1.f;
                        break;
                        //just in case
                    default:
                        mapTerrainMod[x][y] = -1.f;
                        break;
                    }
                }
            }
        }
    }

#ifndef TILE_DRAWING
    unsigned short* colorPixels = new unsigned short[mapSize.x * mapSize.y];

    Image buf = {
        colorPixels,
        mapSize.x,
        mapSize.y,
        1,
        UNCOMPRESSED_R5G6B5
    };

    int index;

    //TODO: check blending principle for generating texture
#pragma omp parallel for private(index)
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            index = mapWidth * i + j;
            ImageDraw(
                &buf,
                tileset[static_cast<int>(mapTerrain[index])],
                Rectangle{ 0,0, pixelsPerTile, pixelsPerTile },
                Rectangle{ j * pixelsPerTile, i * pixelsPerTile, pixelsPerTile, pixelsPerTile },
                WHITE);
        }
    }
#else
    Color* colorPixels = new Color[mapHeight * mapWidth];

    Image buf = {
        colorPixels,
        mapWidth,
        mapHeight,
        1,
        UNCOMPRESSED_R8G8B8A8
    };

#pragma omp parallel for
    for (int i = 0; i < mapHeight * mapWidth; i++)
    {
        colorPixels[i] = palette[static_cast<int>(mapTerrain[i])];
    }

#endif

    terrainTexture = LoadTextureFromImage(buf);
    UnloadImage(buf);

    calculateVectorPathfinding(
        TileIndex{ 0,0 },
        ActorType::LIGHT_INSECT
    );
}

std::vector<TileIndex> GameData::tilesInsideCircle(Vector2 center, unsigned int radius)
{
    std::vector<TileIndex> result;
    TileIndex upLeftCorner = { std::clamp<int>(center.x - radius, 0, center.x), std::clamp<int>(center.y - radius, 0, center.y) };
    TileIndex downRightCorner = { std::clamp<int>(center.x + radius, center.x, this->mapWidth-1), std::clamp<int>(center.y + radius, center.y, this->mapHeight-1) };
    //reducing up limit `cause of max index of maps (not the actual indices)
    
    for (int y = upLeftCorner.y; y <= downRightCorner.y; y++)
    {
        for (int x = upLeftCorner.x; x <= downRightCorner.x; x++)
        {
            if (CheckCollisionPointCircle(Vector2{static_cast<float>(x), static_cast<float>(y) }, center, radius))
            result.push_back(TileIndex{x,y});
            //TODO: optimize this crap (and make parallel computaton?)
        }
    }

    return result;
}

std::vector<TileIndex> GameData::tilesInsideCircleOrdered(TileIndex center, int radius)
{
    std::vector<TileIndex> result;

    for (int r = 1; r <= radius; r++)
    {
        
        for (int x = center.x + 1;x<center.x + r;x++)
        {
            for (int y = center.y; y > center.y - r; y--) //REVERSE int y = center.y - r; y < center.y + r; y++
            {
                if (CheckCollisionPointCircle(Vector2{ static_cast<float>(x), static_cast<float>(y) }, center, r))
                {
                    //adding this and opposite tiles
                    result.push_back(TileIndex{ x,y }); // up-right
                    result.push_back(TileIndex{ center.x + (center.x - x), y }); //up-left
                    result.push_back(TileIndex{ x, center.y + (center.y - y) }); //down-right
                    result.push_back(TileIndex{ center.x + (center.x - x), center.y + (center.y - y) }); //down-left
                }
            }
        }
        result.push_back(TileIndex{ center.x, center.y - r }); //up
        result.push_back(TileIndex{ center.x, center.y + r }); //down
        result.push_back(TileIndex{ center.x + r, center.y }); //right
        result.push_back(TileIndex{ center.x - r, center.y });//left
    }

    std::vector<TileIndex>::iterator iter;
    for (iter = result.begin(); iter != result.end(); ) 
    {
        if ((*iter).x < 0 ||
            (*iter).x>this->mapWidth - 1 ||
            (*iter).y < 0 ||
            (*iter).y>this->mapHeight - 1)
            iter = result.erase(iter);
        else
            iter++;
    }

    return result;
}

int GameData::numOfExpansionTileAdjoin(int x, int y, Side side)
{
    int result = 0;
    if (side == Side::INSECTS)
    {
        if (x - 1 >= 0) //left
            if (mapExpansionCreep[x - 1][y] == ExpandState::EXPANDED || mapExpansionCreep[x - 1][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (y - 1 >= 0) //up
            if (mapExpansionCreep[x][y - 1] == ExpandState::EXPANDED || mapExpansionCreep[x][y - 1] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (x + 1 < this->mapWidth) //right
            if (mapExpansionCreep[x + 1][y] == ExpandState::EXPANDED || mapExpansionCreep[x + 1][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (y + 1 < this->mapHeight) //down
            if (mapExpansionCreep[x][y + 1] == ExpandState::EXPANDED || mapExpansionCreep[x][y + 1] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
    }
    return result;
    //TODO: make checking for MACHINES side
}

int GameData::numOfExpansionTileAdjoinFading(int x, int y, Side side)
{
    int result = 0;
    if (side == Side::INSECTS)
    {
        if (x - 1 >= 0) //left
            if (mapExpansionCreep[x - 1][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (y - 1 >= 0) //up
            if (mapExpansionCreep[x][y - 1] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (x + 1 < this->mapWidth) //right
            if (mapExpansionCreep[x + 1][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (y + 1 < this->mapHeight) //down
            if (mapExpansionCreep[x][y + 1] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
    }
    return result;
    //TODO: make checking for MACHINES side
}

bool GameData::isTileExpanded(TileIndex tile, Side side)
{
    if (side == Side::INSECTS)
    {
        if (mapExpansionCreep[tile.x][tile.y] == ExpandState::EXPANDED || mapExpansionCreep[tile.x][tile.y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
            return true;
    }
    else
    {
        //TODO: do same for MACHINES
    }

    return false;
}

void GameData::recalculateExpansion(Side side)
{
    for (Building* actor : this->expansionUnitsList)
    {
        if (actor->side == side)
            actor->markExpandArea();
    }
}

NeighborsIndex GameData::getNeighbors(int x, int y)
{
    NeighborsIndex result;

    if (x - 1 >= 0)             //left
        result.left = { x - 1, y };
    if (y - 1 >= 0)             //up
        result.up = { x, y - 1 };
    if (x + 1 < this->mapWidth) //right
        result.right = { x + 1, y };
    if (y + 1 < this->mapHeight) //down
        result.down = { x, y + 1 };

    //corners
    if (y - 1 >= 0 && x - 1 >= 0) //up-left
        result.upLeft = { x - 1, y - 1 };
    if (y - 1 >= 0 && x + 1 < this->mapWidth) //up-right
        result.upRight = { x + 1, y - 1 };
    if (y + 1 < this->mapHeight && x - 1 >= 0) //down-left
        result.downLeft = { x - 1, y + 1 };
    if (y + 1 < this->mapHeight && x + 1 < this->mapWidth) //down-right
        result.downRight = { x + 1, y + 1 };


    return result;
}

TileIndex GameData::getTileIndex(Vector2 position)
{
    return TileIndex{
        static_cast<int>(position.x / pixelsPerTile),
        static_cast<int>(position.y / pixelsPerTile)
    };
}

void GameData::calculateVectorPathfinding(TileIndex target, ActorType actorType)
{
    float** mapHeat = mapsPathfinding[actorType]["mapsHeat"];
    float** mapTerrainMod = mapsPathfinding[actorType]["mapsTerrainMod"];
    Vector2** mapVector = vectorFields[actorType];

    //heat and vector map nullification
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            mapHeat[x][y] = 0.f;
            mapVector[x][y] = { 0.f, 0.f };
        }
    }

    //HEATMAP CALCULATION
    mapHeat[target.x][target.y] = 1.f; //initial target point

    //std::vector<TileIndex> toCheck = { target };
    int i_checking = 0, i_max = mapHeight * mapHeight, i_last_added = 0; //to replace vector
    TileIndex* toCheck = new TileIndex[i_max];
    toCheck[0] = target;

    NeighborsIndex checking;
    float previous = 1.f;
    
    //TODO: возможная оптимизация с заменой вектора на фиксированный одномерный массив, используя индексы для заполнения и обращения
    //внутри итерации менять iLastAdded для обоз
    for(int i = 0; i<i_max; i++)
    {
        if (i <= i_last_added) //for "repair" crush on Degug configuration
        {
            previous = mapHeat[toCheck[i].x][toCheck[i].y]; //previous - to calculate neighbors tiles
            checking = neighborsIndices[toCheck[i].x][toCheck[i].y];
        }

        //left
        if (checking.left.x != -1 && checking.left.y != -1) //if index valid
        {
            //if its not obstacle and value not set:
            if (mapTerrainMod[checking.left.x][checking.left.y] != -1 && mapHeat[checking.left.x][checking.left.y] == 0.f)
            {
                i_last_added++;
                //TODO: ADD CONSIDERING MAP DAMAGE
                //set value, according to terrain speed modification and damage map
                mapHeat[checking.left.x][checking.left.y] = previous + (1 / mapTerrainMod[checking.left.x][checking.left.y]);
                //add this tile for further checking
                toCheck[i_last_added] = (checking.left);
            }
        }
        //up
        if (checking.up.x != -1 && checking.up.y != -1) //if index valid
        {
            //if its not obstacle and value not set:
            if (mapTerrainMod[checking.up.x][checking.up.y] != -1 && mapHeat[checking.up.x][checking.up.y] == 0.f)
            {
                i_last_added++;
                //TODO: ADD CONSIDERING MAP DAMAGE
                //set value, according to terrain speed modification and damage map
                mapHeat[checking.up.x][checking.up.y] = previous + (1 / mapTerrainMod[checking.up.x][checking.up.y]);
                //add this tile for further checking
                toCheck[i_last_added] =checking.up;
            }
        }
        //right
        if (checking.right.x != -1 && checking.right.y != -1) //if index valid
        {
            //if its not obstacle and value not set:
            if (mapTerrainMod[checking.right.x][checking.right.y] != -1 && mapHeat[checking.right.x][checking.right.y] == 0.f)
            {
                i_last_added++;
                //TODO: ADD CONSIDERING MAP DAMAGE
                //set value, according to terrain speed modification and damage map
                mapHeat[checking.right.x][checking.right.y] = previous + (1 / mapTerrainMod[checking.right.x][checking.right.y]);
                //add this tile for further checking
                toCheck[i_last_added] =checking.right;
            }
        }
        //down
        if (checking.down.x != -1 && checking.down.y != -1) //if index valid
        {
            //if its not obstacle and value not set:
            if (mapTerrainMod[checking.down.x][checking.down.y] != -1 && mapHeat[checking.down.x][checking.down.y] == 0.f)
            {
                i_last_added++;
                //TODO: ADD CONSIDERING MAP DAMAGE
                //set value, according to terrain speed modification and damage map
                mapHeat[checking.down.x][checking.down.y] = previous + (1 / mapTerrainMod[checking.down.x][checking.down.y]);
                //add this tile for further checking
                toCheck[i_last_added] =checking.down;
            }
        }

        //up-left
        if (checking.upLeft.x!=-1) //if index valid
        {
            //if its not obstacle and value not set:
            if (mapTerrainMod[checking.upLeft.x][checking.upLeft.y] != -1 && mapHeat[checking.upLeft.x][checking.upLeft.y] == 0.f)
            {
                i_last_added++;
                //TODO: ADD CONSIDERING MAP DAMAGE
                //set value, according to terrain speed modification and damage map
                mapHeat[checking.upLeft.x][checking.upLeft.y] = previous + (1 / mapTerrainMod[checking.upLeft.x][checking.upLeft.y]);
                //add this tile for further checking
                toCheck[i_last_added] =checking.upLeft;
            }
        }
        //up-right
        if (checking.upRight.x != -1) //if index valid
        {
            //if its not obstacle and value not set:
            if (mapTerrainMod[checking.upRight.x][checking.upRight.y] != -1 && mapHeat[checking.upRight.x][checking.upRight.y] == 0.f)
            {
                i_last_added++;
                //TODO: ADD CONSIDERING MAP DAMAGE
                //set value, according to terrain speed modification and damage map
                mapHeat[checking.upRight.x][checking.upRight.y] = previous + (1 / mapTerrainMod[checking.upRight.x][checking.upRight.y]);
                //add this tile for further checking
                toCheck[i_last_added] =checking.upRight;
            }
        }
        //down-left
        if (checking.downLeft.x != -1) //if index valid
        {
            //if its not obstacle and value not set:
            if (mapTerrainMod[checking.downLeft.x][checking.downLeft.y] != -1 && mapHeat[checking.downLeft.x][checking.downLeft.y] == 0.f)
            {
                i_last_added++;
                //TODO: ADD CONSIDERING MAP DAMAGE
                //set value, according to terrain speed modification and damage map
                mapHeat[checking.downLeft.x][checking.downLeft.y] = previous + (1 / mapTerrainMod[checking.downLeft.x][checking.downLeft.y]);
                //add this tile for further checking
                toCheck[i_last_added] =checking.downLeft;
            }
        }
        //down-right
        if (checking.downRight.x != -1) //if index valid
        {
            //if its not obstacle and value not set:
            if (mapTerrainMod[checking.downRight.x][checking.downRight.y] != -1 && mapHeat[checking.downRight.x][checking.downRight.y] == 0.f)
            {
                i_last_added++;
                //TODO: ADD CONSIDERING MAP DAMAGE
                //set value, according to terrain speed modification and damage map
                mapHeat[checking.downRight.x][checking.downRight.y] = previous + (1 / mapTerrainMod[checking.downRight.x][checking.downRight.y]);
                //add this tile for further checking
                toCheck[i_last_added] =checking.downRight;
            }
        }
    }

    delete[] toCheck;

    //VECTOR MAP CALCULATION

    //float x_buf;
    //float y_buf;
    float minimumHeat;
    Vector2 buf;
    NeighborsIndex neighbors;

#pragma omp parallel for private(neighbors, minimumHeat)
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            neighbors = neighborsIndices[x][y];
            //x_buf = 0.f;
            //y_buf = 0.f;


            ////left
            //if (neighbors.left.x != -1) //check for map borders
            //{
            //    if (mapTerrainMod[x - 1][y] != -1.f) //check for obstruction 
            //        x_buf += mapHeat[x-1][y]; //left
            //    else
            //        x_buf += mapHeat[x][y]; //this tile
            //}
            //else
            //    x_buf += mapHeat[x][y];
            ////right
            //if (neighbors.right.x != -1) //check for map borders
            //{
            //    if (mapTerrainMod[x + 1][y] != -1.f) //check for obstruction 
            //        x_buf -= mapHeat[x + 1][y]; //right
            //    else
            //        x_buf -= mapHeat[x][y]; //this tile
            //}
            //else
            //    x_buf -= mapHeat[x][y];
            ////up
            //if (neighbors.up.x != -1) //check for map borders
            //{
            //    if (mapTerrainMod[x][y-1] != -1.f) //check for obstruction 
            //        y_buf += mapHeat[x][y-1]; //up
            //    else
            //        y_buf += mapHeat[x][y]; //this tile
            //}
            //else
            //    y_buf += mapHeat[x][y];
            ////down
            //if (neighbors.down.x != -1) //check for map borders
            //{
            //    if (mapTerrainMod[x][y+1] != -1.f) //check for obstruction 
            //        y_buf -= mapHeat[x][y+1]; //down
            //    else
            //        y_buf -= mapHeat[x][y]; //this tile
            //}
            //else
            //    y_buf -= mapHeat[x][y];

            //mapVector[x][y] = { x_buf, y_buf };

            //TODO: remake for using gradient?

            minimumHeat = mapHeat[x][y];

            //left
            if (neighbors.left.x != -1) //check for map borders
            {
                if (mapTerrainMod[x - 1][y] != -1.f) //check for obstruction
                    if (mapHeat[x - 1][y] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x - 1][y];
                        mapVector[x][y] = { -1, 0 };
                    }
            }
            //right
            if (neighbors.right.x != -1) //check for map borders
            {
                if (mapTerrainMod[x + 1][y] != -1.f) //check for obstruction
                    if (mapHeat[x + 1][y] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x + 1][y];
                        mapVector[x][y] = { 1.f, 0.f };
                    }
            }
            //up
            if (neighbors.up.x != -1) //check for map borders
            {
                if (mapTerrainMod[x][y - 1] != -1.f) //check for obstruction
                    if (mapHeat[x][y - 1] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x][y - 1];
                        mapVector[x][y] = { 0.f, -1.f };
                    }
            }
            //down
            if (neighbors.down.x != -1) //check for map borders
            {
                if (mapTerrainMod[x][y + 1] != -1.f) //check for obstruction
                    if (mapHeat[x][y + 1] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x][y + 1];
                        mapVector[x][y] = { 0.f, 1.f };
                    }
            }

            //upLeft
            if (neighbors.upLeft.x != -1) //check for map borders
            {
                if (mapTerrainMod[x-1][y - 1] != -1.f) //check for obstruction
                    if (mapHeat[x - 1][y - 1] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x - 1][y - 1];
                        mapVector[x][y] = { -1.f, -1.f };
                    }
            }
            //upRight
            if (neighbors.upRight.x != -1) //check for map borders
            {
                if (mapTerrainMod[x+1][y - 1] != -1.f) //check for obstruction
                    if (mapHeat[x + 1][y - 1] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x + 1][y - 1];
                        mapVector[x][y] = { 1.f, -1.f };
                    }
            }
            //downLeft
            if (neighbors.downLeft.x != -1) //check for map borders
            {
                if (mapTerrainMod[x-1][y + 1] != -1.f) //check for obstruction
                    if (mapHeat[x - 1][y + 1] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x - 1][y + 1];
                        mapVector[x][y] = { -1.f, 1.f };
                    }
            }
            //downRight
            if (neighbors.downRight.x != -1) //check for map borders
            {
                if (mapTerrainMod[x+1][y + 1] != -1.f) //check for obstruction
                    if (mapHeat[x+1][y + 1] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x+1][y + 1];
                        mapVector[x][y] = { 1.f, 1.f };
                    }
            }
        }
    }
}

GameActor* GameData::getActorInTile(int x, int y)
{
    GameActor* result = nullptr;
    Vector2 pos;
    for (GameActor* actor : unitsList)
    {
        pos = actor->getPosition();
        Vector2 test = { pos.x / pixelsPerTile, pos.y / pixelsPerTile };
        if (static_cast<int>(pos.x / pixelsPerTile) == x && static_cast<int>(pos.y / pixelsPerTile) == y)
        {
            result = actor;
        }
    }
    return result;
}

std::vector<GameActor*> GameData::getActorsInRadius(Vector2 center, float radius)
{
    std::vector<GameActor*> result;

    for (GameActor* actor: unitsList)
    {
        if (CheckCollisionPointCircle(actor->getPosition(), center, radius))
        {
            result.push_back(actor);
        }
    }

    return result;
}

GameActor* GameData::getNearestSpecificActor(Vector2 position, std::vector<GameActor*> actors, ActorType type, GameActor* caller)
{
    GameActor* result = nullptr;
    double minDistance;
    double buf;

    for (GameActor* actor : actors)
    {
        if (actor->type == type && actor!=caller)
        {
            if (result == nullptr) //if no actor found yet
            {
                result = actor;
                //eucledian distance
                minDistance = sqrt(pow(position.x - actor->getPosition().x, 2) + pow(position.y - actor->getPosition().y, 2)); //+ actor->size;
            }
            else
            {
                buf = sqrt(pow(position.x - actor->getPosition().x, 2) + pow(position.y - actor->getPosition().y, 2));// + actor->size;
                if (minDistance > buf)
                {
                    minDistance = buf;
                    result = actor;
                }
            }
        }
    }
    
    return result;
}

void GameData::removeActor(unsigned int ID)
{
    std::vector<GameActor*>::iterator iter;
    GameActor* buf = nullptr;
    for (iter = unitsList.begin(); iter != unitsList.end(); )
    {
        if ((*iter)->ID == ID)
        {
            buf = *iter;
            iter = unitsList.erase(iter);
        }
        else
            iter++;
    }

    std::vector<Building*>::iterator expandIter;
    for (expandIter = expansionUnitsList.begin(); expandIter != expansionUnitsList.end(); )
    {
        if ((*expandIter)->ID == ID)
            expandIter = expansionUnitsList.erase(expandIter);
        else
            expandIter++;
    }

    if(buf!=nullptr)
        delete buf;
}

//std::vector<TileIndex> GameData::tilesInPerimeterCircle(TileIndex center, unsigned int radius)
//{
//    std::vector<TileIndex> buf;
//    int x_centre = center.x, y_centre = center.y;
//    int x = radius, y = 0;
//
//    // Printing the initial point on the axes 
//    // after translation
//    
//
//    // When radius is zero only a single
//    // point will be printed
//    if (radius > 0)
//    {
//        buf.push_back(TileIndex{ x + x_centre, y_centre });  //right
//        buf.push_back(TileIndex{ x_centre, -x + y_centre }); //up
//        buf.push_back(TileIndex{ x_centre, x + y_centre });  //down
//        buf.push_back(TileIndex{ -x + x_centre,  y_centre });//left
//    }
//
//    // Initialising the value of P
//    int P = 1 - radius;
//    while (x > y)
//    {
//        y++;
//
//        //// Mid-point is inside or on the perimeter
//        //if (P <= 0)
//        //    P = P + 2 * y + 1;
//        //// Mid-point is outside the perimeter
//        //else
//        //{
//        //    x--;
//        //    P = P + 2 * y - 2 * x + 1;
//        //}
//
//        // Mid-point is inside or on the perimeter
//        if (CheckCollisionPointCircle(Vector2{ static_cast<float>(x), static_cast<float>(y) }, center, radius))
//        {
//            //x++;
//            P = P + 2 * y - 2 * x + 1;
//        }
//        else
//        {
//            x--;
//        }
//
//        // All the perimeter points have already been printed
//        /*if (x < y)
//            break;*/
//
//        // Printing the generated point and its reflection
//        // in the other octants after translation
//        buf.push_back(TileIndex{ x + x_centre, y + y_centre });
//        buf.push_back(TileIndex{ -x + x_centre, y + y_centre });
//        buf.push_back(TileIndex{ x + x_centre , -y + y_centre });
//        buf.push_back(TileIndex{ -x + x_centre , -y + y_centre });
//
//        // If the generated point is on the line x = y then 
//        // the perimeter points have already been printed
//        if (x != y)
//        {
//            buf.push_back(TileIndex{ y + x_centre , x + y_centre });
//            buf.push_back(TileIndex{ -y + x_centre , x + y_centre });
//            buf.push_back(TileIndex{ y + x_centre , -x + y_centre });
//            buf.push_back(TileIndex{ -y + x_centre , -x + y_centre });
//        }
//    }
//
//    std::vector<TileIndex> result;
//
//    for (int i = 0; i < buf.size(); i++)
//    {
//        //checking to not cross the border, if all right - add TileIndex to result vector
//        if (!(buf[i].x < 0 ||
//            buf[i].x>this->mapWidth - 1 ||
//            buf[i].y < 0 ||
//            buf[i].y>this->mapHeight - 1))
//        {
//            result.push_back(buf[i]);
//        }
//        //TODO: consider using result.erase()
//    }
//
//    return result;
//}

void GameData::GameUpdate()
{
    if (timeCount != 60)
        timeCount++;
    else
        timeCount = 0;

    if (IsKeyPressed(KEY_ESCAPE)) closed = true;
    
    mousePosition = GetMousePosition();
    mouseWorldPosition = GetScreenToWorld2D(mousePosition, camera);
    mouseIndex = { std::clamp<int>(mouseWorldPosition.x/pixelsPerTile, 0, this->mapWidth-1), std::clamp<int>(mouseWorldPosition.y / pixelsPerTile, 0, this->mapHeight-1) };

    // Camera shifting
    if ((mousePosition.x < screenSize.x * 0.15f || IsKeyDown(KEY_A)) && camera.target.x > 100)           camera.target.x -= 10 / camera.zoom;
    if ((mousePosition.x > screenSize.x * 0.85f || IsKeyDown(KEY_D)) && camera.target.x < mapSize.x)     camera.target.x += 10 / camera.zoom;
    if ((mousePosition.y < screenSize.y * 0.15f || IsKeyDown(KEY_W)) && camera.target.y > 100)           camera.target.y -= 10 / camera.zoom;
    if ((mousePosition.y > screenSize.y * 0.85f || IsKeyDown(KEY_S)) && camera.target.y < mapSize.y)     camera.target.y += 10 / camera.zoom;

    // Camera zoom controls
    camera.zoom += ((float)GetMouseWheelMove() * 0.2f);
    if (camera.zoom > 10.0f) camera.zoom = 10.0f;
    else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

    // view borders calculation
    Vector2 buf1 = GetScreenToWorld2D(Vector2{ 0,0 }, camera);
    Vector2 buf2 = GetScreenToWorld2D(Vector2{ screenSize.x,screenSize.y }, camera);
    viewBorders = {
        buf1.x,
        buf1.y,
        buf2.x - buf1.x,
        buf2.y - buf1.y
    };
    renderBorders[0] = (buf1.y / pixelsPerTile) - 1;
    renderBorders[1] = (buf1.x / pixelsPerTile) - 1;
    renderBorders[2] = (buf2.y / pixelsPerTile) + 1;
    renderBorders[3] = (buf2.x / pixelsPerTile) + 1;

    if (renderBorders[0] < 0) renderBorders[0] = 0;
    if (renderBorders[1] < 0) renderBorders[1] = 0;
    if (renderBorders[2] > mapHeight) renderBorders[2] = mapHeight;
    if (renderBorders[3] > mapWidth) renderBorders[3] = mapWidth;

    if (IsKeyPressed(KEY_ONE))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::LIGHT_INSECT;

    if (IsKeyPressed(KEY_E))
        if(wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::TUMOR;
        else
            wantToBuild = ActorType::ACTOR_NULL;

    if (IsKeyPressed(KEY_C))
        clearMap();

    if (IsKeyPressed(KEY_R))
        if (wantToRemove)
            wantToRemove = false;
        else
        {
            wantToBuild = ActorType::ACTOR_NULL;
            wantToRemove = true;
        }

    if (IsKeyPressed(KEY_T))
    {
        calculateVectorPathfinding(
            mouseIndex,
            ActorType::LIGHT_INSECT
        );
        calculateVectorPathfinding(
            mouseIndex,
            ActorType::HEAVY_INSECT
        );
        calculateVectorPathfinding(
            mouseIndex,
            ActorType::FLYING_INSECT
        );
    }

    if (IsKeyPressed(KEY_F2))
        if (showingCreepStates)
            showingCreepStates = false;
        else
            showingCreepStates = true;

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && wantToBuild != ActorType::ACTOR_NULL)
    {
        //right-click to clear
        wantToBuild = ActorType::ACTOR_NULL;
    }

    //TODO: replace to separate method
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && wantToBuild != ActorType::ACTOR_NULL)
    {
        switch (wantToBuild)
        {
        case ActorType::LIGHT_TURRET:
            break;
        case ActorType::HEAVY_TURRET:
            break;
        case ActorType::AIRDEFENSE_TURRET:
            break;
        case ActorType::LIGHT_INSECT:
            unitsList.push_back(
                new LightInsect(
                    this,
                    ActorType::LIGHT_INSECT,
                    Vector2{ mouseIndex.x * pixelsPerTile + pixelsPerTile / 2, mouseIndex.y * pixelsPerTile + pixelsPerTile / 2 },
                    State::ONLINE)
            );
            break;
        case ActorType::HEAVY_INSECT:
            break;
        case ActorType::FLYING_INSECT:
            break;
        case ActorType::CORE:
            break;
        case ActorType::BASE:
            break;
        case ActorType::HIVE:
            break;
        case ActorType::TUMOR:
            Building* buf;
            buf = new Tumor(
                this,
                ActorType::TUMOR,
                Vector2{ mouseIndex.x * pixelsPerTile + pixelsPerTile / 2, mouseIndex.y * pixelsPerTile + pixelsPerTile / 2 },
                State::ONLINE);
            unitsList.push_back(
                buf
            );
            expansionUnitsList.push_back(
                buf
            );
            wantToBuild = ActorType::ACTOR_NULL;
            break;
        default:
            break;
        }
    }

    //if(timeCount %30==0) //slowing down
    for (GameActor* actor : this->unitsList)
    {
        actor->Update();
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && wantToRemove)
    {
        GameActor *buf = getActorInTile(mouseIndex.x, mouseIndex.y);
        if(buf!=nullptr)
            removeActor(buf->ID);
        wantToRemove = false;
    }

    //expansion fading
    if (timeCount  == 0)
    {
        std::vector<TileIndex>buf;
        //INSECTS
        for (int x = 0; x < mapWidth; x++)
        {
            for (int y = 0; y < mapHeight; y++)
            {
                if (mapExpansionCreep[x][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                {
                    if (numOfExpansionTileAdjoinFading(x, y, Side::INSECTS) < 4)
                        buf.push_back(TileIndex{ x,y });
                }
            }
        }

        creepCount -= buf.size();

        for (TileIndex tile : buf)
            mapExpansionCreep[tile.x][tile.y] = ExpandState::AVAILABLE;
        //TODO: do same for MACHINES
    }
}

void GameData::GameDraw()
{
    BeginMode2D(camera);
    //TODO: test 
    //draw terrain

#ifndef TILE_DRAWING
    DrawTextureEx(
        terrainTexture,
        Vector2{ 0.f, 0.f },
        0.f, pixelsPerTile, WHITE);
#else

    if (camera.zoom < 1.f && !IsKeyDown(KEY_T)) //low-detailed map
    {
        DrawTextureEx(
            terrainTexture,
            Vector2{ 0.f, 0.f },
            0.f, pixelsPerTile, WHITE);
    }
    else
    {                    //full-detailed map
        Vector2** vectorField = vectorFields[ActorType::LIGHT_INSECT];
        float** terrainMod = mapsPathfinding[ActorType::LIGHT_INSECT]["mapsTerrainMod"];

        int index;
        for (int x = renderBorders[1]; x < renderBorders[3]; x++) //columns
        {
            for (int y = renderBorders[0]; y < renderBorders[2]; y++) //rows
            {
                index = mapWidth * y + x;

                //ACCESS VIOLATION
                if (mapExpansionCreep[x][y] == ExpandState::EXPANDED || mapExpansionCreep[x][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                {
                    float x_exp = static_cast<float>(x % (creepTexture.width / static_cast<int>(pixelsPerTile)));
                    float y_exp = static_cast<float>(y % (creepTexture.height / static_cast<int>(pixelsPerTile)));
                    Rectangle buf = { 
                        x_exp* pixelsPerTile,
                        y_exp* pixelsPerTile,
                        pixelsPerTile,
                        pixelsPerTile };
                    
                    DrawTextureRec(
                        creepTexture,
                        buf,
                        Vector2{ x * pixelsPerTile, y * pixelsPerTile, },
                        WHITE);
                }
                else
                {
                    DrawTexture(
                        tilesetTex[static_cast<int>(mapTerrain[index])],
                        x * pixelsPerTile,
                        y * pixelsPerTile,
                        WHITE);
                }

                if (showingCreepStates)
                    DrawText(FormatText("%d", mapExpansionCreep[x][y]), x * pixelsPerTile + pixelsPerTile / 3, y * pixelsPerTile + pixelsPerTile / 3, 14, RED);
            
                if(IsKeyDown(KEY_F4))
                    DrawText(FormatText("%.0f", mapsPathfinding[ActorType::LIGHT_INSECT]["mapsHeat"][x][y]), x * pixelsPerTile + pixelsPerTile / 3, y * pixelsPerTile + pixelsPerTile / 3, 10, SKYBLUE);

                if (IsKeyDown(KEY_F5) && terrainMod[x][y]!=-1.f)
                {                   
                    DrawLineEx(
                        Vector2{ x * pixelsPerTile + pixelsPerTile / 2,
                        y * pixelsPerTile + pixelsPerTile / 2 },
                        Vector2{ x * pixelsPerTile + pixelsPerTile / 2 + vectorField[x][y].x *2,
                        y * pixelsPerTile + pixelsPerTile / 2 + vectorField[x][y].y * 2,
                        },
                        3.f,
                        GREEN);
                    DrawCircle(x * pixelsPerTile + pixelsPerTile / 2, y * pixelsPerTile + pixelsPerTile / 2, 1, RED);
                }
            }
        }
    }

    DrawRectangleLinesEx(
        viewBorders,
        5,
        RED
    );

    //DrawRectangle(camera.target.x, camera.target.y, 32, 32, YELLOW);
#endif

    if (this->wantToBuild != ActorType::ACTOR_NULL)
    {
        //draw hologram of chosen actor in any case
        switch (wantToBuild)
        {
        case ActorType::LIGHT_TURRET:
        case ActorType::HEAVY_TURRET:
        case ActorType::AIRDEFENSE_TURRET:
            //draw red circle of attack range and yellow - of view range
            break;
        case ActorType::LIGHT_INSECT:
        case ActorType::HEAVY_INSECT:
        case ActorType::FLYING_INSECT:
            break;
        case ActorType::CORE:
            //draw circle of connection range
        case ActorType::BASE:
        case ActorType::HIVE:
        case ActorType::TUMOR:
            //draw blue circle of expansion
            for (TileIndex tile : tilesInsideCircle(Vector2{ static_cast<float>(mouseIndex.x), static_cast<float>(mouseIndex.y) }, radius))
            {
                DrawRectangle(tile.x * pixelsPerTile, tile.y * pixelsPerTile, pixelsPerTile, pixelsPerTile, Fade(SKYBLUE, 0.5f));
            }
            break;
        }
    }

    if (wantToRemove)
    {
        DrawRectangle(mouseIndex.x * pixelsPerTile, mouseIndex.y * pixelsPerTile, pixelsPerTile, pixelsPerTile, Fade(RED, 0.3f));
    }

    for (GameActor* actor : this->unitsList)
    {
        //TODO: checking, is actor inside of screen rectangle (render it or not)
        actor->Draw();
    }

    EndMode2D();
    DrawText(FormatText("%f", camera.zoom), 20, 20, 20, RED);
    DrawText(FormatText("Tiles rendering: %d", (renderBorders[2] - renderBorders[0])* (renderBorders[3] - renderBorders[1])), 20, 80, 20, RED);

    DrawFPS(20, 50);
}

GameData::~GameData()
{
    //Unloading images and textures

    if (isMapLoaded())
    {
        delete[] mapTerrain;
        UnloadTexture(terrainTexture);

        for (int x = 0; x < mapWidth; x++)
            delete[] mapExpansionCreep[x];
        delete[] mapExpansionCreep;

        ActorType types[] = { ActorType::LIGHT_INSECT, ActorType::HEAVY_INSECT, ActorType::FLYING_INSECT };
        std::vector<std::string> matrices = { "mapsHeat", "mapsTerrainMod", "mapsDamage" };
        for (ActorType type : types)
        {
            for (std::string matrixName : matrices)
            {
                for (int x = 0; x < mapWidth; x++)
                    delete[] mapsPathfinding[type][matrixName][x];
                delete[] mapsPathfinding[type][matrixName];
            }

            //vector matrix
            for (int x = 0; x < mapWidth; x++)
                delete[] vectorFields[type][x];
            delete[] vectorFields[type];
        }

        //neightbors matrix
        for (int x = 0; x < mapWidth; x++)
            delete[] neighborsIndices[x];
        delete[] neighborsIndices;
    }

    UnloadTexture(creepTexture);

    for (int i = 0; i < 7; i++)
    {
        UnloadTexture(tilesetTex[i]);
    }

    delete[] palette;
}