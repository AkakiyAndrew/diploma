#include "GameClasses.h"

GameData::GameData()
{
	camera.target = { 0, 0 };
	camera.offset = { 0, 0 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

    screenSize = Vector2{ GetScreenWidth(), GetScreenHeight() };
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
}

void GameData::GameUpdate()
{
	cursor = GetMousePosition();

    cursor = GetMousePosition();
    if (cursor.x < screenSize.x / 4) camera.target.x -= 6;
    else if (cursor.x > (screenSize.x * 3) / 4) camera.target.x += 6;

    // Camera target shift
    camera.target = { camera.target.x + 20, camera.target.y + 20 };

    // Camera rotation controls
    if (IsKeyDown(KEY_A)) camera.rotation--;
    else if (IsKeyDown(KEY_S)) camera.rotation++;

    // Limit camera rotation to 80 degrees (-40 to 40)
    if (camera.rotation > 40) camera.rotation = 40;
    else if (camera.rotation < -40) camera.rotation = -40;

    // Camera zoom controls
    camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

    if (camera.zoom > 3.0f) camera.zoom = 3.0f;
    else if (camera.zoom < 0.1f) camera.zoom = 0.1f;
}

void GameData::GameDraw()
{
	
}