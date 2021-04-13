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

    TerrainGenerator terrain{};
    
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        BeginDrawing();

        // Background
        ClearBackground(Color{150,150,150,255});
        terrain.DrawInterface(); //???????? ?? return ????????? ?? ??????????????? ?????????
        //DrawTexture(texture, 0, 0, WHITE);

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}