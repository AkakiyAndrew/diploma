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
    const int screenWidth = 1400;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "raylib [core] map generation");
    SetTargetFPS(60);

    GameState state = GameState::MAIN_MENU;
    GameData gameData;
    TerrainGenerator terrain{};
    
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(Color{ 150,150,150,255 });

        switch (state)
        {
        case GameState::MAIN_MENU:
            if (GuiButton(Rectangle{}, "Play") && gameData.isMapLoaded())
                state = GameState::GAMEPLAY;
            
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
            break;
        case GameState::OPTIONS:
            break;
        default:
            break;
        }


        // Background
        
        
        //DrawTexture(texture, 0, 0, WHITE);

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}