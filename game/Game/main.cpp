#define _CRT_SECURE_NO_WARNINGS
#define RAYGUI_IMPLEMENTATION

//MOVE INCLUDES TO WHERE THEY BELONG (and remove what dont need)
#include "PerlinNoise.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "raygui.h"
#include "raylib.h"
#include "TerrainGenerator.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1700;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "diploma game");
    SetTargetFPS(60);
    SetExitKey(KEY_HOME); //releasing ESC-key for in-gameplay menu

    bool gameClosed = false;

    GameState state = GameState::MAIN_MENU;
    GameData gameData;
    TerrainGenerator terrain{};

    GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0x282828ff);

    // Main game loop
    while (!gameClosed)        // Detect window close button
    {
        BeginDrawing();
        ClearBackground(Color{ 150,150,150,255 });

        switch (state)
        {
            //TODO: same zeropoint-based visualisation
        case GameState::MAIN_MENU:
            if (GuiButton(Rectangle{ screenWidth /2.f - 200, 200, 100,50}, "Play") && gameData.isMapLoaded())
                state = GameState::GAMEPLAY;
            
            if (GuiButton(Rectangle{ screenWidth / 2.f - 250, 300, 200,50 }, "Generate terrain"))
                state = GameState::TERRAIN_GENERATOR;

            if (GuiButton(Rectangle{ screenWidth / 2.f - 200, 400, 100,50 }, "Exit"))
                gameClosed = true;

            break;
        case GameState::TERRAIN_GENERATOR:
            terrain.DrawInterface();
            if (terrain.closed)
            {
                gameData.setTerrain(terrain.getMap());
                state = GameState::MAIN_MENU;
            }
            break;
        case GameState::GAMEPLAY:
            gameData.GameUpdate();
            gameData.GameDraw();

            if (gameData.closed)
            {
                state = GameState::MAIN_MENU;
                gameData.closed = false;
            }

            break;
        case GameState::OPTIONS:
            break;
        default:
            break;
        }
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}