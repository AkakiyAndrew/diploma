#include "GameClasses.h"
#include <omp.h>

#define TILE_DRAWING

GameData::GameData()
{
    screenSize = Vector2{ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };

    camera.target = { screenSize.x/2, screenSize.y/2 };
    camera.offset = { screenSize.x/2, screenSize.y/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    //tileset loading
    tileset = new Image[7];

    const char* filenames[] = { "textures\\tileset\\LAKE.bmp","textures\\tileset\\SWAMP.bmp","textures\\tileset\\SAND.bmp","textures\\tileset\\PLAIN.bmp","textures\\tileset\\TREE.bmp","textures\\tileset\\STONE.bmp","textures\\tileset\\MOUNTAIN.bmp"};
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
        UnloadTexture(terrainTexture);

    this->mapHeight = terr.height;
    this->mapWidth = terr.width;
    this->mapTerrain = terr.map;

    mapSize.x = mapWidth * pixelsPerTile;
    mapSize.y = mapHeight * pixelsPerTile;
    int size = sizeof(unsigned short);
    unsigned short *colorPixels = new unsigned short [mapSize.x * mapSize.y];

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
            //ImageDrawRectangle(&buf, i * pixelsPerTile, j * pixelsPerTile, pixelsPerTile, pixelsPerTile, pallete[static_cast<int>(mapTerrain[index])]);
        }
    }

    terrainTexture = LoadTextureFromImage(buf);
    UnloadImage(buf);
}

void GameData::GameUpdate()
{
    if (IsKeyPressed(KEY_ESCAPE)) closed = true;

    cursor = GetMousePosition();

    if ((cursor.x < screenSize.x * 0.15f || IsKeyDown(KEY_A)) /*&& camera.target.x > 100*/)           camera.target.x -= 10/ camera.zoom;
    if ((cursor.x > screenSize.x * 0.85f || IsKeyDown(KEY_D)) /*&& camera.target.x < mapSize.x*/)     camera.target.x += 10/ camera.zoom;
    if ((cursor.y < screenSize.y * 0.15f || IsKeyDown(KEY_W)) /*&& camera.target.y > 100*/)           camera.target.y -= 10/ camera.zoom;
    if ((cursor.y > screenSize.y * 0.85f || IsKeyDown(KEY_S)) /*&& camera.target.y < mapSize.y*/)     camera.target.y += 10/ camera.zoom;

    // Camera zoom controls
    camera.zoom += ((float)GetMouseWheelMove() * 0.2f);

    if (camera.zoom > 10.0f) camera.zoom = 10.0f;
    else if (camera.zoom < 0.1f) camera.zoom = 0.1f;
}

void GameData::GameDraw()
{
    Vector2 buf1 = GetScreenToWorld2D(Vector2{ 0,0 }, camera);
    Vector2 buf2 = GetScreenToWorld2D(Vector2{ screenSize.x,screenSize.y }, camera);
    
    //Vector2 buf2 = GetScreenToWorld2D(Vector2{buf1.x+ screenSize.x,buf1.y + screenSize.y }, camera);

    Rectangle screen = {
        buf1.x,
        buf1.y,
        buf2.x - buf1.x,
        buf2.y - buf1.y
    };

    BeginMode2D(camera);

    DrawRectangle(camera.target.x, camera.target.y, 32, 32, YELLOW);

#ifndef TILE_DRAWING
    DrawTexture(terrainTexture, 0, 0, WHITE);
#else
    //draw terrain
    int index;
    //TODO: DEFINE WHICH i, j, mapHeight AND mapWidth WE SHOULD DRAW DEPENDS ON CAMERA POSITION, ZOOM AND WINDOW SIZE

    DrawRectangleLinesEx(
        screen,
        5,
        RED
        );

    DrawRectangle(buf2.x, buf2.y, 32, 32, DARKBLUE);

    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            index = mapWidth * i + j;
            DrawTexture(
                tilesetTex[static_cast<int>(mapTerrain[index])],
                j * pixelsPerTile,
                i * pixelsPerTile,
                WHITE);

            //ImageDrawRectangle(&buf, i * pixelsPerTile, j * pixelsPerTile, pixelsPerTile, pixelsPerTile, pallete[static_cast<int>(mapTerrain[index])]);
        }
    }
#endif // !TILE_DRAWING
    EndMode2D();

    DrawText(FormatText("%f, %f", buf1.x, buf1.y), 20.f, 20.f, 20, BLUE);
    DrawText(FormatText("%f, %f", buf2.x, buf2.y), 1200.f, 550.f, 20, BLUE);
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

}