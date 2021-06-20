#include "GameClasses.h"

#define TILE_DRAWING

GameData::GameData()
{
    screenSize = Vector2{ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };

    camera.target = { screenSize.x / 2, screenSize.y / 2 };
    camera.offset = { screenSize.x / 2, screenSize.y / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    palette = new Color[]{ DARKBLUE, BLUE, YELLOW, GREEN, DARKGREEN, GRAY, DARKGRAY };

    //TILESET TEXTURES

    const char* filenames[] = { 
        "LAKE",
        "SWAMP",
        "SAND",
        "PLAIN",
        "TREE",
        "STONE",
        "MOUNTAIN",
    };

    for (int i = 0; i < 7; i++)
    {
        tilesetTex[i] = LoadTexture(TextFormat("textures\\tileset\\%s.png", filenames[i]));
    }

    //EXPANSION TEXTURES
    expansionInsectsTexture = LoadTexture("textures\\source\\creep_1.png");
    expansionMachinesAnimation = {
        new Texture2D[15],
        //TODO: return to 15
        1
    };

    for (int i = 0; i < expansionMachinesAnimation.framesAmount; i++)
    {
        expansionMachinesAnimation.frames[i] = LoadTexture(TextFormat("textures\\tileset\\machines_expansion_frames\\%d.png", i));
    }

    expansionMachinesAnimation.width = expansionMachinesAnimation.frames[0].width;
    expansionMachinesAnimation.height = expansionMachinesAnimation.frames[0].height;

    //UNIT SPRITES

    //HEAVY INSECT
    unitAnimations[ActorType::HEAVY_INSECT][State::GOES] =
    {
        new Texture2D[1],
        1
    };
    for (int i = 0; i < 1; i++)
    {
        unitAnimations[ActorType::HEAVY_INSECT][State::GOES].frames[i] = LoadTexture(TextFormat("textures\\sprites\\heavyInsect_goes_%d.png", i));
    }

    //FLYING INSECT
    unitAnimations[ActorType::FLYING_INSECT][State::GOES] =
    {
        new Texture2D[1],
        1
    };
    for (int i = 0; i < 1; i++)
    {
        unitAnimations[ActorType::FLYING_INSECT][State::GOES].frames[i] = LoadTexture(TextFormat("textures\\sprites\\flyingInsect_goes_%d.png", i));
    }

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

    //LIGHT TURRET
    unitAnimations[ActorType::LIGHT_TURRET][State::ONLINE] =
    {
        new Texture2D[1],
        1
    };
    for (int i = 0; i < 1; i++)
    {
        unitAnimations[ActorType::LIGHT_TURRET][State::ONLINE].frames[i] = LoadTexture(TextFormat("textures\\sprites\\lightTurret_online_%d.png", i));
    }

    unitAnimations[ActorType::LIGHT_TURRET][State::ATTACKING] =
    {
        new Texture2D[1],
        1
    };
    for (int i = 0; i < 1; i++)
    {
        unitAnimations[ActorType::LIGHT_TURRET][State::ATTACKING].frames[i] = LoadTexture(TextFormat("textures\\sprites\\lightTurret_attacking_%d.png", i));
    }

    //AIRDEFENSE TURRET
    unitAnimations[ActorType::AIRDEFENSE_TURRET][State::ONLINE] =
    {
        new Texture2D[1],
        1
    };
    for (int i = 0; i < 1; i++)
    {
        unitAnimations[ActorType::AIRDEFENSE_TURRET][State::ONLINE].frames[i] = LoadTexture(TextFormat("textures\\sprites\\antiAirTurret_online_%d.png", i));
    }

    unitAnimations[ActorType::AIRDEFENSE_TURRET][State::ATTACKING] =
    {
        new Texture2D[1],
        1
    };
    for (int i = 0; i < 1; i++)
    {
        unitAnimations[ActorType::AIRDEFENSE_TURRET][State::ATTACKING].frames[i] = LoadTexture(TextFormat("textures\\sprites\\antiAirTurret_attacking_%d.png", i));
    }

    //GENERIC ACTORS ATTRIBUTES
    genericAttributes[ActorType::TUMOR] = std::map<std::string, int>{
        {"maxHP", 100},
        {"size", 16},
        {"cost", 50},
        {"sightRange", 10},
        {"armor", 5},
    };
    genericAttributes[ActorType::HIVE] = std::map<std::string, int>{
        {"maxHP", 500},
        {"size", 32},
        {"cost", 1000},
        {"sightRange", 12},
        {"armor", 10},
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
    genericAttributes[ActorType::LIGHT_TURRET] = std::map<std::string, int>{
        {"maxHP", 100},
        {"size", 12},
        {"cost", 50},
        {"sightRange", 12},
        {"armor", 5},
    };
    genericAttributes[ActorType::HEAVY_TURRET] = std::map<std::string, int>{
        {"maxHP", 200},
        {"size", 16},
        {"cost", 50},
        {"sightRange", 12},
        {"armor", 5},
    };
    genericAttributes[ActorType::AIRDEFENSE_TURRET] = std::map<std::string, int>{
        {"maxHP", 150},
        {"size", 12},
        {"cost", 50},
        {"sightRange", 12},
        {"armor", 5},
    };
    genericAttributes[ActorType::LIGHT_INSECT] = std::map<std::string, int>{
        {"maxHP", 30},
        {"size", 4},
        {"cost", 10},
        {"sightRange", 4},
        {"armor", 1},
    };
    genericAttributes[ActorType::HEAVY_INSECT] = std::map<std::string, int>{
        {"maxHP", 100},
        {"size", 8},
        {"cost", 30},
        {"sightRange", 3},
        {"armor", 10},
    };
    genericAttributes[ActorType::FLYING_INSECT] = std::map<std::string, int>{
        {"maxHP", 70},
        {"size", 6},
        {"cost", 20},
        {"sightRange", 5},
        {"armor", 2},
    };

    //BUILDING ACTORS ATTRIBUTES
    buildingsAttributes[ActorType::TUMOR] = std::map<std::string, int>{
        {"expansionRange", 8},
        {"expansionTime", 12 },
    };
    buildingsAttributes[ActorType::HIVE] = std::map<std::string, int>{
        {"expansionRange", 8},
        {"expansionTime", 8 },
    };
    buildingsAttributes[ActorType::CORE] = std::map<std::string, int>{
        {"expansionRange", 8},
        {"expansionTime", 4 },
    };
    buildingsAttributes[ActorType::BASE] = std::map<std::string, int>{
        {"expansionRange", 12},
        {"expansionTime", 4 },
    };

    //MILITARY ACTORS ATTRIBUTES
    militaryAttributes[ActorType::LIGHT_INSECT] = std::map<std::string, int>{
        {"seekRange", 8 * pixelsPerTile},
        {"attackRange", 2 * pixelsPerTile},
        {"speed", 2},
        {"damage", 5},
        {"rotationSpeed", 8},
        {"cooldownDuration", 30}, //ticks to reload
    };
    militaryAttributes[ActorType::HEAVY_INSECT] = std::map<std::string, int>{
        {"seekRange", 8 * pixelsPerTile},
        {"attackRange", 2 * pixelsPerTile},
        {"speed", 1},
        {"damage", 30},
        {"rotationSpeed", 4},
        {"cooldownDuration", 60}, //ticks to reload
    };
    militaryAttributes[ActorType::FLYING_INSECT] = std::map<std::string, int>{
        {"seekRange", 8 * pixelsPerTile},
        {"attackRange", 2 * pixelsPerTile},
        {"speed", 2},
        {"damage", 15},
        {"rotationSpeed", 8},
        {"cooldownDuration", 45}, //ticks to reload
    };
    militaryAttributes[ActorType::LIGHT_TURRET] = std::map<std::string, int>{
        {"seekRange", 10 * pixelsPerTile},
        {"attackRange", 4 * pixelsPerTile},
        {"speed", 1},
        {"damage", 15},
        {"rotationSpeed", 6},
        {"cooldownDuration", 15}, //ticks to reload
    };
    militaryAttributes[ActorType::HEAVY_TURRET] = std::map<std::string, int>{
        {"seekRange", 12 * pixelsPerTile},
        {"attackRange", 10 * pixelsPerTile},
        {"speed", 1},
        {"damage", 70},
        {"rotationSpeed", 2},
        {"cooldownDuration", 60}, //ticks to reload
    };
    militaryAttributes[ActorType::AIRDEFENSE_TURRET] = std::map<std::string, int>{
        {"seekRange", 16 * pixelsPerTile},
        {"attackRange", 12 * pixelsPerTile},
        {"speed", 1},
        {"damage", 40},
        {"rotationSpeed", 4},
        {"cooldownDuration", 30}, //ticks to reload
    };

    //CONNECTABLE ACTORS ATTRIBUTES
    connectableAttributes[ActorType::CORE] = std::map<std::string, int>
    {
        {"connectRange", 10 * pixelsPerTile},
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
        {"buildRange", 8 * pixelsPerTile},
    };
    constructorsAttributes[ActorType::BASE] = std::map<std::string, int>
    {
        {"buildPower", 20},
        {"buildRange", 8 * pixelsPerTile},
    };

    //TURRETS ATTRIBUTES
    turretsAttributes[ActorType::LIGHT_TURRET] = std::map<std::string, int>
    {
        {"maxCharge", 70},
        {"chargeRate", 2},
        {"energyPerShot", 3},
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
        {"energyPerShot", 7},
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
    resourcesMachines = 1000;
    basePtr = nullptr;
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
        clearMap();

        UnloadTexture(terrainTexture);

        //expansion
        for (int x = 0; x < mapWidth; x++)
                delete[] mapExpansionCreep[x];
        delete[] mapExpansionCreep;

        //pathfinding
        ActorType types[] = { ActorType::LIGHT_INSECT, ActorType::HEAVY_INSECT, ActorType::FLYING_INSECT };
        std::vector<std::string> matrices = { "mapsHeat", "mapsTerrainMod"};

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

            //damage matrix
            for (int x = 0; x < mapWidth; x++)
                delete[] mapsDamage[type][x];
            delete[] mapsDamage[type];
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
    std::vector<std::string> matrices = { "mapsHeat", "mapsTerrainMod"};

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

        //damage map
        int** buf_matrix = new int* [mapWidth];
        for (int x = 0; x < mapWidth; x++)
            buf_matrix[x] = new int[mapHeight];
        //damage map initialisation
        for (int x = 0; x < mapWidth; x++)
            for (int y = 0; y < mapHeight; y++)
                buf_matrix[x][y] = 0;
        mapsDamage[type] = buf_matrix;
        
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
std::vector<TileIndex> GameData::tilesInPerimeterCircleOrdered(TileIndex center, unsigned int radius)
{
    std::vector<TileIndex> buf;
    int x_centre = center.x, y_centre = center.y;
    int x = radius, y = 0;

    // Printing the initial point on the axes 
    // after translation


    // When radius is zero only a single
    // point will be printed
    if (radius > 0)
    {
        /*buf.push_back(TileIndex{ x + x_centre, -y + y_centre });
        buf.push_back(TileIndex{ y + x_centre,  x + y_centre });
        buf.push_back(TileIndex{ -y + x_centre, x + y_centre });*/
        buf.push_back(TileIndex{ x + x_centre, y_centre });  //right
        buf.push_back(TileIndex{ x_centre, -x + y_centre }); //up
        buf.push_back(TileIndex{ x_centre, x + y_centre });  //down
        buf.push_back(TileIndex{ -x + x_centre,  y_centre });//left
    }

    // Initialising the value of P
    int P = 1 - radius;
    while (x > y)
    {
        y++;

        // Mid-point is inside or on the perimeter
        if (P <= 0)
            P = P + 2 * y + 1;
        // Mid-point is outside the perimeter
        else
        {
            x--;
            P = P + 2 * y - 2 * x + 1;
        }

        // All the perimeter points have already been printed
        if (x < y)
            break;

        // Printing the generated point and its reflection
        // in the other octants after translation
        buf.push_back(TileIndex{ x + x_centre, y + y_centre });
        buf.push_back(TileIndex{ -x + x_centre, y + y_centre });
        buf.push_back(TileIndex{ x + x_centre , -y + y_centre });
        buf.push_back(TileIndex{ -x + x_centre , -y + y_centre });

        // If the generated point is on the line x = y then 
        // the perimeter points have already been printed
        if (x != y)
        {
            buf.push_back(TileIndex{ y + x_centre , x + y_centre });
            buf.push_back(TileIndex{ -y + x_centre , x + y_centre });
            buf.push_back(TileIndex{ y + x_centre , -x + y_centre });
            buf.push_back(TileIndex{ -y + x_centre , -x + y_centre });
        }
    }

    std::vector<TileIndex> result;

    for (int i = 0; i < buf.size(); i++)
    {
        //checking to not cross the border, if all right - add TileIndex to result vector
        if (!(buf[i].x < 0 ||
            buf[i].x>this->mapWidth - 1 ||
            buf[i].y < 0 ||
            buf[i].y>this->mapHeight - 1))
        {
            result.push_back(buf[i]);
        }
        //TODO: consider using result.erase()
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
    result.resize(8);
    NeighborsIndex neighbors = neighborsIndices[x][y];

    //this tile
    //result[0] = TileIndex{ x,y };

    //left
    if (neighbors.left.x != -1)
        result[0] = neighbors.left;
    else
        result[0] = { -1,-1 };
    //up
    if (neighbors.up.x != -1)
        result[1] = neighbors.up;
    else
        result[1] = { -1,-1 };
    //right
    if (neighbors.right.x != -1)
        result[2] = neighbors.right;
    else
        result[2] = { -1,-1 };
    //down
    if (neighbors.down.x != -1)
        result[3] = neighbors.down;
    else
        result[3] = { -1,-1 };

    //upLeft
    if (neighbors.upLeft.x != -1)
        result[4] = neighbors.upLeft;
    else
        result[4] = { -1,-1 };
    //upRight
    if (neighbors.upRight.x != -1)
        result[5] = neighbors.upRight;
    else
        result[5] = { -1,-1 };
    //downLeft
    if (neighbors.downLeft.x != -1)
        result[6] = neighbors.downLeft;
    else
        result[6] = { -1,-1 };
    //downRight
    if (neighbors.downRight.x != -1)
        result[7] = neighbors.downRight;
    else
        result[7] = { -1,-1 };

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
    int** mapDamage = mapsDamage[actorType];

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

    float previous;
    int bufHeat;
    std::list<TileIndex> toCheck = { target };
    std::vector<TileIndex> neighborsVector;
    NeighborsIndex neighbors;

    std::list<TileIndex>::iterator iter;
    //iter = toCheck.erase(iter);
    for (iter = toCheck.begin(); iter != toCheck.end(); )
    {
        //neighborsVector = getNeighborsAsVector((*iter).x, (*iter).y);
        neighbors = getNeighbors((*iter).x, (*iter).y);
        previous = mapHeat[(*iter).x][(*iter).y];

        //left
        if (neighbors.left.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[neighbors.left.x][neighbors.left.y] == 0.f)
            {
                //if there's fog of war:
                if (mapFogOfWar[neighbors.left.x][neighbors.left.y] == -1)
                {
                    mapHeat[neighbors.left.x][neighbors.left.y] = 1.f;
                    toCheck.push_back(neighbors.left);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[neighbors.left.x][neighbors.left.y] != -1)
                    {
                        //set value, according to terrain speed modification and damage map
                        mapHeat[neighbors.left.x][neighbors.left.y] = previous + (1 / mapTerrainMod[neighbors.left.x][neighbors.left.y]) + mapDamage[neighbors.left.x][neighbors.left.y];
                        //add this neighbors.left for further checking
                        toCheck.push_back(neighbors.left);
                    }
                }
            }
            else
            {
                //if there's NO fog of war:
                if (mapFogOfWar[neighbors.left.x][neighbors.left.y] != -1)
                {
                    //if value set - check, if it greater 
                    bufHeat = previous + (1 / mapTerrainMod[neighbors.left.x][neighbors.left.y]) + mapDamage[neighbors.left.x][neighbors.left.y];
                    if (mapHeat[neighbors.left.x][neighbors.left.y] > bufHeat)
                    {
                        mapHeat[neighbors.left.x][neighbors.left.y] = bufHeat;
                        toCheck.push_back(neighbors.left);
                    }
                }
            }
        }
        //up
        if (neighbors.up.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[neighbors.up.x][neighbors.up.y] == 0.f)
            {
                //if there's fog of war:
                if (mapFogOfWar[neighbors.up.x][neighbors.up.y] == -1)
                {
                    mapHeat[neighbors.up.x][neighbors.up.y] = 1.f;
                    toCheck.push_back(neighbors.up);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[neighbors.up.x][neighbors.up.y] != -1)
                    {
                        //set value, according to terrain speed modification and damage map
                        mapHeat[neighbors.up.x][neighbors.up.y] = previous + (1 / mapTerrainMod[neighbors.up.x][neighbors.up.y]) + mapDamage[neighbors.up.x][neighbors.up.y];
                        //add this neighbors.up for further checking
                        toCheck.push_back(neighbors.up);
                    }
                }
            }
            else
            {
                //if there's NO fog of war:
                if (mapFogOfWar[neighbors.up.x][neighbors.up.y] != -1)
                {
                    //if value set - check, if it greater 
                    bufHeat = previous + (1 / mapTerrainMod[neighbors.up.x][neighbors.up.y]) + mapDamage[neighbors.up.x][neighbors.up.y];
                    if (mapHeat[neighbors.up.x][neighbors.up.y] > bufHeat)
                    {
                        mapHeat[neighbors.up.x][neighbors.up.y] = bufHeat;
                        toCheck.push_back(neighbors.up);
                    }
                }
            }
        }
        //right
        if (neighbors.right.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[neighbors.right.x][neighbors.right.y] == 0.f)
            {
                //if there's fog of war:
                if (mapFogOfWar[neighbors.right.x][neighbors.right.y] == -1)
                {
                    mapHeat[neighbors.right.x][neighbors.right.y] = 1.f;
                    toCheck.push_back(neighbors.right);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[neighbors.right.x][neighbors.right.y] != -1)
                    {
                        //set value, according to terrain speed modification and damage map
                        mapHeat[neighbors.right.x][neighbors.right.y] = previous + (1 / mapTerrainMod[neighbors.right.x][neighbors.right.y]) + mapDamage[neighbors.right.x][neighbors.right.y];
                        //add this neighbors.right for further checking
                        toCheck.push_back(neighbors.right);
                    }
                }
            }
            else
            {
                //if there's NO fog of war:
                if (mapFogOfWar[neighbors.right.x][neighbors.right.y] != -1)
                {
                    //if value set - check, if it greater 
                    bufHeat = previous + (1 / mapTerrainMod[neighbors.right.x][neighbors.right.y]) + mapDamage[neighbors.right.x][neighbors.right.y];
                    if (mapHeat[neighbors.right.x][neighbors.right.y] > bufHeat)
                    {
                        mapHeat[neighbors.right.x][neighbors.right.y] = bufHeat;
                        toCheck.push_back(neighbors.right);
                    }
                }
            }
        }
        //down
        if (neighbors.down.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[neighbors.down.x][neighbors.down.y] == 0.f)
            {
                //if there's fog of war:
                if (mapFogOfWar[neighbors.down.x][neighbors.down.y] == -1)
                {
                    mapHeat[neighbors.down.x][neighbors.down.y] = 1.f;
                    toCheck.push_back(neighbors.down);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[neighbors.down.x][neighbors.down.y] != -1)
                    {
                        //set value, according to terrain speed modification and damage map
                        mapHeat[neighbors.down.x][neighbors.down.y] = previous + (1 / mapTerrainMod[neighbors.down.x][neighbors.down.y]) + mapDamage[neighbors.down.x][neighbors.down.y];
                        //add this neighbors.down for further checking
                        toCheck.push_back(neighbors.down);
                    }
                }
            }
            else
            {
                //if there's NO fog of war:
                if (mapFogOfWar[neighbors.down.x][neighbors.down.y] != -1)
                {
                    //if value set - check, if it greater 
                    bufHeat = previous + (1 / mapTerrainMod[neighbors.down.x][neighbors.down.y]) + mapDamage[neighbors.down.x][neighbors.down.y];
                    if (mapHeat[neighbors.down.x][neighbors.down.y] > bufHeat)
                    {
                        mapHeat[neighbors.down.x][neighbors.down.y] = bufHeat;
                        toCheck.push_back(neighbors.down);
                    }
                }
            }
        }

        //upLeft
        if (neighbors.upLeft.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[neighbors.upLeft.x][neighbors.upLeft.y] == 0.f)
            {
                //if there's fog of war:
                if (mapFogOfWar[neighbors.upLeft.x][neighbors.upLeft.y] == -1)
                {
                    mapHeat[neighbors.upLeft.x][neighbors.upLeft.y] = 1.f;
                    toCheck.push_back(neighbors.upLeft);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[neighbors.upLeft.x][neighbors.upLeft.y] != -1 && (mapTerrainMod[neighbors.up.x][neighbors.up.y] != -1.f || mapTerrainMod[neighbors.left.x][neighbors.left.y] != -1.f))
                    {
                        //set value, according to terrain speed modification and damage map
                        mapHeat[neighbors.upLeft.x][neighbors.upLeft.y] = previous + (1 / mapTerrainMod[neighbors.upLeft.x][neighbors.upLeft.y]) + mapDamage[neighbors.upLeft.x][neighbors.upLeft.y];
                        //add this neighbors.upLeft for further checking
                        toCheck.push_back(neighbors.upLeft);
                    }
                }
            }
            else
            {
                //if there's NO fog of war:
                if (mapFogOfWar[neighbors.upLeft.x][neighbors.upLeft.y] != -1)
                {
                    //if value set - check, if it greater 
                    bufHeat = previous + (1 / mapTerrainMod[neighbors.upLeft.x][neighbors.upLeft.y]) + mapDamage[neighbors.upLeft.x][neighbors.upLeft.y];
                    if (mapHeat[neighbors.upLeft.x][neighbors.upLeft.y] > bufHeat)
                    {
                        mapHeat[neighbors.upLeft.x][neighbors.upLeft.y] = bufHeat;
                        toCheck.push_back(neighbors.upLeft);
                    }
                }
            }
        }
        //upRight
        if (neighbors.upRight.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[neighbors.upRight.x][neighbors.upRight.y] == 0.f)
            {
                //if there's fog of war:
                if (mapFogOfWar[neighbors.upRight.x][neighbors.upRight.y] == -1)
                {
                    mapHeat[neighbors.upRight.x][neighbors.upRight.y] = 1.f;
                    toCheck.push_back(neighbors.upRight);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[neighbors.upRight.x][neighbors.upRight.y] != -1 && (mapTerrainMod[neighbors.up.x][neighbors.up.y] != -1.f || mapTerrainMod[neighbors.right.x][neighbors.right.y] != -1.f))
                    {
                        //set value, according to terrain speed modification and damage map
                        mapHeat[neighbors.upRight.x][neighbors.upRight.y] = previous + (1 / mapTerrainMod[neighbors.upRight.x][neighbors.upRight.y]) + mapDamage[neighbors.upRight.x][neighbors.upRight.y];
                        //add this neighbors.upRight for further checking
                        toCheck.push_back(neighbors.upRight);
                    }
                }
            }
            else
            {
                //if there's NO fog of war:
                if (mapFogOfWar[neighbors.upRight.x][neighbors.upRight.y] != -1)
                {
                    //if value set - check, if it greater 
                    bufHeat = previous + (1 / mapTerrainMod[neighbors.upRight.x][neighbors.upRight.y]) + mapDamage[neighbors.upRight.x][neighbors.upRight.y];
                    if (mapHeat[neighbors.upRight.x][neighbors.upRight.y] > bufHeat)
                    {
                        mapHeat[neighbors.upRight.x][neighbors.upRight.y] = bufHeat;
                        toCheck.push_back(neighbors.upRight);
                    }
                }
            }
        }
        //downLeft
        if (neighbors.downLeft.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[neighbors.downLeft.x][neighbors.downLeft.y] == 0.f)
            {
                //if there's fog of war:
                if (mapFogOfWar[neighbors.downLeft.x][neighbors.downLeft.y] == -1)
                {
                    mapHeat[neighbors.downLeft.x][neighbors.downLeft.y] = 1.f;
                    toCheck.push_back(neighbors.downLeft);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[neighbors.downLeft.x][neighbors.downLeft.y] != -1 && (mapTerrainMod[neighbors.down.x][neighbors.down.y] != -1.f || mapTerrainMod[neighbors.left.x][neighbors.left.y] != -1.f))
                    {
                        //set value, according to terrain speed modification and damage map
                        mapHeat[neighbors.downLeft.x][neighbors.downLeft.y] = previous + (1 / mapTerrainMod[neighbors.downLeft.x][neighbors.downLeft.y]) + mapDamage[neighbors.downLeft.x][neighbors.downLeft.y];
                        //add this neighbors.downLeft for further checking
                        toCheck.push_back(neighbors.downLeft);
                    }
                }
            }
            else
            {
                //if there's NO fog of war:
                if (mapFogOfWar[neighbors.downLeft.x][neighbors.downLeft.y] != -1)
                {
                    //if value set - check, if it greater 
                    bufHeat = previous + (1 / mapTerrainMod[neighbors.downLeft.x][neighbors.downLeft.y]) + mapDamage[neighbors.downLeft.x][neighbors.downLeft.y];
                    if (mapHeat[neighbors.downLeft.x][neighbors.downLeft.y] > bufHeat)
                    {
                        mapHeat[neighbors.downLeft.x][neighbors.downLeft.y] = bufHeat;
                        toCheck.push_back(neighbors.downLeft);
                    }
                }
            }
        }
        //downRight
        if (neighbors.downRight.x != -1) //if index valid
        {
            //if value not set:
            if (mapHeat[neighbors.downRight.x][neighbors.downRight.y] == 0.f)
            {
                //if there's fog of war:
                if (mapFogOfWar[neighbors.downRight.x][neighbors.downRight.y] == -1)
                {
                    mapHeat[neighbors.downRight.x][neighbors.downRight.y] = 1.f;
                    toCheck.push_back(neighbors.downRight);
                }
                else
                {
                    //if there's no obstacle:
                    if (mapTerrainMod[neighbors.downRight.x][neighbors.downRight.y] != -1 && (mapTerrainMod[neighbors.down.x][neighbors.down.y] != -1.f || mapTerrainMod[neighbors.right.x][neighbors.right.y] != -1.f))
                    {
                        //set value, according to terrain speed modification and damage map
                        mapHeat[neighbors.downRight.x][neighbors.downRight.y] = previous + (1 / mapTerrainMod[neighbors.downRight.x][neighbors.downRight.y]) + mapDamage[neighbors.downRight.x][neighbors.downRight.y];
                        //add this neighbors.downRight for further checking
                        toCheck.push_back(neighbors.downRight);
                    }
                }
            }
            else
            {
                //if there's NO fog of war:
                if (mapFogOfWar[neighbors.downRight.x][neighbors.downRight.y] != -1)
                {
                    //if value set - check, if it greater 
                    bufHeat = previous + (1 / mapTerrainMod[neighbors.downRight.x][neighbors.downRight.y]) + mapDamage[neighbors.downRight.x][neighbors.downRight.y];
                    if (mapHeat[neighbors.downRight.x][neighbors.downRight.y] > bufHeat)
                    {
                        mapHeat[neighbors.downRight.x][neighbors.downRight.y] = bufHeat;
                        toCheck.push_back(neighbors.downRight);
                    }
                }
            }
        }
        
        iter++;
        toCheck.pop_front();
    }

    //VECTOR MAP CALCULATION

    //float x_buf;
    //float y_buf;
    float minimumHeat;
    //Vector2 buf;
    //NeighborsIndex neighbors;
    //TileIndex upLeft, rightDown, indexMinimum;

#pragma omp parallel for private(neighbors, minimumHeat)
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            neighbors = neighborsIndices[x][y];
            //TODO: remake for using gradient?
            minimumHeat = -1;
            //left
            if (neighbors.left.x != -1) //check for map borders
            {
                if (mapTerrainMod[x - 1][y] != -1.f) //check for obstruction
                    if (mapHeat[x - 1][y] < minimumHeat || minimumHeat == -1)
                    {
                        minimumHeat = mapHeat[x - 1][y];
                        mapVector[x][y] = { -1, 0 };
                    }
            }
            //right
            if (neighbors.right.x != -1) //check for map borders
            {
                if (mapTerrainMod[x + 1][y] != -1.f) //check for obstruction
                    if (mapHeat[x + 1][y] < minimumHeat || minimumHeat == -1)
                    {
                        minimumHeat = mapHeat[x + 1][y];
                        mapVector[x][y] = { 1.f, 0.f };
                    }
            }
            //up
            if (neighbors.up.x != -1) //check for map borders
            {
                if (mapTerrainMod[x][y - 1] != -1.f) //check for obstruction
                    if (mapHeat[x][y - 1] < minimumHeat || minimumHeat == -1)
                    {
                        minimumHeat = mapHeat[x][y - 1];
                        mapVector[x][y] = { 0.f, -1.f };
                    }
            }
            //down
            if (neighbors.down.x != -1) //check for map borders
            {
                if (mapTerrainMod[x][y + 1] != -1.f) //check for obstruction
                    if (mapHeat[x][y + 1] < minimumHeat || minimumHeat == -1)
                    {
                        minimumHeat = mapHeat[x][y + 1];
                        mapVector[x][y] = { 0.f, 1.f };
                    }
            }

            //upLeft
            if (neighbors.upLeft.x != -1) //check for map borders
            {
                if (mapTerrainMod[x - 1][y - 1] != -1.f && (mapTerrainMod[x][y - 1] != -1.f || mapTerrainMod[x - 1][y] != -1.f)) //check for obstruction
                    if (mapHeat[x - 1][y - 1] < minimumHeat || minimumHeat == -1)
                    {
                        minimumHeat = mapHeat[x - 1][y - 1];
                        mapVector[x][y] = { -1.f, -1.f };
                    }
            }
            //upRight
            if (neighbors.upRight.x != -1) //check for map borders
            {
                if (mapTerrainMod[x + 1][y - 1] != -1.f && (mapTerrainMod[x][y - 1] != -1.f || mapTerrainMod[x + 1][y] != -1.f)) //check for obstruction
                    if (mapHeat[x + 1][y - 1] < minimumHeat || minimumHeat == -1)
                    {
                        minimumHeat = mapHeat[x + 1][y - 1];
                        mapVector[x][y] = { 1.f, -1.f };
                    }
            }
            //downLeft
            if (neighbors.downLeft.x != -1) //check for map borders
            {
                if (mapTerrainMod[x - 1][y + 1] != -1.f && (mapTerrainMod[x][y + 1] != -1.f || mapTerrainMod[x - 1][y] != -1.f)) //check for obstruction
                    if (mapHeat[x - 1][y + 1] < minimumHeat || minimumHeat == -1)
                    {
                        minimumHeat = mapHeat[x - 1][y + 1];
                        mapVector[x][y] = { -1.f, 1.f };
                    }
            }
            //downRight
            if (neighbors.downRight.x != -1) //check for map borders
            {
                if (mapTerrainMod[x + 1][y + 1] != -1.f && (mapTerrainMod[x][y + 1] != -1.f || mapTerrainMod[x + 1][y] != -1.f)) //check for obstruction
                    if (mapHeat[x + 1][y + 1] < minimumHeat || minimumHeat == -1)
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
    Constructor* buf_constructor;
    Hive* buf_hive;

    TileIndex mapCenter;
    TileIndex positionIndex;
    TileIndex desireLocation;

    switch (type)
    {
    case ActorType::LIGHT_TURRET:
    case ActorType::HEAVY_TURRET:
    case ActorType::AIRDEFENSE_TURRET:
        buf_turret = new Turret(
            this,
            wantToBuild,
            position,
            State::UNDER_CONSTRUCTION);

        unitsList.push_back(buf_turret);
        militaryUnitsList.push_back(buf_turret);
        turretUnitsList.push_back(buf_turret);
        break;
    case ActorType::LIGHT_INSECT:
    case ActorType::HEAVY_INSECT:
    case ActorType::FLYING_INSECT:
        buf_military = new Insect(
            this,
            wantToBuild,
            position,
            State::GOES);

        unitsList.push_back(buf_military);
        militaryUnitsList.push_back(buf_military);
        break;
    case ActorType::CORE:
        buf_constructor = new Core(
            this,
            ActorType::CORE,
            position,
            State::UNDER_CONSTRUCTION);
        unitsList.push_back(buf_constructor);
        expansionUnitsList_Machines.push_back(buf_constructor);
        constructorList.push_back(buf_constructor);
        break;
    case ActorType::BASE:
        buf_constructor = new Base(
            this,
            ActorType::BASE,
            position,
            State::ONLINE);
        unitsList.push_back(buf_constructor);
        expansionUnitsList_Machines.push_back(buf_constructor);
        basePtr = buf_constructor;
        constructorList.push_back(buf_constructor);
        insectsDesirePosition = basePtr->getPositionIndex(); //once base placed, game begins

        //SPAWNING HIVE
        mapCenter = { mapWidth / 2, mapHeight / 2 };
        positionIndex = getTileIndex(position);
        desireLocation = { mapCenter.x * 2 - positionIndex.x, mapCenter.y * 2 - positionIndex.y };

        for (TileIndex tile : tilesInsideCircleOrdered(desireLocation, 50))
        {
            if (mapExpansionCreep[tile.x][tile.y] == ExpandState::AVAILABLE)
            {
                addActor(ActorType::HIVE, Vector2{tile.x*pixelsPerTile + pixelsPerTile / 2, tile.y * pixelsPerTile + pixelsPerTile / 2 }, State::ONLINE);
                break;
            }
        }
        break;
    case ActorType::HIVE:
        buf_hive = new Hive(
            this,
            ActorType::HIVE,
            position,
            State::ONLINE);
        unitsList.push_back(buf_hive);
        expansionUnitsList_Insects.push_back(buf_hive);
        hivePtr = buf_hive;
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
    }
}
GameActor* GameData::getActorInTile(int x, int y)
{
    GameActor* result = nullptr;
    TileIndex pos;
    for (GameActor* actor : unitsList)
    {
        if (actor != nullptr)
        {
            pos = actor->getPositionIndex();
            if (pos.x == x && pos.y == y)
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

    std::vector<Constructor*>::iterator constructorIter;
    for (constructorIter = constructorList.begin(); constructorIter != constructorList.end(); )
    {
        if ((*constructorIter)->ID == ID)
            constructorIter = constructorList.erase(constructorIter);
        else
            constructorIter++;
    }
    

    if(buf!=nullptr)
        delete buf;
}
void GameData::Hit(GameActor* target, int damage, ActorType hitBy)
{
    bool result = false;
    damage -= target->armor;
    if (damage <= 0) damage = 1; //every hit deals at least 1 damage
    target->setHP(target->getHP() - damage);
    target->inBattle = true;
    target->inBattleCounter = 60;

    //TODO: consider victory 

    if (target->getHP() <= 0)
    {
        Side opposite;
        if (target->side == Side::INSECTS)
            opposite = Side::MACHINES;
        else
            opposite = Side::INSECTS;

        if (target->side == Side::MACHINES)
        {
            //TODO: make reducing damage map, if damaging Machines unit
            for (Constructor* constructor : constructorList)
            {
                constructor->SeekForTarget();
            }
        }
        else
        {
            //affect damage map if it is Insect 
            switch (target->type)
            {
            case ActorType::LIGHT_INSECT:
            case ActorType::HEAVY_INSECT:
            case ActorType::FLYING_INSECT:
                raiseDamageMap(target->type, damage, target->getPositionIndex());
                break;

            default:
                std::vector<ActorType> types = { ActorType::LIGHT_INSECT, ActorType::HEAVY_INSECT, ActorType::FLYING_INSECT };
                for (ActorType type : types)
                {
                    raiseDamageMap(type, damage, target->getPositionIndex());
                }
                break;
            }
        }

        removeActor(target->ID);
        recalculateMilitaryTargets(opposite);

        result = true;
    }
}
void GameData::raiseDamageMap(ActorType type, int damage, TileIndex tile)
{
    int** damageMap = mapsDamage[type];
    TileIndex targetTileIndex = tile;
    NeighborsIndex neighbors = getNeighbors(targetTileIndex.x, targetTileIndex.y);

    damageMap[targetTileIndex.x][targetTileIndex.y] += damage; //center
    if (neighbors.left.x != -1)
        damageMap[neighbors.left.x][neighbors.left.y] += damage / 2; //left
    if (neighbors.up.x != -1)
        damageMap[neighbors.up.x][neighbors.up.y] += damage / 2; //up
    if (neighbors.right.x != -1)
        damageMap[neighbors.right.x][neighbors.right.y] += damage / 2; //right
    if (neighbors.down.x != -1)
        damageMap[neighbors.down.x][neighbors.down.y] += damage / 2; //down

    if (neighbors.upLeft.x != -1)
        damageMap[neighbors.upLeft.x][neighbors.upLeft.y] += damage / 2; //upLeft
    if (neighbors.upRight.x != -1)
        damageMap[neighbors.upRight.x][neighbors.upRight.y] += damage / 2; //upRight
    if (neighbors.downLeft.x != -1)
        damageMap[neighbors.downLeft.x][neighbors.downLeft.y] += damage / 2; //downLeft
    if (neighbors.downRight.x != -1)
        damageMap[neighbors.downRight.x][neighbors.downRight.y] += damage / 2; //downRight
}

void GameData::revealTerritory(TileIndex position, int radius, Side side)
{
    //revealed tiles numered by distance between unit and tile

    int** matrix = mapsFogOfWar[side];
    int buf;
    int intelPrevious = matrix[position.x][position.y];

    //check tile, is it need to reveal for again
    if (static_cast<int>(matrix[position.x][position.y]) < radius)
    {
        matrix[position.x][position.y] = radius;

        for (int r = 1; r <= radius; r++)
        {
            buf = radius - r;
            for (int x = position.x + 1; x < position.x + r; x++)
            {
                for (int y = position.y; y > position.y - r; y--)
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

        if (side == Side::INSECTS && intelPrevious < matrix[position.x][position.y]) //recalculate only when unit on edge of vision
        {
            std::vector<ActorType> types = { ActorType::LIGHT_INSECT, ActorType::HEAVY_INSECT, ActorType::FLYING_INSECT };
#pragma omp parallel for
            for (int i = 0; i < types.size(); i++)
            {
                calculateVectorPathfinding(
                    insectsDesirePosition,
                    types[i]
                );
            }
        }
    }
}

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

    //TURRET BUILDING
    if (IsKeyPressed(KEY_ONE))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::LIGHT_TURRET;
    if (IsKeyPressed(KEY_TWO))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::HEAVY_TURRET;
    if (IsKeyPressed(KEY_THREE))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::AIRDEFENSE_TURRET;

    //BUILD BASE
    if (IsKeyPressed(KEY_B) && basePtr == nullptr)
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::BASE;
        else
            wantToBuild = ActorType::ACTOR_NULL;

    //BUILD CORE
    if (IsKeyPressed(KEY_Q))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::CORE;
        else
            wantToBuild = ActorType::ACTOR_NULL;
    
    
    //MANUAL INSECTS SPAWN
    if (IsKeyPressed(KEY_FOUR))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::LIGHT_INSECT;
    if (IsKeyPressed(KEY_FIVE))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::HEAVY_INSECT;
    if (IsKeyPressed(KEY_SIX))
        if (wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::FLYING_INSECT;

    //SPAWN TUMOR
    if (IsKeyPressed(KEY_E))
        if(wantToBuild == ActorType::ACTOR_NULL)
            wantToBuild = ActorType::TUMOR;
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
        calculateVectorPathfinding(insectsDesirePosition, ActorType::LIGHT_INSECT);
        calculateVectorPathfinding(insectsDesirePosition, ActorType::HEAVY_INSECT);
        calculateVectorPathfinding(insectsDesirePosition, ActorType::FLYING_INSECT);
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
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && wantToBuild != ActorType::ACTOR_NULL && mapExpansionEnergised[mouseIndex.x][mouseIndex.y] != ExpandState::UNAVAILABLE)
    {
        Vector2 position = { mouseIndex.x * pixelsPerTile + pixelsPerTile / 2, mouseIndex.y * pixelsPerTile + pixelsPerTile / 2 };
        addActor(wantToBuild, position, State::ONLINE);
    }

    if (!gamePaused && basePtr!=nullptr) //game starts only when base placed
    {
        //TIME COUNTER
        timeCount++;
        if (timeCount > 60)
        {
            timeCount = 0;
            timeCountSeconds++;
        }

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

        ////TUMOR SPAWNING
        if (timeCountSeconds % 3 == 0 && timeCount == 0)
        {
            int tumorCost = genericAttributes[ActorType::TUMOR]["cost"];
            if (trySpendResources(tumorCost, Side::INSECTS) == tumorCost) //if have enough Food
            {
                bool builded;
                int** maps[] = { mapsDamage[ActorType::LIGHT_INSECT], mapsDamage[ActorType::HEAVY_INSECT],mapsDamage[ActorType::FLYING_INSECT] };
                int tumorRadius = buildingsAttributes[ActorType::TUMOR]["expansionRange"];
                std::vector<GameActor*> actorBuf;

                std::vector<TileIndex> bufTiles;

                for (Building* bulding : expansionUnitsList_Insects)
                {
                    if (bulding->getState() != State::UNDER_CONSTRUCTION)
                    {
                        builded = false;
                        bufTiles = tilesInPerimeterCircleOrdered(bulding->getPositionIndex(), tumorRadius + 1);

                        for (TileIndex tile : bufTiles)
                        {
                            Vector2 vectorBuf = { tile.x * pixelsPerTile, tile.y * pixelsPerTile };

                            if (getActorInTile(tile.x, tile.y) == nullptr &&
                                getTerrainType(tile.x, tile.y) != TerrainType::MOUNTAIN &&
                                getTerrainType(tile.x, tile.y) != TerrainType::LAKE &&
                                mapExpansionCreep[tile.x][tile.y] != ExpandState::EXPANDED &&
                                mapExpansionCreep[tile.x][tile.y] != ExpandState::UNAVAILABLE &&
                                maps[0][tile.x][tile.y] == 0 &&
                                maps[1][tile.x][tile.y] == 0 &&
                                maps[2][tile.x][tile.y] == 0)
                            {
                                addActor(ActorType::TUMOR, vectorBuf, State::ONLINE);
                                spendResources(tumorCost, Side::INSECTS);
                                builded = true;
                                break;
                            }
                        }
                        if (builded)
                            break;
                    }
                }
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
                        if (numOfExpansionTileAdjoinFading(x, y, Side::INSECTS) < 3)
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

        //damage map fading
        if (timeCountSeconds % 5 == 0 && timeCount == 0)
        {
            //iterate through all insects types
            std::vector<ActorType> insectsTypes = { ActorType::LIGHT_INSECT,ActorType::HEAVY_INSECT, ActorType::FLYING_INSECT };
            for (ActorType actorType : insectsTypes)
            {
                int** matrixBuf = mapsDamage[actorType];
         
                for (int x = 0; x < mapWidth; x++)
                {
                    for (int y = 0; y < mapHeight; y++)
                    {
                        if (matrixBuf[x][y] >= 1)
                            matrixBuf[x][y] -= 1;
                        /*else
                            matrixBuf[x][y] = 0;*/
                    }
                }
         
                //vector field recalculating
                calculateVectorPathfinding(insectsDesirePosition, actorType);
            }
        }

        //damage map spread
        
        if (timeCount % 15 == 0)
        {
            //iterate through all insects types
            std::vector<ActorType> insectsTypes = { ActorType::LIGHT_INSECT,ActorType::HEAVY_INSECT, ActorType::FLYING_INSECT };
            for (ActorType actorType : insectsTypes)
            {
                int** matrixBuf = mapsDamage[actorType];
                int toTransfer;
                NeighborsIndex neighbors;
                for (int x = 0; x < mapWidth; x++)
                {
                    for (int y = 0; y < mapHeight; y++)
                    {
                        if (matrixBuf[x][y] >= 10) //if damage more than 36
                        {
                            neighbors = getNeighbors(x, y);
                            toTransfer = matrixBuf[x][y] * 0.1;
                            if (neighbors.left.x != -1)
                            {
                                matrixBuf[x][y] -= toTransfer;
                                matrixBuf[x - 1][y] += toTransfer;
                            }
                            if (neighbors.up.x != -1)
                            {
                                matrixBuf[x][y] -= toTransfer;
                                matrixBuf[x][y - 1] += toTransfer;
                            }
                            if (neighbors.right.x != -1)
                            {
                                matrixBuf[x][y] -= toTransfer;
                                matrixBuf[x + 1][y] += toTransfer;
                            }
                            if (neighbors.down.x != -1)
                            {
                                matrixBuf[x][y] -= toTransfer;
                                matrixBuf[x][y + 1] += toTransfer;
                            }
                        }
                    }
                }
            }
        }

        //military targets recalculation
        if (timeCountSeconds % 1 == 0 && timeCount == 0)
        {
            for (Militaty* unit : militaryUnitsList)
            {
                unit->SeekForEnemy();
            }
        }

        //RESOURCES GATHERING
        if (timeCount % 30 == 0)
        {
            resourcesInsects += creepTilesCount / 50;
            resourcesMachines += energisedTilesCount / 50;
            if (resourcesMachines > 1000) resourcesMachines = 1000; //maximum of Energy
        }

        //TILE ANIMATION
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
        int** mapDamage = mapsDamage[ActorType::LIGHT_INSECT];

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

                if (showingCreepStates)
                    DrawText(FormatText("%d", mapExpansionCreep[x][y]), x * pixelsPerTile + pixelsPerTile / 3, y * pixelsPerTile + pixelsPerTile / 3, 14, RED);
            
                //heatmap, light insects
                if(IsKeyDown(KEY_F4))
                    DrawText(FormatText("%.0f", mapsPathfinding[ActorType::LIGHT_INSECT]["mapsHeat"][x][y]), x * pixelsPerTile + pixelsPerTile / 3, y * pixelsPerTile + pixelsPerTile / 3, 6, SKYBLUE);

                //damage map, light insects
                if (IsKeyDown(KEY_F7))
                    DrawText(FormatText("%d", mapDamage[x][y]), x * pixelsPerTile + pixelsPerTile / 3, y * pixelsPerTile + pixelsPerTile / 3, 6, RED);

                //vector field, light insects
                if (IsKeyDown(KEY_F5) && terrainMod[x][y]!=-1.f)
                {                   
                    DrawLineEx(
                        Vector2{ x * pixelsPerTile + pixelsPerTile / 2,
                        y * pixelsPerTile + pixelsPerTile / 2 },
                        Vector2{ x * pixelsPerTile + pixelsPerTile / 2 + vectorField[x][y].x *6,
                        y * pixelsPerTile + pixelsPerTile / 2 + vectorField[x][y].y * 6,
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

    //for (TileIndex tile : tilesInPerimeterCircleOrdered(mouseIndex, buildingsAttributes[ActorType::TUMOR]["expansionRange"] + 1))
    //{
    //    DrawRectangle(tile.x * pixelsPerTile, tile.y * pixelsPerTile, pixelsPerTile, pixelsPerTile, Fade(SKYBLUE, 0.5f));
    //}

    if (this->wantToBuild != ActorType::ACTOR_NULL)
    {
        //draw hologram of chosen actor in any case

        Texture2D bufTexture;
        int size;
        Color tint;
        unsigned int radius;
        int** fogOfWar = mapsFogOfWar[visionSide];

        switch (wantToBuild)
        {
        case ActorType::LIGHT_TURRET:
        case ActorType::HEAVY_TURRET:
        case ActorType::AIRDEFENSE_TURRET:
            DrawCircleLines(
                mouseIndex.x * pixelsPerTile + pixelsPerTile / 2,
                mouseIndex.y * pixelsPerTile + pixelsPerTile / 2,
                militaryAttributes[wantToBuild]["attackRange"],
                RED
            );
            DrawCircleLines(
                mouseIndex.x* pixelsPerTile + pixelsPerTile / 2,
                mouseIndex.y* pixelsPerTile + pixelsPerTile / 2,
                militaryAttributes[wantToBuild]["seekRange"],
                ORANGE
            );
            DrawCircleLines(
                mouseIndex.x* pixelsPerTile + pixelsPerTile / 2,
                mouseIndex.y* pixelsPerTile + pixelsPerTile / 2,
                connectableAttributes[wantToBuild]["connectRange"],
                DARKBLUE
            );

            bufTexture = getUnitAnimation(ActorType::TURRET_CHASIS, State::CHANGING_MODE).frames[3];
            size = genericAttributes[wantToBuild]["size"];
            
            if (mapExpansionEnergised[mouseIndex.x][mouseIndex.y] != ExpandState::UNAVAILABLE)
                tint = GREEN;
            else
                tint = RED;
                
            DrawTexturePro(
                bufTexture, //texture
                Rectangle{ 0,0, (float)bufTexture.width, (float)bufTexture.height }, //source
                Rectangle{
                    mouseIndex.x * pixelsPerTile + pixelsPerTile / 2,
                    mouseIndex.y * pixelsPerTile + pixelsPerTile / 2,
                    (float)size * 2.f, (float)size * 2.f }, //dest
                Vector2{ (float)(size), (float)(size) }, //origin
                (float)0, //rotation
                tint
                );

            break;
        case ActorType::LIGHT_INSECT:
        case ActorType::HEAVY_INSECT:
        case ActorType::FLYING_INSECT:
            break;
        case ActorType::CORE:
            DrawCircleLines(
                mouseIndex.x * pixelsPerTile + pixelsPerTile / 2,
                mouseIndex.y * pixelsPerTile + pixelsPerTile / 2,
                connectableAttributes[wantToBuild]["connectRange"],
                YELLOW
            );
        case ActorType::BASE:
            DrawCircleLines(
                mouseIndex.x * pixelsPerTile + pixelsPerTile / 2,
                mouseIndex.y * pixelsPerTile + pixelsPerTile / 2,
                constructorsAttributes[wantToBuild]["buildRange"],
                VIOLET
            );

            radius = buildingsAttributes[wantToBuild]["expansionRange"];
            for (TileIndex tile : tilesInsideCircle(Vector2{ static_cast<float>(mouseIndex.x), static_cast<float>(mouseIndex.y) }, radius))
            {
                if(mapExpansionEnergised[tile.x][tile.y]==ExpandState::AVAILABLE && fogOfWar[tile.x][tile.y]!=-1)
                    DrawRectangle(tile.x * pixelsPerTile, tile.y * pixelsPerTile, pixelsPerTile, pixelsPerTile, Fade(SKYBLUE, 0.5f));
            }

            if (mapExpansionEnergised[mouseIndex.x][mouseIndex.y]!=ExpandState::UNAVAILABLE)
            {
                DrawCircle(
                    mouseIndex.x* pixelsPerTile + pixelsPerTile / 2,
                    mouseIndex.y* pixelsPerTile + pixelsPerTile / 2,
                    genericAttributes[wantToBuild]["size"],
                    Fade(SKYBLUE, 0.5f));
            }
            else
            {
                DrawCircle(
                    mouseIndex.x * pixelsPerTile + pixelsPerTile / 2,
                    mouseIndex.y * pixelsPerTile + pixelsPerTile / 2,
                    genericAttributes[wantToBuild]["size"],
                    Fade(RED, 0.5f));
            }

        case ActorType::HIVE:
        case ActorType::TUMOR:
            //draw blue circle of expansion
            break;
        }
    }

    if (wantToRemove)
    {
        DrawRectangle(mouseIndex.x * pixelsPerTile, mouseIndex.y * pixelsPerTile, pixelsPerTile, pixelsPerTile, Fade(RED, 0.3f));
    }

    TileIndex buf_position;
    int** mapFogOfWar = mapsFogOfWar[visionSide];

    for (GameActor* actor : this->unitsList)
    {
        //TODO: consider fog of war
        if (actor != nullptr)
        {
            buf_position = actor->getPositionIndex();
            if (buf_position.x >= renderBorders[1] &&
                buf_position.x < renderBorders[3] &&
                buf_position.y >= renderBorders[0] &&
                buf_position.y < renderBorders[2] &&
                mapFogOfWar[buf_position.x][buf_position.y] != -1)
            {
                actor->Draw();
            }
        }
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
        std::vector<std::string> matrices = { "mapsHeat", "mapsTerrainMod"};
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