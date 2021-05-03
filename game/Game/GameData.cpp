#include "GameClasses.h"
#include <omp.h>

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
        "textures\\tileset\\CREEP.png" };

    for (int i = 0; i < 8; i++)
    {
        tileset[i] = LoadImage(filenames[i]);
        ImageFormat(&tileset[i], UNCOMPRESSED_R5G6B5);
    }

    for (int i = 0; i < 8; i++)
    {
        tilesetTex[i] = LoadTextureFromImage(tileset[i]);
    }

    //TODO: texture files loading, units properties
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
    //TODO: MAKE NEW FUNCTION TO REDRAW PREVIEW TEXTURE, CALL IT ONLY FEW TIMES IN UPDATE
    if (isMapLoaded())
    {
        UnloadTexture(terrainTexture);

        for (int i = 0; i < mapHeight; i++)
                delete[] mapExpansionCreep[i];
        delete[] mapExpansionCreep;
    }

    this->mapHeight = terr.height;
    this->mapWidth = terr.width;
    this->mapTerrain = terr.map;

    mapSize.x = mapWidth * pixelsPerTile;
    mapSize.y = mapHeight * pixelsPerTile;

    //expansion maps memory allocation
    mapExpansionCreep = new unsigned char* [mapHeight];
    for (int i = 0; i < mapHeight; i++)
        mapExpansionCreep[i] = new unsigned char[mapWidth];

    //expansion pre-calculation
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            switch (getTerrainType(i, j))
            {
            //if cant expand in this tile
            case TerrainType::LAKE:
            case TerrainType::SWAMP:
            case TerrainType::MOUNTAIN:
                mapExpansionCreep[i][j] = 0;
                break;
            //if expansion can be provide
            case TerrainType::SAND:
            case TerrainType::PLAIN:
            case TerrainType::TREE:
            case TerrainType::STONE:
            case TerrainType::ASH:
                mapExpansionCreep[i][j] = 1;
                break;
            //just in case
            default:
                mapExpansionCreep[i][j] = 0;
                break;
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

void GameData::recalculateExpansion(Side side)
{
    for (Building* actor : this->expansionUnitsList)
    {
        if(actor->side==side)
            actor->markExpandArea();
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
        radius = 1;
    if (IsKeyPressed(KEY_TWO))
        radius = 2;
    if (IsKeyPressed(KEY_THREE))
        radius = 3;
    if (IsKeyPressed(KEY_FOUR))
        radius = 4;
    if (IsKeyPressed(KEY_FIVE))
        radius = 5;
    if (IsKeyPressed(KEY_SIX))
        radius = 6;
    if (IsKeyPressed(KEY_SEVEN))
        radius = 12;

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
        for (int i = 0; i < mapHeight; i++)
        {
            for (int j = 0; j < mapWidth; j++)
            {
                if (mapExpansionCreep[i][j] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                {
                    if (numOfExpansionTileAdjoin(i, j, Side::INSECTS) < 3)
                        buf.push_back(TileIndex{ i,j });
                }
            }
        }

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

    if (camera.zoom < 1.f) //low-detailed map
    {
        DrawTextureEx(
            terrainTexture,
            Vector2{ 0.f, 0.f },
            0.f, pixelsPerTile, WHITE);
    }
    else
    {                    //full-detailed map
        int index;
        for (int i = renderBorders[0]; i < renderBorders[2]; i++)
        {
            for (int j = renderBorders[1]; j < renderBorders[3]; j++)
            {
                index = mapWidth * i + j;
                if (mapExpansionCreep[j][i] == ExpandState::EXPANDED || mapExpansionCreep[j][i] == ExpandState::EXPANDED_WITHOUT_SOURCE)
                {
                    DrawTexture(
                        tilesetTex[7],
                        j * pixelsPerTile,
                        i * pixelsPerTile,
                        WHITE);
                }
                else
                {
                    DrawTexture(
                        tilesetTex[static_cast<int>(mapTerrain[index])],
                        j * pixelsPerTile,
                        i * pixelsPerTile,
                        WHITE);
                }

                if (showingCreepStates)
                    DrawText(FormatText("%d", mapExpansionCreep[j][i]), j * pixelsPerTile + pixelsPerTile / 3, i * pixelsPerTile + pixelsPerTile / 3, 14, RED);
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

    //for (TileIndex tile : tilesInPerimeterCircle(Vector2{ static_cast<float>(mouseIndex.x), static_cast<float>(mouseIndex.y) }, radius))
    //{
    //    DrawRectangle(tile.x * pixelsPerTile, tile.y * pixelsPerTile, pixelsPerTile, pixelsPerTile, RED);
    //}

    EndMode2D();
    DrawText(FormatText("%f", camera.zoom), 20, 20, 20, RED);

    DrawFPS(20, 50);
}

GameData::~GameData()
{
    //Unloading images and textures

    for (int i = 0; i < 7; i++)
        UnloadImage(tileset[i]);

    if (isMapLoaded())
    {
        delete[] mapTerrain;
        UnloadTexture(terrainTexture);
    }

    for (int i = 0; i < 7; i++)
    {
        UnloadTexture(tilesetTex[i]);
    }

    delete[] palette;

    if (isMapLoaded())
    {
        for (int i = 0; i < mapHeight; i++)
                delete[] mapExpansionCreep[i];
        delete[] mapExpansionCreep;
    }

}