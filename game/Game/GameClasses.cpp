#include "GameClasses.h"
#include <omp.h>

GameData::GameData()
{
    screenSize = Vector2{ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };

    camera.target = { 0, 0 };
    camera.offset = { screenSize.x/2, screenSize.y/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    pixelsPerTile = 16;
}

bool GameData::isMapLoaded()
{
    return this->mapTerrain != nullptr;
}

void GameData::setTerrain(Terrain terr)
{
    this->mapHeight = terr.height;
    this->mapWidth = terr.width;
    this->mapTerrain = terr.map;

    mapSize.x = mapWidth * pixelsPerTile;
    mapSize.y = mapHeight * pixelsPerTile;

    Image buf = GenImageColor(mapSize.x, mapSize.y, WHITE);

    int index;
    Color pallete[] = { DARKBLUE, BLUE, YELLOW, GREEN, DARKGREEN, GRAY, BLACK };


    //TODO: check blending principle for generating texture
#pragma omp parallel for private(index)
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            index = mapWidth * j + i;

            ImageDrawRectangle(&buf, i * pixelsPerTile, j * pixelsPerTile, pixelsPerTile, pixelsPerTile, pallete[static_cast<int>(mapTerrain[index])]);
        }
    }

    terrainTexture = LoadTextureFromImage(buf);

}

void GameData::GameUpdate()
{
    if (IsKeyPressed(KEY_ESCAPE)) closed = true;

    cursor = GetMousePosition();

    if ((cursor.x < screenSize.x * 0.15f || IsKeyDown(KEY_A)) && camera.target.x > 100)           camera.target.x -= 10/ camera.zoom;
    if ((cursor.x > screenSize.x * 0.85f || IsKeyDown(KEY_D)) && camera.target.x < mapSize.x)     camera.target.x += 10/ camera.zoom;
    if ((cursor.y < screenSize.y * 0.15f || IsKeyDown(KEY_W)) && camera.target.y > 100)           camera.target.y -= 10/ camera.zoom;
    if ((cursor.y > screenSize.y * 0.85f || IsKeyDown(KEY_S)) && camera.target.y < mapSize.y)     camera.target.y += 10/ camera.zoom;

    

    // Camera target shift
    //camera.target = { camera.target.x + 20, camera.target.y + 20 };

    // Camera rotation controls
    /*if (IsKeyDown(KEY_A)) camera.rotation--;
    else if (IsKeyDown(KEY_S)) camera.rotation++;*/

    // Limit camera rotation to 80 degrees (-40 to 40)
    /*if (camera.rotation > 40) camera.rotation = 40;
    else if (camera.rotation < -40) camera.rotation = -40;*/

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

    //DrawRectangleLinesEx(screen, 5, RED);
    

    

}