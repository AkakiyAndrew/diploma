#include "GameClasses.h"

#define TILE_DRAWING

float max(float a, float b)
{
    if (a > b)
        return a;
    else
        return b;
}

GameData::GameData()
{
    screenSize = Vector2{ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };

    camera.target = { screenSize.x / 2, screenSize.y / 2 };
    camera.offset = { screenSize.x / 2, screenSize.y / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    palette = new Color[]{ DARKBLUE, BLUE, YELLOW, GREEN, DARKGREEN, GRAY, BLACK };

    //TILESET TEXTURES

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
        tilesetTex[i] = LoadTexture(filenames[i]);
    }

    //EXPANSION TEXTURES
    expansionInsectsTexture = LoadTexture("textures\\source\\creep_1.png");
    expansionMachinesAnimation = {
        new Texture2D[15],
        15
    };

    for (int i = 0; i < expansionMachinesAnimation.framesAmount; i++)
    {
        expansionMachinesAnimation.frames[i] = LoadTexture(TextFormat("textures\\tileset\\machines_expansion_frames\\%d.png", i));
    }

    expansionMachinesAnimation.width = expansionMachinesAnimation.frames[0].width;
    expansionMachinesAnimation.height = expansionMachinesAnimation.frames[0].height;

    //UNIT SPRITES

    //загрузку проводить циклом по всем нужным юнитам, везде одно и то же, подставлять форматом текста и называть файлы по типу "unitType_unitState_frameNum.png"
    //LIGHT INSECT
    unitAnimations[ActorType::LIGHT_INSECT][State::GOES] =
    {
        new Texture2D[5],
        5
    };

    for (int i = 0; i < 5; i++)
    {
        unitAnimations[ActorType::LIGHT_INSECT][State::GOES].frames[i] = LoadTexture(TextFormat("textures\\sprites\\lightInsect_goes_%d.png", i));
    }

    //TURRET CHASIS
    //CHANGING_MODE
    unitAnimations[ActorType::TURRET_CHASIS][State::CHANGING_MODE] =
    {
        new Texture2D[4],
        4
    };
    for (int i = 0; i < 4; i++)
    {
        unitAnimations[ActorType::TURRET_CHASIS][State::CHANGING_MODE].frames[i] = LoadTexture(TextFormat("textures\\sprites\\turretChasis_changeMode_%d.png", i));
    }
    //GOES
    unitAnimations[ActorType::TURRET_CHASIS][State::GOES] =
    {
        new Texture2D[3],
        3
    };
    for (int i = 0; i < 3; i++)
    {
        unitAnimations[ActorType::TURRET_CHASIS][State::GOES].frames[i] = LoadTexture(TextFormat("textures\\sprites\\turretChasis_goes_%d.png", i));
    }

    //HEAVY_TURRET
    unitAnimations[ActorType::HEAVY_TURRET][State::ONLINE] =
    {
        new Texture2D[1],
        1
    };
    for (int i = 0; i < 1; i++)
    {
        unitAnimations[ActorType::HEAVY_TURRET][State::ONLINE].frames[i] = LoadTexture(TextFormat("textures\\sprites\\heavyTurret_online_%d.png", i));
    }

    unitAnimations[ActorType::HEAVY_TURRET][State::ATTACKING] =
    {
        new Texture2D[6],
        6
    };
    for (int i = 0; i < 6; i++)
    {
        unitAnimations[ActorType::HEAVY_TURRET][State::ATTACKING].frames[i] = LoadTexture(TextFormat("textures\\sprites\\heavyTurret_attacking_%d.png", i));
    }

    //TODO: texture files loading, units properties

    //GENERIC ACTORS ATTRIBUTES
    genericAttributes[ActorType::TUMOR] = std::map<std::string, int>{
        {"maxHP", 100},
        {"size", 16},
        {"cost", 10},
        {"sightRange", 10},
        {"armor", 5},
    };
    genericAttributes[ActorType::CORE] = std::map<std::string, int>{
        {"maxHP", 100},
        {"size", 16},
        {"cost", 50},
        {"sightRange", 10},
        {"armor", 4},
    };
    genericAttributes[ActorType::BASE] = std::map<std::string, int>{
        {"maxHP", 300},
        {"size", 32},
        {"cost", 1000},
        {"sightRange", 12},
        {"armor", 10},
    };
    genericAttributes[ActorType::HEAVY_TURRET] = std::map<std::string, int>{
        {"maxHP", 200},
        {"size", 16},
        {"cost", 150},
        {"sightRange", 12},
        {"armor", 5},
    };
    genericAttributes[ActorType::LIGHT_INSECT] = std::map<std::string, int>{
        {"maxHP", 50},
        {"size", 8},
        {"cost", 5},
        {"sightRange", 3},
        {"armor", 1},
    };

    //BUILDING ACTORS ATTRIBUTES
    buildingsAttributes[ActorType::TUMOR] = std::map<std::string, int>{
        {"expansionRange", 8},
        {"expansionTime", 8 },
    };
    buildingsAttributes[ActorType::HIVE] = std::map<std::string, int>{
        {"expansionRange", 12},
        {"expansionTime", 8 },
    };
    buildingsAttributes[ActorType::CORE] = std::map<std::string, int>{
        {"expansionRange", 8},
        {"expansionTime", 16 },
    };
    buildingsAttributes[ActorType::BASE] = std::map<std::string, int>{
        {"expansionRange", 12},
        {"expansionTime", 16 },
    };

    //MILITARY ACTORS ATTRIBUTES
    militaryAttributes[ActorType::LIGHT_INSECT] = std::map<std::string, int>{
        {"seekRange", 8 * pixelsPerTile},
        {"attackRange", 2 * pixelsPerTile},
        {"speed", 2},
        {"damage", 15},
        {"rotationSpeed", 8},
        {"cooldownDuration", 45}, //ticks to reload
    };
    militaryAttributes[ActorType::HEAVY_TURRET] = std::map<std::string, int>{
        {"seekRange", 10 * pixelsPerTile},
        {"attackRange", 7 * pixelsPerTile},
        {"speed", 1},
        {"damage", 70},
        {"rotationSpeed", 2},
        {"cooldownDuration", 240}, //ticks to reload
    };

    //CONNECTABLE ACTORS ATTRIBUTES
    connectableAttributes[ActorType::CORE] = std::map<std::string, int>
    {
        {"connectRange", 8 * pixelsPerTile},
    };
    connectableAttributes[ActorType::LIGHT_TURRET] = std::map<std::string, int>
    {
        {"connectRange", 6 * pixelsPerTile},
    };
    connectableAttributes[ActorType::HEAVY_TURRET] = std::map<std::string, int>
    {
        {"connectRange", 6 * pixelsPerTile},
    };
    connectableAttributes[ActorType::AIRDEFENSE_TURRET] = std::map<std::string, int>
    {
        {"connectRange", 6 * pixelsPerTile},
    };

    //CONSTRUCTORS ATTRIBUTES
    constructorsAttributes[ActorType::CORE] = std::map<std::string, int>
    {
        {"buildPower", 10},
        {"buildRange", 6 * pixelsPerTile},
    };
    constructorsAttributes[ActorType::BASE] = std::map<std::string, int>
    {
        {"buildPower", 20},
        {"buildRange", 8 * pixelsPerTile},
    };

    //TURRETS ATTRIBUTES
    turretsAttributes[ActorType::LIGHT_TURRET] = std::map<std::string, int>
    {
        {"maxCharge", 50},
        {"chargeRate", 2},
        {"energyPerShot", 5},
    };
    turretsAttributes[ActorType::HEAVY_TURRET] = std::map<std::string, int>
    {
        {"maxCharge", 100},
        {"chargeRate", 2},
        {"energyPerShot", 20},
    };
    turretsAttributes[ActorType::AIRDEFENSE_TURRET] = std::map<std::string, int>
    {
        {"maxCharge", 50},
        {"chargeRate", 2},
        {"energyPerShot", 5},
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
        if(actor!=nullptr)
            IDs.push_back(actor->ID);
    }
    
    for (unsigned int ID : IDs)
    {
        removeActor(ID);
    }

    lastID = 0;
    resourcesInsects = 0;
    resourcesMachines = 0;
    //creepTilesCount = 0;
    //energisedTilesCount = 0;
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

        //fog of war
        Side sides[] = {Side::INSECTS, Side::MACHINES};
        for (Side side : sides)
        {
            for (int x = 0; x < mapWidth; x++)
                delete[] mapsFogOfWar[side][x];
            delete[] mapsFogOfWar[side];
        }
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

    mapExpansionEnergised = new unsigned char* [mapWidth];
    for (int x = 0; x < mapWidth; x++)
        mapExpansionEnergised[x] = new unsigned char[mapHeight];

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
                mapExpansionEnergised[x][y] = ExpandState::UNAVAILABLE;
                break;
            //if expansion can be provide
            case TerrainType::SAND:
            case TerrainType::PLAIN:
            case TerrainType::TREE:
            case TerrainType::STONE:
            case TerrainType::ASH:
                mapExpansionCreep[x][y] = ExpandState::AVAILABLE;
                mapExpansionEnergised[x][y] = ExpandState::AVAILABLE;
                break;
            //just in case
            default:
                mapExpansionCreep[x][y] = ExpandState::UNAVAILABLE;
                mapExpansionEnergised[x][y] = ExpandState::UNAVAILABLE;
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

    //fog of war
    Side sides[] = { Side::INSECTS, Side::MACHINES };
    for (Side side : sides)
    {
        mapsFogOfWar[side] = new int*[mapWidth];
        for (int x = 0; x < mapWidth; x++)
            mapsFogOfWar[side][x] = new int[mapHeight];

        int** ptr = mapsFogOfWar[side];
        for (int x = 0; x < mapWidth; x++)
        {
            for (int y = 0; y < mapHeight; y++)
                ptr[x][y] = -1;
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
    
    if(side==Side::MACHINES)
    {
        if (x - 1 >= 0) //left
            if (mapExpansionEnergised[x - 1][y] == ExpandState::EXPANDED || mapExpansionEnergised[x - 1][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (y - 1 >= 0) //up
            if (mapExpansionEnergised[x][y - 1] == ExpandState::EXPANDED || mapExpansionEnergised[x][y - 1] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (x + 1 < this->mapWidth) //right
            if (mapExpansionEnergised[x + 1][y] == ExpandState::EXPANDED || mapExpansionEnergised[x + 1][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (y + 1 < this->mapHeight) //down
            if (mapExpansionEnergised[x][y + 1] == ExpandState::EXPANDED || mapExpansionEnergised[x][y + 1] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
    }

    return result;
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

    if (side == Side::MACHINES)
    {
        if (x - 1 >= 0) //left
            if (mapExpansionEnergised[x - 1][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (y - 1 >= 0) //up
            if (mapExpansionEnergised[x][y - 1] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (x + 1 < this->mapWidth) //right
            if (mapExpansionEnergised[x + 1][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
        if (y + 1 < this->mapHeight) //down
            if (mapExpansionEnergised[x][y + 1] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                result++;
    }

    return result;
}

bool GameData::isTileExpanded(TileIndex tile, Side side)
{
    if (side == Side::INSECTS)
    {
        if (mapExpansionCreep[tile.x][tile.y] == ExpandState::EXPANDED || mapExpansionCreep[tile.x][tile.y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
            return true;
    }
    
    if(side == Side::MACHINES)
    {
        if (mapExpansionEnergised[tile.x][tile.y] == ExpandState::EXPANDED || mapExpansionEnergised[tile.x][tile.y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
            return true;
    }

    return false;
}
bool GameData::isTileInBounds(TileIndex tile)
{
    if (
        tile.x < 0 ||
        tile.x>this->mapWidth - 1 ||
        tile.y < 0 ||
        tile.y>this->mapHeight - 1
        )
        return false;
    else
        return true;
}
TileIndex GameData::getTileIndex(Vector2 position)
{
    return TileIndex{
        static_cast<int>(position.x / pixelsPerTile),
        static_cast<int>(position.y / pixelsPerTile)
    };
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
std::vector<TileIndex> GameData::getNeighborsAsVector(int x, int y)
{
    std::vector<TileIndex> result;
    result.resize(9);
    NeighborsIndex neighbors = neighborsIndices[x][y];

    //this tile
    result[0] = TileIndex{ x,y };

    //left
    if (neighbors.left.x != -1)
        result[1] = neighbors.left;
    else
        result[1] = { -1,-1 };
    //up
    if (neighbors.up.x != -1)
        result[2] = neighbors.up;
    else
        result[2] = { -1,-1 };
    //right
    if (neighbors.right.x != -1)
        result[3] = neighbors.right;
    else
        result[3] = { -1,-1 };
    //down
    if (neighbors.down.x != -1)
        result[4] = neighbors.down;
    else
        result[4] = { -1,-1 };

    //upLeft
    if (neighbors.upLeft.x != -1)
        result[5] = neighbors.upLeft;
    else
        result[5] = { -1,-1 };
    //upRight
    if (neighbors.upRight.x != -1)
        result[6] = neighbors.upRight;
    else
        result[6] = { -1,-1 };
    //downLeft
    if (neighbors.downLeft.x != -1)
        result[7] = neighbors.downLeft;
    else
        result[7] = { -1,-1 };
    //downRight
    if (neighbors.downRight.x != -1)
        result[8] = neighbors.downRight;
    else
        result[8] = { -1,-1 };

    return result;
}
bool GameData::isOnLineOfSight(TileIndex pos1, TileIndex pos2, ActorType unitType)
{
    bool result = true;
    float** mapMod = nullptr;

    switch (unitType)
    {
    case ActorType::LIGHT_TURRET:
    case ActorType::HEAVY_TURRET:
        //can shoot only when no mountains on sight
        mapMod = mapsPathfinding[ActorType::FLYING_INSECT]["mapsTerrainMod"];
        break;
    case ActorType::AIRDEFENSE_TURRET:
        //can shoot everywhere
        mapMod = mapsPathfinding[ActorType::FLYING_INSECT]["mapsTerrainMod"];
        break;
    case ActorType::LIGHT_INSECT:
    case ActorType::HEAVY_INSECT:
    case ActorType::FLYING_INSECT:
        //checking line of sight according to type of insect
        mapMod = mapsPathfinding[unitType]["mapsTerrainMod"];
        break;
    default:
        break;
    }

    int x0 = pos1.x;
    int y0 = pos1.y;
    int x1 = pos2.x;
    int y1 = pos2.y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int x = x0;
    int y = y0;
    int n = 1 + dx + dy;
    int x_inc = (x1 > x0) ? 1 : -1;
    int y_inc = (y1 > y0) ? 1 : -1;
    int error = dx - dy;
    dx *= 2;
    dy *= 2;

    for (; n > 0; --n)
    {
        //checking tile here
        if (mapMod[x][y] == -1.f)
            return false;

        if (error > 0)
        {
            x += x_inc;
            error -= dy;
        }
        else
        {
            y += y_inc;
            error += dx;
        }
    }

    //returns true, if all checks fine
    return result;
}

void GameData::recalculateExpansion(Side side)
{
    if (side == Side::INSECTS)
    {
        for (Building* actor : this->expansionUnitsList_Insects)
        {
            if (actor->getState() == State::ONLINE)
                actor->markAreaExpand();
        }
    }
    
    if (side == Side::MACHINES)
    {
        for (Building* actor : this->expansionUnitsList_Machines)
        {
            if(actor->getState()==State::ONLINE)
                actor->markAreaExpand();
        }
    }
}
void GameData::recalculateMilitaryTargets(Side side)
{
    for (Militaty* unit : militaryUnitsList)
    {
        if (unit->side == side)
            unit->SeekForEnemy();
    }
}

void GameData::calculateVectorPathfinding(TileIndex target, ActorType actorType)
{
    float** mapHeat = mapsPathfinding[actorType]["mapsHeat"];
    float** mapTerrainMod = mapsPathfinding[actorType]["mapsTerrainMod"];
    Vector2** mapVector = vectorFields[actorType];

    int** mapFogOfWar = mapsFogOfWar[Side::INSECTS];

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
    //std::vector<TileIndex> checking;
    float previous = 1.f;
    
    //TODO: возможная оптимизация с заменой вектора на фиксированный одномерный массив, используя индексы для заполнения и обращения
    //внутри итерации менять iLastAdded для обоз
    for(int i = 0; i<i_max; i++)
    {
        if (i <= i_last_added) //for "repair" crush on Degug configuration
        {
            previous = mapHeat[toCheck[i].x][toCheck[i].y]; //previous - to calculate neighbors tiles
            checking = neighborsIndices[toCheck[i].x][toCheck[i].y];
            //checking = getNeighborsAsVector(toCheck[i].x, toCheck[i].y);
        }

        //left
        if (checking.left.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[checking.left.x][checking.left.y] == 0.f)
            {
                //if there's no fog of war:
                if (mapFogOfWar[checking.left.x][checking.left.y] == -1)
                {
                    i_last_added++;
                    mapHeat[checking.left.x][checking.left.y] = 1.f;
                    toCheck[i_last_added] = (checking.left);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[checking.left.x][checking.left.y] != -1)
                    {
                        i_last_added++;
                        //TODO: ADD CONSIDERING MAP DAMAGE
                        //set value, according to terrain speed modification and damage map

                        mapHeat[checking.left.x][checking.left.y] = previous + (1 / mapTerrainMod[checking.left.x][checking.left.y]);
                        //add this checking.left for further checking
                        toCheck[i_last_added] = (checking.left);
                    }
                }
            }
        }
        //up
        if (checking.up.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[checking.up.x][checking.up.y] == 0.f)
            {
                //if there's no fog of war:
                if (mapFogOfWar[checking.up.x][checking.up.y] == -1)
                {
                    i_last_added++;
                    mapHeat[checking.up.x][checking.up.y] = 1.f;
                    toCheck[i_last_added] = (checking.up);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[checking.up.x][checking.up.y] != -1)
                    {
                        i_last_added++;
                        //TODO: ADD CONSIDERING MAP DAMAGE
                        //set value, according to terrain speed modification and damage map

                        mapHeat[checking.up.x][checking.up.y] = previous + (1 / mapTerrainMod[checking.up.x][checking.up.y]);
                        //add this checking.up for further checking
                        toCheck[i_last_added] = (checking.up);
                    }
                }
            }
        }
        //right
        if (checking.right.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[checking.right.x][checking.right.y] == 0.f)
            {
                //if there's no fog of war:
                if (mapFogOfWar[checking.right.x][checking.right.y] == -1)
                {
                    i_last_added++;
                    mapHeat[checking.right.x][checking.right.y] = 1.f;
                    toCheck[i_last_added] = (checking.right);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[checking.right.x][checking.right.y] != -1)
                    {
                        i_last_added++;
                        //TODO: ADD CONSIDERING MAP DAMAGE
                        //set value, according to terrain speed modification and damage map

                        mapHeat[checking.right.x][checking.right.y] = previous + (1 / mapTerrainMod[checking.right.x][checking.right.y]);
                        //add this checking.right for further checking
                        toCheck[i_last_added] = (checking.right);
                    }
                }
            }
        }
        //down
        if (checking.down.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[checking.down.x][checking.down.y] == 0.f)
            {
                //if there's no fog of war:
                if (mapFogOfWar[checking.down.x][checking.down.y] == -1)
                {
                    i_last_added++;
                    mapHeat[checking.down.x][checking.down.y] = 1.f;
                    toCheck[i_last_added] = (checking.down);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[checking.down.x][checking.down.y] != -1)
                    {
                        i_last_added++;
                        //TODO: ADD CONSIDERING MAP DAMAGE
                        //set value, according to terrain speed modification and damage map

                        mapHeat[checking.down.x][checking.down.y] = previous + (1 / mapTerrainMod[checking.down.x][checking.down.y]);
                        //add this checking.down for further checking
                        toCheck[i_last_added] = (checking.down);
                    }
                }
            }
        }

        //upLeft
        if (checking.upLeft.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[checking.upLeft.x][checking.upLeft.y] == 0.f)
            {
                //if there's no fog of war:
                if (mapFogOfWar[checking.upLeft.x][checking.upLeft.y] == -1)
                {
                    i_last_added++;
                    mapHeat[checking.upLeft.x][checking.upLeft.y] = 1.f;
                    toCheck[i_last_added] = (checking.upLeft);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[checking.upLeft.x][checking.upLeft.y] != -1 && (mapTerrainMod[checking.up.x][checking.up.y] != -1.f || mapTerrainMod[checking.left.x][checking.left.y] != -1.f))
                    {
                        i_last_added++;
                        //TODO: ADD CONSIDERING MAP DAMAGE
                        //set value, according to terrain speed modification and damage map

                        mapHeat[checking.upLeft.x][checking.upLeft.y] = previous + (1 / mapTerrainMod[checking.upLeft.x][checking.upLeft.y]);
                        //add this checking.upLeft for further checking
                        toCheck[i_last_added] = (checking.upLeft);
                    }
                }
            }
        }
        //upRight
        if (checking.upRight.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[checking.upRight.x][checking.upRight.y] == 0.f)
            {
                //if there's no fog of war:
                if (mapFogOfWar[checking.upRight.x][checking.upRight.y] == -1)
                {
                    i_last_added++;
                    mapHeat[checking.upRight.x][checking.upRight.y] = 1.f;
                    toCheck[i_last_added] = (checking.upRight);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[checking.upRight.x][checking.upRight.y] != -1 && (mapTerrainMod[checking.up.x][checking.up.y] != -1.f || mapTerrainMod[checking.right.x][checking.right.y] != -1.f))
                    {
                        i_last_added++;
                        //TODO: ADD CONSIDERING MAP DAMAGE
                        //set value, according to terrain speed modification and damage map

                        mapHeat[checking.upRight.x][checking.upRight.y] = previous + (1 / mapTerrainMod[checking.upRight.x][checking.upRight.y]);
                        //add this checking.upRight for further checking
                        toCheck[i_last_added] = (checking.upRight);
                    }
                }
            }
        }
        //downLeft
        if (checking.downLeft.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[checking.downLeft.x][checking.downLeft.y] == 0.f)
            {
                //if there's no fog of war:
                if (mapFogOfWar[checking.downLeft.x][checking.downLeft.y] == -1)
                {
                    i_last_added++;
                    mapHeat[checking.downLeft.x][checking.downLeft.y] = 1.f;
                    toCheck[i_last_added] = (checking.downLeft);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[checking.downLeft.x][checking.downLeft.y] != -1 && (mapTerrainMod[checking.down.x][checking.down.y] != -1.f || mapTerrainMod[checking.left.x][checking.left.y] != -1.f))
                    {
                        i_last_added++;
                        //TODO: ADD CONSIDERING MAP DAMAGE
                        //set value, according to terrain speed modification and damage map
                        mapHeat[checking.downLeft.x][checking.downLeft.y] = previous + (1 / mapTerrainMod[checking.downLeft.x][checking.downLeft.y]);
                        //add this checking.downLeft for further checking
                        toCheck[i_last_added] = (checking.downLeft);
                    }
                }
            }
        }
        //downRight
        if (checking.downRight.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[checking.downRight.x][checking.downRight.y] == 0.f)
            {
                //if there's no fog of war:
                if (mapFogOfWar[checking.downRight.x][checking.downRight.y] == -1)
                {
                    i_last_added++;
                    mapHeat[checking.downRight.x][checking.downRight.y] = 1.f;
                    toCheck[i_last_added] = (checking.downRight);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[checking.downRight.x][checking.downRight.y] != -1 && (mapTerrainMod[checking.down.x][checking.down.y] != -1.f || mapTerrainMod[checking.right.x][checking.right.y] != -1.f))
                    {
                        i_last_added++;
                        //TODO: ADD CONSIDERING MAP DAMAGE
                        //set value, according to terrain speed modification and damage map
                        mapHeat[checking.downRight.x][checking.downRight.y] = previous + (1 / mapTerrainMod[checking.downRight.x][checking.downRight.y]);
                        //add this checking.downRight for further checking
                        toCheck[i_last_added] = (checking.downRight);
                    }
                }
            }
        }


        //for (TileIndex tile : checking)
        //{
        //    if (tile.x != -1) //if index valid
        //    {
        //        //if value not set:
        //        if (mapHeat[tile.x][tile.y] == 0.f)
        //        {
        //            //if there's no fog of war:
        //            if (mapFogOfWar[tile.x][tile.y] == -1)
        //            {
        //                i_last_added++;
        //                mapHeat[tile.x][tile.y] = 1.f;
        //                toCheck[i_last_added] = (tile);
        //            }
        //            else
        //            {
        //                //if there's no obstacle:
        //                if (mapTerrainMod[tile.x][tile.y] != -1)
        //                {
        //                    i_last_added++;
        //                    //TODO: ADD CONSIDERING MAP DAMAGE
        //                    //set value, according to terrain speed modification and damage map
        //                    mapHeat[tile.x][tile.y] = previous + (1 / mapTerrainMod[tile.x][tile.y]);
        //                    //add this tile for further checking
        //                    toCheck[i_last_added] = (tile);
        //                }
        //            }
        //        }
        //    }
        //}

        //left
        //if (checking.left.x != -1 ) //if index valid
        //{
        //    //if value not set:
        //    if (mapHeat[checking.left.x][checking.left.y] == 0.f)
        //    {
        //        //if there's no fog of war:
        //        if (mapFogOfWar[checking.left.x][checking.left.y] == -1)
        //        {
        //            i_last_added++;
        //            mapHeat[checking.left.x][checking.left.y] = 1.f;
        //            toCheck[i_last_added] = (checking.left);
        //        }
        //        else
        //        {
        //            //if there's no obstacle:
        //            if (mapTerrainMod[checking.left.x][checking.left.y] != -1)
        //            {
        //                i_last_added++;
        //                //TODO: ADD CONSIDERING MAP DAMAGE
        //                //set value, according to terrain speed modification and damage map
        //                mapHeat[checking.left.x][checking.left.y] = previous + (1 / mapTerrainMod[checking.left.x][checking.left.y]);
        //                //add this tile for further checking
        //                toCheck[i_last_added] = (checking.left);
        //            }
        //        }
        //    }
        //}
        ////up
        //if (checking.up.x != -1 ) //if index valid
        //{
        //    //if value not set:
        //    if (mapHeat[checking.up.x][checking.up.y] == 0.f)
        //    {
        //        //if there's no fog of war:
        //        if (mapFogOfWar[checking.up.x][checking.up.y] == -1)
        //        {
        //            i_last_added++;
        //            mapHeat[checking.up.x][checking.up.y] = 1.f;
        //            toCheck[i_last_added] = (checking.up);
        //        }
        //        else
        //            //if there's no obstacle:
        //            if (mapTerrainMod[checking.up.x][checking.up.y] != -1)
        //            {
        //                i_last_added++;
        //                //TODO: ADD CONSIDERING MAP DAMAGE
        //                //set value, according to terrain speed modification and damage map
        //                mapHeat[checking.up.x][checking.up.y] = previous + (1 / mapTerrainMod[checking.up.x][checking.up.y]);
        //                //add this tile for further checking
        //                toCheck[i_last_added] = (checking.up);
        //            }
        //    }
        //}
        ////right
        //if (checking.right.x != -1 ) //if index valid
        //{
        //    //if value not set:
        //    if (mapHeat[checking.right.x][checking.right.y] == 0.f)
        //    {
        //        //if there's no fog of war:
        //        if (mapFogOfWar[checking.right.x][checking.right.y] == -1)
        //        {
        //            i_last_added++;
        //            mapHeat[checking.right.x][checking.right.y] = 1.f;
        //            toCheck[i_last_added] = (checking.right);
        //        }
        //        else
        //            //if there's no obstacle:
        //            if (mapTerrainMod[checking.right.x][checking.right.y] != -1)
        //            {
        //                i_last_added++;
        //                //TODO: ADD CONSIDERING MAP DAMAGE
        //                //set value, according to terrain speed modification and damage map
        //                mapHeat[checking.right.x][checking.right.y] = previous + (1 / mapTerrainMod[checking.right.x][checking.right.y]);
        //                //add this tile for further checking
        //                toCheck[i_last_added] = (checking.right);
        //            }
        //    }
        //}
        ////down
        //if (checking.down.x != -1) //if index valid
        //{
        //    //if value not set:
        //    if (mapHeat[checking.down.x][checking.down.y] == 0.f)
        //    {
        //        //if there's no fog of war:
        //        if (mapFogOfWar[checking.down.x][checking.down.y] == -1)
        //        {
        //            i_last_added++;
        //            mapHeat[checking.down.x][checking.down.y] = 1.f;
        //            toCheck[i_last_added] = (checking.down);
        //        }
        //        else
        //            //if there's no obstacle:
        //            if (mapTerrainMod[checking.down.x][checking.down.y] != -1)
        //            {
        //                i_last_added++;
        //                //TODO: ADD CONSIDERING MAP DAMAGE
        //                //set value, according to terrain speed modification and damage map
        //                mapHeat[checking.down.x][checking.down.y] = previous + (1 / mapTerrainMod[checking.down.x][checking.down.y]);
        //                //add this tile for further checking
        //                toCheck[i_last_added] = (checking.down);
        //            }
        //    }
        //}
        ////up-left
        //if (checking.upLeft.x!=-1) //if index valid
        //{
        //    //if value not set:
        //    if (mapHeat[checking.upLeft.x][checking.upLeft.y] == 0.f)
        //    {
        //        //if there's no fog of war:
        //        if (mapFogOfWar[checking.upLeft.x][checking.upLeft.y] == -1)
        //        {
        //            i_last_added++;
        //            mapHeat[checking.upLeft.x][checking.upLeft.y] = 1.f;
        //            toCheck[i_last_added] = (checking.upLeft);
        //        }
        //        else
        //            //if there's no obstacle:
        //            if (mapTerrainMod[checking.upLeft.x][checking.upLeft.y] != -1)
        //            {
        //                i_last_added++;
        //                //TODO: ADD CONSIDERING MAP DAMAGE
        //                //set value, according to terrain speed modification and damage map
        //                mapHeat[checking.upLeft.x][checking.upLeft.y] = previous + (1 / mapTerrainMod[checking.upLeft.x][checking.upLeft.y]);
        //                //add this tile for further checking
        //                toCheck[i_last_added] = (checking.upLeft);
        //            }
        //    }
        //}
        ////up-right
        //if (checking.upRight.x != -1) //if index valid
        //{
        //    //if value not set:
        //    if (mapHeat[checking.upRight.x][checking.upRight.y] == 0.f)
        //    {
        //        //if there's no fog of war:
        //        if (mapFogOfWar[checking.upRight.x][checking.upRight.y] == -1)
        //        {
        //            i_last_added++;
        //            mapHeat[checking.downLeft.x][checking.downLeft.y] = 1.f;
        //            toCheck[i_last_added] = (checking.downLeft);
        //        }
        //        else
        //            //if there's no obstacle:
        //            if (mapTerrainMod[checking.upRight.x][checking.upRight.y] != -1)
        //            {
        //                i_last_added++;
        //                //TODO: ADD CONSIDERING MAP DAMAGE
        //                //set value, according to terrain speed modification and damage map
        //                mapHeat[checking.upRight.x][checking.upRight.y] = previous + (1 / mapTerrainMod[checking.upRight.x][checking.upRight.y]);
        //                //add this tile for further checking
        //                toCheck[i_last_added] = (checking.upRight);
        //            }
        //    }
        //}
        ////down-left
        //if (checking.downLeft.x != -1) //if index valid
        //{
        //    //if value not set:
        //    if (mapHeat[checking.downLeft.x][checking.downLeft.y] == 0.f)
        //    {
        //        //if there's no fog of war:
        //        if (mapFogOfWar[checking.downLeft.x][checking.downLeft.y] == -1)
        //        {
        //            i_last_added++;
        //            mapHeat[checking.downLeft.x][checking.downLeft.y] = 1.f;
        //            toCheck[i_last_added] = (checking.downLeft);
        //        }
        //        else
        //            //if there's no obstacle:
        //            if (mapTerrainMod[checking.downLeft.x][checking.downLeft.y] != -1)
        //            {
        //                i_last_added++;
        //                //TODO: ADD CONSIDERING MAP DAMAGE
        //                //set value, according to terrain speed modification and damage map
        //                mapHeat[checking.downLeft.x][checking.downLeft.y] = previous + (1 / mapTerrainMod[checking.downLeft.x][checking.downLeft.y]);
        //                //add this tile for further checking
        //                toCheck[i_last_added] = (checking.downLeft);
        //            }
        //    }
        //}
        ////down-right
        //if (checking.downRight.x != -1) //if index valid
        //{
        //    //if value not set:
        //    if (mapHeat[checking.downRight.x][checking.downRight.y] == 0.f)
        //    {
        //        //if there's no fog of war:
        //        if (mapFogOfWar[checking.downRight.x][checking.downRight.y] == -1)
        //        {
        //            i_last_added++;
        //            mapHeat[checking.downRight.x][checking.downRight.y] = 1.f;
        //            toCheck[i_last_added] = (checking.downRight);
        //        }
        //        else
        //            //if there's no obstacle:
        //            if (mapTerrainMod[checking.downRight.x][checking.downRight.y] != -1)
        //            {
        //                i_last_added++;
        //                //TODO: ADD CONSIDERING MAP DAMAGE
        //                //set value, according to terrain speed modification and damage map
        //                mapHeat[checking.downRight.x][checking.downRight.y] = previous + (1 / mapTerrainMod[checking.downRight.x][checking.downRight.y]);
        //                //add this tile for further checking
        //                toCheck[i_last_added] = (checking.downRight);
        //            }
        //    }
        //}
    }

    delete[] toCheck;

    //VECTOR MAP CALCULATION

    float x_buf;
    float y_buf;
    float minimumHeat;
    Vector2 buf;
    NeighborsIndex neighbors;
    TileIndex upLeft, rightDown, indexMinimum;

//#pragma omp parallel for private(upLeft, rightDown, indexMinimum, minimumHeat, buf)
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            /*upLeft = {x-2, y-2};
            rightDown = { x + 2, y + 2 };
            if (upLeft.x < 0)
                upLeft.x = 0;
            if (upLeft.y < 0)
                upLeft.y = 0;
            if (rightDown.x > mapWidth-1)
                rightDown.x = mapWidth-1;
            if (rightDown.y > mapHeight-1)
                rightDown.y = mapHeight-1;

            minimumHeat = mapHeat[x][y];
            indexMinimum = { x,y };

            for (int x_checking = upLeft.x; x_checking <= rightDown.x; x_checking++)
            {
                for (int y_checking = upLeft.y; y_checking <= rightDown.y; y_checking++)
                {
                    if (mapTerrainMod[x_checking][y_checking] != -1.f && x_checking != x && y_checking != y)
                    {
                        if (mapHeat[x_checking][y_checking] <= minimumHeat)
                        {
                            minimumHeat = mapHeat[x_checking][y_checking];
                            indexMinimum = { x_checking, y_checking };
                        }
                    }
                }
            }
            buf = { static_cast<float>(indexMinimum.x - x), static_cast<float>(indexMinimum.y - y) };
            if(buf.x!=0 || buf.y!= 0)
                mapVector[x][y] = { buf.x / fabsf(max(buf.x, buf.y)), buf.y / fabsf(max(buf.x, buf.y)) };*/

            neighbors = neighborsIndices[x][y];
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
                if (mapTerrainMod[x - 1][y - 1] != -1.f && (mapTerrainMod[x][y - 1] != -1.f || mapTerrainMod[x - 1][y] != -1.f)) //check for obstruction
                    if (mapHeat[x - 1][y - 1] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x - 1][y - 1];
                        mapVector[x][y] = { -1.f, -1.f };
                    }
            }
            //upRight
            if (neighbors.upRight.x != -1) //check for map borders
            {
                if (mapTerrainMod[x + 1][y - 1] != -1.f && (mapTerrainMod[x][y - 1] != -1.f || mapTerrainMod[x + 1][y] != -1.f)) //check for obstruction
                    if (mapHeat[x + 1][y - 1] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x + 1][y - 1];
                        mapVector[x][y] = { 1.f, -1.f };
                    }
            }
            //downLeft
            if (neighbors.downLeft.x != -1) //check for map borders
            {
                if (mapTerrainMod[x - 1][y + 1] != -1.f && (mapTerrainMod[x][y + 1] != -1.f || mapTerrainMod[x - 1][y] != -1.f)) //check for obstruction
                    if (mapHeat[x - 1][y + 1] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x - 1][y + 1];
                        mapVector[x][y] = { -1.f, 1.f };
                    }
            }
            //downRight
            if (neighbors.downRight.x != -1) //check for map borders
            {
                if (mapTerrainMod[x + 1][y + 1] != -1.f && (mapTerrainMod[x][y + 1] != -1.f || mapTerrainMod[x + 1][y] != -1.f)) //check for obstruction
                    if (mapHeat[x + 1][y + 1] < minimumHeat)
                    {
                        minimumHeat = mapHeat[x + 1][y + 1];
                        mapVector[x][y] = { 1.f, 1.f };
                    }
            }
        }
    }
}

void GameData::addActor(ActorType type, Vector2 position, State state)
{
    Building* buf_building;
    Militaty* buf_military;
    Turret* buf_turret;

    switch (wantToBuild)
    {
    case ActorType::LIGHT_TURRET:
        break;
    case ActorType::HEAVY_TURRET:
        buf_turret = new Turret(
            this,
            ActorType::HEAVY_TURRET,
            position,
            State::UNDER_CONSTRUCTION);

        unitsList.push_back(buf_turret);
        militaryUnitsList.push_back(buf_turret);
        turretUnitsList.push_back(buf_turret);
        break;
    case ActorType::AIRDEFENSE_TURRET:
        break;
    case ActorType::LIGHT_INSECT:
        buf_military = new Insect(
            this,
            ActorType::LIGHT_INSECT,
            position,
            State::GOES);

        unitsList.push_back(buf_military);
        militaryUnitsList.push_back(buf_military);
        break;
    case ActorType::HEAVY_INSECT:
        break;
    case ActorType::FLYING_INSECT:
        break;
    case ActorType::CORE:
        
        buf_building = new Core(
            this,
            ActorType::CORE,
            position,
            State::UNDER_CONSTRUCTION);
        unitsList.push_back(buf_building);
        expansionUnitsList_Machines.push_back(buf_building);

        break;
    case ActorType::BASE:
        buf_building = new Base(
            this,
            ActorType::BASE,
            position,
            State::ONLINE);
        unitsList.push_back(buf_building);
        expansionUnitsList_Machines.push_back(buf_building);

        break;
    case ActorType::HIVE:
        break;
    case ActorType::TUMOR:
        buf_building = new Tumor(
            this,
            ActorType::TUMOR,
            position,
            State::UNDER_CONSTRUCTION);
        unitsList.push_back(buf_building);
        expansionUnitsList_Insects.push_back(buf_building);

        break;
    default:
        break;
    }

    //in any case nulify wantToBuild
    wantToBuild = ActorType::ACTOR_NULL;
    
}
GameActor* GameData::getActorInTile(int x, int y)
{
    GameActor* result = nullptr;
    Vector2 pos;
    for (GameActor* actor : unitsList)
    {
        if (actor != nullptr)
        {
            pos = actor->getPosition();
            Vector2 test = { pos.x / pixelsPerTile, pos.y / pixelsPerTile };
            if (static_cast<int>(pos.x / pixelsPerTile) == x && static_cast<int>(pos.y / pixelsPerTile) == y)
            {
                result = actor;
            }
        }
    }
    return result;
}
std::vector<GameActor*> GameData::getActorsInRadius(Vector2 center, float radius)
{
    std::vector<GameActor*> result;

    for (GameActor* actor: unitsList)
    {
        if (actor != nullptr)
        {
            if (CheckCollisionPointCircle(actor->getPosition(), center, radius))
            {
                result.push_back(actor);
            }
        }
    }

    return result;
}
GameActor* GameData::getNearestSpecificActor(Vector2 position, std::vector<GameActor*> actors, GameActor* caller, ActorType type)
{
    GameActor* result = nullptr;
    double minDistance;
    double buf;
    //TODO: if type==ActorType::ACTOR_NULL, do search"by default"
    for (GameActor* actor : actors)
    {
        if ((actor->type == type || type == ActorType::ACTOR_NULL) && actor!=caller)
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
    for (iter = unitsList.begin(); iter != unitsList.end(); iter++)
    {
        if ((*iter) != nullptr)
        {
            if ((*iter)->ID == ID)
            {
                buf = *iter;
                //iter = unitsList.erase(iter);
                (*iter) = nullptr;
                break;
            }
        }
    }

    //seek in insects expansion units
    std::vector<Building*>::iterator expandIter;
    for (expandIter = expansionUnitsList_Insects.begin(); expandIter != expansionUnitsList_Insects.end(); )
    {
        if ((*expandIter)->ID == ID)
            expandIter = expansionUnitsList_Insects.erase(expandIter);
        else
            expandIter++;
    }

    //seek in machines expansion units
    for (expandIter = expansionUnitsList_Machines.begin(); expandIter != expansionUnitsList_Machines.end(); )
    {
        if ((*expandIter)->ID == ID)
            expandIter = expansionUnitsList_Machines.erase(expandIter);
        else
            expandIter++;
    }

    std::vector<Militaty*>::iterator militaryIter;
    for (militaryIter = militaryUnitsList.begin(); militaryIter != militaryUnitsList.end(); )
    {
        if ((*militaryIter)->ID == ID)
            militaryIter = militaryUnitsList.erase(militaryIter);
        else
            militaryIter++;
    }
    
    std::vector<Turret*>::iterator turretIter;
    for (turretIter = turretUnitsList.begin(); turretIter != turretUnitsList.end(); )
    {
        if ((*turretIter)->ID == ID)
            turretIter = turretUnitsList.erase(turretIter);
        else
            turretIter++;
    }

    if(buf!=nullptr)
        delete buf;
}
void GameData::Hit(GameActor* target, int damage, ActorType hitBy)
{
    bool result = false;
    damage -= target->armor;
    target->setHP(target->getHP() - damage);
    target->inBattle = true;
    target->inBattleCounter = 60;

    if (target->getHP() <= 0)
    {
        Side opposite;
        if (target->side == Side::INSECTS)
            opposite = Side::MACHINES;
        else
            opposite = Side::INSECTS;

        removeActor(target->ID);
        recalculateMilitaryTargets(opposite);
        result = true;
    }
    //TODO: update damage maps here
}

void GameData::revealTerritory(TileIndex position, int radius, Side side)
{
    //revealed tiles numered by distance between unit and tile

    int** matrix = mapsFogOfWar[side];
    int buf;

    //check tile, is it need to reveal for again
    if (static_cast<int>(matrix[position.x][position.y]) < radius) //WTF?!
    {
        matrix[position.x][position.y] = radius;
        //TODO: reverse radius to descending, remake radius using 
        for (int r = 1; r <= radius; r++)
        {
            buf = radius - r;
            for (int x = position.x + 1; x < position.x + r; x++)
            {
                for (int y = position.y; y > position.y - r; y--) //REVERSE int y = position.y - r; y < position.y + r; y++
                {
                    if (CheckCollisionPointCircle(Vector2{ static_cast<float>(x), static_cast<float>(y) }, position, r))
                    {
                        //adding this and opposite tiles
                        if (isTileInBounds({ x, y }))
                            if(matrix[x][y]<buf)
                                matrix[x][y] = buf; // up-right
                        if (isTileInBounds({ position.x + (position.x - x), y }))
                            if (matrix[position.x + (position.x - x)][y] < buf)
                                matrix[position.x + (position.x - x)][y] = buf; //up-left
                        if (isTileInBounds({ x, position.y + (position.y - y) }))
                            if (matrix[x][position.y + (position.y - y)] < buf)
                                matrix[x][position.y + (position.y - y)] = buf; //down-right
                        if (isTileInBounds({ position.x + (position.x - x), position.y + (position.y - y) }))
                            if (matrix[position.x + (position.x - x)][position.y + (position.y - y)] < buf)
                                matrix[position.x + (position.x - x)][position.y + (position.y - y)] = buf; //down-left
                    }
                }
            }

            if (isTileInBounds({ position.x, position.y - r }))
                if (matrix[position.x][position.y - r] < buf)
                    matrix[position.x][position.y - r] = buf; //up
            if (isTileInBounds({ position.x, position.y + r }))
                if (matrix[position.x][position.y + r] < buf)
                    matrix[position.x][position.y + r] = buf; //down
            if (isTileInBounds({ position.x + r, position.y }))
                if (matrix[position.x + r][position.y] < buf)
                    matrix[position.x + r][position.y] = buf; //right
            if (isTileInBounds({ position.x - r, position.y }))
                if (matrix[position.x - r][position.y] < buf)
                    matrix[position.x - r][position.y] = buf;//left
        }

        //TODO: optimization - make vector field recalculation only when revealed tile count above zero (increase count, when matrix value changed FROM -1)
        if (side == Side::INSECTS)
        {
            calculateVectorPathfinding(
                insectsDesirePosition,
                ActorType::LIGHT_INSECT
            );
            //calculateVectorPathfinding(
            //    insectsDesirePosition,
            //    ActorType::HEAVY_INSECT
            //);
            //calculateVectorPathfinding(
            //    insectsDesirePosition,
            //    ActorType::FLYING_INSECT
            //);
        }
    }
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

int GameData::trySpendResources(int amount, Side side)
{
    int spended;

    if (side == Side::INSECTS)
    {
        if (static_cast<int>(resourcesInsects) - amount < 0)
        {
            spended = resourcesInsects;
        }
        else
        {
            spended = amount;
        }
    }
    if(side == Side::MACHINES)
    {
        if (static_cast<int>(resourcesMachines) - amount < 0)
        {
            spended = resourcesMachines;
        }
        else
        {
            spended = amount;
        }
    }

    return spended;
}
void GameData::spendResources(int amount, Side side)
{
    if (side == Side::INSECTS)
    {
        resourcesInsects -= amount;
    }
    if (side == Side::MACHINES)
    {
        resourcesMachines -= amount;
    }
}

void GameData::GameUpdate()
{
    //TIME COUNTER
    timeCount++;
    if (timeCount > 60)
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

    if (IsKeyPressed(KEY_TWO))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::HEAVY_TURRET;

    if (IsKeyPressed(KEY_FOUR))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::LIGHT_INSECT;

    if (IsKeyPressed(KEY_E))
        if(wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::TUMOR;
        else
            wantToBuild = ActorType::ACTOR_NULL;

    if (IsKeyPressed(KEY_Q))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::CORE;
        else
            wantToBuild = ActorType::ACTOR_NULL;

    if (IsKeyPressed(KEY_B))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::BASE;
        else
            wantToBuild = ActorType::ACTOR_NULL;

    if (IsKeyPressed(KEY_C))
        clearMap();

    //clear all fog of war by current vision side
    if (IsKeyPressed(KEY_V))
    {
        int** fogOfWar = mapsFogOfWar[visionSide];

        for (int x = 0; x < mapWidth; x++)
            for (int y = 0; y < mapHeight; y++)
                fogOfWar[x][y] = 100;

        calculateVectorPathfinding(insectsDesirePosition, ActorType::LIGHT_INSECT);
        calculateVectorPathfinding(insectsDesirePosition, ActorType::HEAVY_INSECT);
        calculateVectorPathfinding(insectsDesirePosition, ActorType::FLYING_INSECT);
    }

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
        insectsDesirePosition = mouseIndex;
        calculateVectorPathfinding(
            insectsDesirePosition,
            ActorType::LIGHT_INSECT
        );
        //calculateVectorPathfinding(
        //    insectsDesirePosition,
        //    ActorType::HEAVY_INSECT
        //);
        //calculateVectorPathfinding(
        //    insectsDesirePosition,
        //    ActorType::FLYING_INSECT
        //);
    }

    if (IsKeyPressed(KEY_F1))
        if (visionSide == Side::INSECTS)
            visionSide = Side::MACHINES;
        else
            visionSide = Side::INSECTS;

    if (IsKeyPressed(KEY_F2))
        if (showingCreepStates)
            showingCreepStates = false;
        else
            showingCreepStates = true;

    if (IsKeyPressed(KEY_SPACE))
        if (gamePaused)
            gamePaused = false;
        else
            gamePaused = true;

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && wantToBuild != ActorType::ACTOR_NULL)
    {
        //right-click to clear
        wantToBuild = ActorType::ACTOR_NULL;
    }

    //ACTORS BUILDING
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && wantToBuild != ActorType::ACTOR_NULL)
    {
        Vector2 position = { mouseIndex.x * pixelsPerTile + pixelsPerTile / 2, mouseIndex.y * pixelsPerTile + pixelsPerTile / 2 };
        addActor(wantToBuild, position, State::ONLINE);
    }

    if (!gamePaused)
    {
        //UNIT UPDATING
        std::vector<GameActor*>::iterator iter;
        GameActor* buf = nullptr;
        for (iter = unitsList.begin(); iter != unitsList.end(); )
        {
            //removing pointers for killed units from vector
            if ((*iter) == nullptr)
            {
                iter = unitsList.erase(iter);
            }
            else
            {
                (*iter)->Update();
                iter++;
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && wantToRemove)
        {
            GameActor* buf = getActorInTile(mouseIndex.x, mouseIndex.y);
            if (buf != nullptr)
                removeActor(buf->ID);
            wantToRemove = false;
        }

        //expansion fading
        if (timeCount == 0)
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

            creepTilesCount -= buf.size();
            for (TileIndex tile : buf)
                mapExpansionCreep[tile.x][tile.y] = ExpandState::AVAILABLE;

            //MACHINES
            buf.clear();
            for (int x = 0; x < mapWidth; x++)
            {
                for (int y = 0; y < mapHeight; y++)
                {
                    if (mapExpansionEnergised[x][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                    {
                        if (numOfExpansionTileAdjoinFading(x, y, Side::MACHINES) < 4)
                            buf.push_back(TileIndex{ x,y });
                    }
                }
            }

            energisedTilesCount -= buf.size();

            for (TileIndex tile : buf)
                mapExpansionEnergised[tile.x][tile.y] = ExpandState::AVAILABLE;

        }

        //RESOURCES GATHERING
        if (timeCount == 0)
        {
            resourcesInsects += creepTilesCount / 100;
            resourcesMachines += energisedTilesCount / 100;
            if (resourcesMachines > 200) resourcesMachines = 200; //maximum of Energy
        }

        //ANIMATION
        if (timeCount % 15 == 0)
        {
            currentFrame++;

            if (currentFrame >= expansionMachinesAnimation.framesAmount)
                currentFrame = 0;
        }
    }
}

void GameData::GameDraw()
{
    BeginMode2D(camera);

#ifndef TILE_DRAWING
    DrawTextureEx(
        terrainTexture,
        Vector2{ 0.f, 0.f },
        0.f, pixelsPerTile, WHITE);
#else

    if (camera.zoom < 1.f /*&& !IsKeyDown(KEY_T)*/) //low-detailed map
    {
        DrawTextureEx(
            terrainTexture,
            Vector2{ 0.f, 0.f },
            0.f, pixelsPerTile, WHITE);
    }
    else
    {   //full-detailed map
        
        //black background for fog of war
        DrawRectangle(0, 0, mapSize.x, mapSize.y, BLACK);

        Vector2** vectorField = vectorFields[ActorType::LIGHT_INSECT];
        float** terrainMod = mapsPathfinding[ActorType::LIGHT_INSECT]["mapsTerrainMod"];
        int** fogOfWar = mapsFogOfWar[visionSide];

        int index;
        for (int x = renderBorders[1]; x < renderBorders[3]; x++) //columns
        {
            for (int y = renderBorders[0]; y < renderBorders[2]; y++) //rows
            {
                if (fogOfWar[x][y] != -1)
                {
                    index = mapWidth * y + x;

                    //if there is creep:
                    if (mapExpansionCreep[x][y] == ExpandState::EXPANDED || mapExpansionCreep[x][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                    {
                        float x_exp = static_cast<float>(x % (expansionInsectsTexture.width / static_cast<int>(pixelsPerTile)));
                        float y_exp = static_cast<float>(y % (expansionInsectsTexture.height / static_cast<int>(pixelsPerTile)));
                        Rectangle buf = {
                            x_exp * pixelsPerTile,
                            y_exp * pixelsPerTile,
                            pixelsPerTile,
                            pixelsPerTile };

                        DrawTextureRec(
                            expansionInsectsTexture,
                            buf,
                            Vector2{ x * pixelsPerTile, y * pixelsPerTile, },
                            WHITE);
                    }
                    else
                    {
                        //if there is energized tile:
                        if (mapExpansionEnergised[x][y] == ExpandState::EXPANDED || mapExpansionEnergised[x][y] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                        {
                            float x_exp = static_cast<float>(x % (expansionMachinesAnimation.width / static_cast<int>(pixelsPerTile)));
                            float y_exp = static_cast<float>(y % (expansionMachinesAnimation.height / static_cast<int>(pixelsPerTile)));
                            Rectangle buf = {
                                x_exp * pixelsPerTile,
                                y_exp * pixelsPerTile,
                                pixelsPerTile,
                                pixelsPerTile };

                            DrawTextureRec(
                                expansionMachinesAnimation.frames[currentFrame],
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
                    }
                }

                //TODO: remove
                if (showingCreepStates)
                    DrawText(FormatText("%d", mapExpansionCreep[x][y]), x * pixelsPerTile + pixelsPerTile / 3, y * pixelsPerTile + pixelsPerTile / 3, 14, RED);
            
                //heatmap, light insects
                if(IsKeyDown(KEY_F4))
                    DrawText(FormatText("%.0f", mapsPathfinding[ActorType::LIGHT_INSECT]["mapsHeat"][x][y]), x * pixelsPerTile + pixelsPerTile / 3, y * pixelsPerTile + pixelsPerTile / 3, 10, SKYBLUE);

                //vector field, light insects
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

                //fog of war, INSECTS side
                if(IsKeyDown(KEY_F6))
                    DrawText(FormatText("%d", static_cast<int>(fogOfWar[x][y])), x * pixelsPerTile + pixelsPerTile / 3, y * pixelsPerTile + pixelsPerTile / 3, 10, SKYBLUE);
            }
        }
    }

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
        case ActorType::BASE:
            //draw circle of connection range
        case ActorType::HIVE:
        case ActorType::TUMOR:
            //draw blue circle of expansion
            radius = buildingsAttributes[wantToBuild]["expansionRange"];
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
        if(actor!=nullptr) //dont need this?
            actor->Draw();
    }

    EndMode2D();
    DrawText(FormatText("Camera zoom: %.2f", camera.zoom), 20, 20, 20, RED);
    DrawText(FormatText("Tiles rendering: %d", (renderBorders[2] - renderBorders[0])* (renderBorders[3] - renderBorders[1])), 20, 80, 20, RED);
    DrawText(FormatText("Mouse tile index: %d, %d", mouseIndex.x, mouseIndex.y), 20, 140, 20, RED);
    DrawText(FormatText("Desire position: %d, %d", insectsDesirePosition.x, insectsDesirePosition.y), 20, 200, 20, RED);
    if(visionSide==Side::INSECTS)
        DrawText("Vision side: INSECTS", 20, 260, 20, RED);
    else
        DrawText("Vision side: MACHINES", 20, 260, 20, RED);

    DrawText(TextFormat("Creep: %d, food: %d", creepTilesCount, resourcesInsects), 20, 320, 20, RED);
    DrawText(TextFormat("Zerolayer: %d, energy: %d", energisedTilesCount, resourcesMachines), 20, 380, 20, RED);

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

        //fog of war
        Side sides[] = { Side::INSECTS, Side::MACHINES };
        for (Side side : sides)
        {
            for (int x = 0; x < mapWidth; x++)
                delete[] mapsFogOfWar[side][x];
            delete[] mapsFogOfWar[side];
        }
    }

    UnloadTexture(expansionInsectsTexture);

    for (int i = 0; i < 7; i++)
    {
        UnloadTexture(tilesetTex[i]);
    }

    delete[] palette;
}