#include "GameClasses.h"

GameData::GameData()
{
    screenSize = Vector2{ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };

	camera.target = { 0, 0 };
	camera.offset = { screenSize.x/2, screenSize.y/2 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

    pixelsPerTile = 32;
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
}

void GameData::GameUpdate()
{
    if (IsKeyPressed(KEY_ESCAPE)) closed = true;

	cursor = GetMousePosition();

    if ((cursor.x < screenSize.x / 4 && camera.target.x > 100)         || IsKeyDown(KEY_A)) camera.target.x -= 10;
    if ((cursor.x > mapWidth - 50 && camera.target.x < mapWidth)       || IsKeyDown(KEY_D)) camera.target.x += 10;
    if ((cursor.y < 50 && camera.target.y > 100)                       || IsKeyDown(KEY_W)) camera.target.y -= 10;
    if ((cursor.y > mapHeight - 50 && camera.target.y < mapHeight)     || IsKeyDown(KEY_S)) camera.target.y += 10;

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
    BeginMode2D(camera);
    
    Color pallete[] = {	DARKBLUE, BLUE, YELLOW, GREEN, DARKGREEN, GRAY, BLACK};

    //draw terrain
    int index;
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            index = mapWidth * j + i;
            
            DrawRectangleRec(Rectangle{ i * 32.f, j * 32.f, 32.f,32.f }, pallete[static_cast<int>(mapTerrain[index])]);
        }
    }

}