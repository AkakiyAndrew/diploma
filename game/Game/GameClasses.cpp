#include "GameClasses.h"
#include <omp.h>

GameData::GameData()
{
    screenSize = Vector2{ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };

    camera.target = { 0, 0 };
    camera.offset = { screenSize.x/2, screenSize.y/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    //tileset loading
    tileset = new Image[7];

    const char* filenames[] = { "textures\\tileset\\LAKE.bmp","textures\\tileset\\SWAMP.bmp","textures\\tileset\\SAND.bmp","textures\\tileset\\PLAIN.bmp","textures\\tileset\\TREE.bmp","textures\\tileset\\STONE.bmp","textures\\tileset\\MOUNTAIN.bmp"};
    for (int i = 0; i < 7; i++)
        tileset[i] = LoadImage(filenames[i]);
    //TODO: texture files loading, units properties
}

bool GameData::isMapLoaded()
{
    return this->mapTerrain != nullptr;
}

void GameData::setTerrain(Terrain terr)
{
    bool textureToUpdate = false;
    if (this->mapTerrain != nullptr)
    {
        delete[] this->mapTerrain; //ALSO DELETE terr.map!!!
        this->mapTerrain = nullptr;
        textureToUpdate = true;
    }
    
    this->mapHeight = terr.height;
    this->mapWidth = terr.width;
    this->mapTerrain = terr.map;

    mapSize.x = mapWidth * pixelsPerTile;
    mapSize.y = mapHeight * pixelsPerTile;

    Image buf = GenImageColor(mapSize.x, mapSize.y, WHITE);
    //TODO: Solve problem with format
    //ImageFormat(&buf, UNCOMPRESSED_R8G8B8); 

    int index;
    Color pallete[] = { DARKBLUE, BLUE, YELLOW, GREEN, DARKGREEN, GRAY, BLACK };


    //TODO: check blending principle for generating texture
#pragma omp parallel for private(index)
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            index = mapWidth * j + i;
            ImageDraw(
                &buf, 
                tileset[static_cast<int>(mapTerrain[index])],
                Rectangle{ 0,0, pixelsPerTile, pixelsPerTile },
                Rectangle{ i * pixelsPerTile, j * pixelsPerTile, pixelsPerTile, pixelsPerTile },
                WHITE);
            //ImageDrawRectangle(&buf, i * pixelsPerTile, j * pixelsPerTile, pixelsPerTile, pixelsPerTile, pallete[static_cast<int>(mapTerrain[index])]);
        }
    }

    if (textureToUpdate)
        UpdateTexture(terrainTexture, buf.data);
    else
        terrainTexture = LoadTextureFromImage(buf);
    UnloadImage(buf);
}

void GameData::GameUpdate()
{
    if (IsKeyPressed(KEY_ESCAPE)) closed = true;

    cursor = GetMousePosition();

    if ((cursor.x < screenSize.x * 0.15f || IsKeyDown(KEY_A)) && camera.target.x > 100)           camera.target.x -= 10/ camera.zoom;
    if ((cursor.x > screenSize.x * 0.85f || IsKeyDown(KEY_D)) && camera.target.x < mapSize.x)     camera.target.x += 10/ camera.zoom;
    if ((cursor.y < screenSize.y * 0.15f || IsKeyDown(KEY_W)) && camera.target.y > 100)           camera.target.y -= 10/ camera.zoom;
    if ((cursor.y > screenSize.y * 0.85f || IsKeyDown(KEY_S)) && camera.target.y < mapSize.y)     camera.target.y += 10/ camera.zoom;

    // Camera zoom controls
    camera.zoom += ((float)GetMouseWheelMove() * 0.2f);

    if (camera.zoom > 10.0f) camera.zoom = 10.0f;
    else if (camera.zoom < 0.1f) camera.zoom = 0.1f;
}

void GameData::GameDraw()
{
    Rectangle screen = {camera.target.x - camera.offset.x, camera.target.y - camera.offset.y, screenSize.x, screenSize.y };

    BeginMode2D(camera);

    //draw terrain
    DrawTexture(terrainTexture, 0, 0, WHITE);

    EndMode2D();
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
        
}