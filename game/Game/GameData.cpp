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
    tileset = new Image[7];

    const char* filenames[] = { "textures\\tileset\\LAKE.bmp","textures\\tileset\\SWAMP.bmp","textures\\tileset\\SAND.bmp","textures\\tileset\\PLAIN.bmp","textures\\tileset\\TREE.bmp","textures\\tileset\\STONE.bmp","textures\\tileset\\MOUNTAIN.bmp" };
    for (int i = 0; i < 7; i++)
    {
        tileset[i] = LoadImage(filenames[i]);
        ImageFormat(&tileset[i], UNCOMPRESSED_R5G6B5);
    }

    for (int i = 0; i < 7; i++)
    {
        tilesetTex[i] = LoadTextureFromImage(tileset[i]);
    }

    //TODO: texture files loading, units properties
}

bool GameData::isMapLoaded()
{
    return this->mapTerrain != nullptr;
}

void GameData::setTerrain(Terrain terr)
{
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

    //TODO: MAKE NEW FUNCTION TO REDRAW PREVIEW TEXTURE, CALL IT ONLY FEW TIMES IN UPDATE
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

std::vector<TileIndex> GameData::tilesCircleIntersecting(Vector2 center, unsigned int radius)
{
    std::vector<TileIndex> result;
    TileIndex upLeftCorner = { std::clamp<int>(center.x - radius, 0, center.x), std::clamp<int>(center.y - radius, 0, center.y) };
    TileIndex downRightCorner = { std::clamp<int>(center.x + radius, center.x, this->mapWidth), std::clamp<int>(center.y + radius, center.y, this->mapHeight) };

    for (int i = upLeftCorner.x; i < downRightCorner.x; i++)
    {
        for (int j = upLeftCorner.y; j < downRightCorner.y; j++)
        {
            result.push_back(TileIndex{i,j});
        }
    }
    //CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius)

    return result;
}

void GameData::GameUpdate()
{
    if (IsKeyPressed(KEY_ESCAPE)) closed = true;
    
    mousePosition = GetMousePosition();
    mouseWorldPosition = GetScreenToWorld2D(mousePosition, camera);
    mouseIndex = { std::clamp<int>(mouseWorldPosition.x/pixelsPerTile, 0, this->mapWidth), std::clamp<int>(mouseWorldPosition.y / pixelsPerTile, 0, this->mapHeight) };

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
}

void GameData::GameDraw()
{
    BeginMode2D(camera);

    //draw terrain

#ifndef TILE_DRAWING
    DrawTextureEx(
        terrainTexture,
        Vector2{ 0.f, 0.f },
        0.f, pixelsPerTile, WHITE);
#else

    if (camera.zoom < 0.7) //low-detailed map
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
                DrawTexture(
                    tilesetTex[static_cast<int>(mapTerrain[index])],
                    j * pixelsPerTile,
                    i * pixelsPerTile,
                    WHITE);
            }
        }
    }

    DrawRectangleLinesEx(
        viewBorders,
        5,
        RED
    );

    DrawRectangle(camera.target.x, camera.target.y, 32, 32, YELLOW);
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
            break;
        }
    }

    

    for (TileIndex tile : tilesCircleIntersecting(Vector2{ static_cast<float>(mouseIndex.x), static_cast<float>(mouseIndex.y) }, 4))
    {
        DrawRectangle(tile.x * pixelsPerTile, tile.y * pixelsPerTile, pixelsPerTile, pixelsPerTile, RED);
    }
    EndMode2D();
    DrawText(FormatText("%f", camera.zoom), 20, 20, 20, RED);
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