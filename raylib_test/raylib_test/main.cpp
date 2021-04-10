///*******************************************************************************************
//*
//*   raygui - Controls test
//*
//*   TEST CONTROLS:
//*       - GuiScrollPanel()
//*
//*   DEPENDENCIES:
//*       raylib 2.4  - Windowing/input management and drawing.
//*       raygui 2.0  - Immediate-mode GUI controls.
//*
//*   COMPILATION (Windows - MinGW):
//*       gcc -o $(NAME_PART).exe $(FILE_NAME) -I../../src -lraylib -lopengl32 -lgdi32 -std=c99
//*
//*   COMPILATION (Linux - gcc):
//*	gcc -o $(NAME_PART) $(FILE_NAME) -I../../src -lraylib -std=c99
//*
//*   LICENSE: zlib/libpng
//*
//*   Copyright (c) 2019 Vlad Adrian (@Demizdor) and Ramon Santamaria (@raysan5)
//*
//**********************************************************************************************/
//#define _CRT_SECURE_NO_WARNINGS
//#include <raylib.h>
//
//#define RAYGUI_IMPLEMENTATION
//#include "raygui.h"                 // Required for GUI controls
//
//int main(void)
//{
//    // Initialization
//    //--------------------------------------------------------------------------------------
//    const int screenWidth = 800;
//    const int screenHeight = 450;
//
//    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - draw circle sector");
//
//    Vector2 center = { (GetScreenWidth() - 300) / 2, GetScreenHeight() / 2 };
//
//    float outerRadius = 180.0f;
//    float startAngle = 0.0f;
//    float endAngle = 180.0f;
//    int segments = 0;
//    int minSegments = 4;
//
//    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
//    //--------------------------------------------------------------------------------------
//
//    // Main game loop
//    while (!WindowShouldClose())    // Detect window close button or ESC key
//    {
//        // Update
//        //----------------------------------------------------------------------------------
//        // NOTE: All variables update happens inside GUI control functions
//        //----------------------------------------------------------------------------------
//
//        // Draw
//        //----------------------------------------------------------------------------------
//        BeginDrawing();
//
//        ClearBackground(RAYWHITE);
//
//        DrawLine(500, 0, 500, GetScreenHeight(), Fade(LIGHTGRAY, 0.6f));
//        DrawRectangle(500, 0, GetScreenWidth() - 500, GetScreenHeight(), Fade(LIGHTGRAY, 0.3f));
//
//        DrawCircleSector(center, outerRadius, startAngle, endAngle, segments, Fade(MAROON, 0.3));
//        DrawCircleSectorLines(center, outerRadius, startAngle, endAngle, segments, Fade(MAROON, 0.6));
//
//        // Draw GUI controls
//        //------------------------------------------------------------------------------
//        startAngle = GuiSliderBar(Rectangle { 600, 40, 120, 20 }, "StartAngle", NULL, startAngle, 0, 720);
//        endAngle = GuiSliderBar(Rectangle { 600, 70, 120, 20 }, "EndAngle", NULL, endAngle, 0, 720);
//
//        outerRadius = GuiSliderBar(Rectangle { 600, 140, 120, 20 }, "Radius", NULL, outerRadius, 0, 200);
//        segments = GuiSliderBar(Rectangle { 600, 170, 120, 20 }, "Segments", NULL, segments, 0, 100);
//        //------------------------------------------------------------------------------
//
//        minSegments = (int)ceilf((endAngle - startAngle) / 90);
//        DrawText(TextFormat("MODE: %s", (segments >= minSegments) ? "MANUAL" : "AUTO"), 600, 200, 10, (segments >= minSegments) ? MAROON : DARKGRAY);
//
//        DrawFPS(10, 10);
//
//        EndDrawing();
//        //----------------------------------------------------------------------------------
//    }
//
//    // De-Initialization
//    //--------------------------------------------------------------------------------------  
//    CloseWindow();        // Close window and OpenGL context
//    //--------------------------------------------------------------------------------------
//
//    return 0;
//}



/*******************************************************************************************
*
*   raylib [textures] example - Texture loading and drawing a part defined by a rectangle
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define MAX_FRAME_SPEED     15
#define MIN_FRAME_SPEED      1

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [texture] example - texture rectangle");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    Texture2D scarfy = LoadTexture("resources/scarfy.png");        // Texture loading

    Vector2 position = { 350.0f, 280.0f };
    Rectangle frameRec = { 0.0f, 0.0f, (float)scarfy.width / 6, (float)scarfy.height };
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 8;            // Number of spritesheet frames shown by second

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        framesCounter++;

        if (framesCounter >= (60 / framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;

            if (currentFrame > 5) currentFrame = 0;

            frameRec.x = (float)currentFrame * (float)scarfy.width / 6;
        }

        if (IsKeyPressed(KEY_RIGHT)) framesSpeed++;
        else if (IsKeyPressed(KEY_LEFT)) framesSpeed--;

        if (framesSpeed > MAX_FRAME_SPEED) framesSpeed = MAX_FRAME_SPEED;
        else if (framesSpeed < MIN_FRAME_SPEED) framesSpeed = MIN_FRAME_SPEED;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(scarfy, 15, 40, WHITE);
        DrawRectangleLines(15, 40, scarfy.width, scarfy.height, LIME);
        DrawRectangleLines(15 + (int)frameRec.x, 40 + (int)frameRec.y, (int)frameRec.width, (int)frameRec.height, RED);

        DrawText("FRAME SPEED: ", 165, 210, 10, DARKGRAY);
        DrawText(TextFormat("%02i FPS", framesSpeed), 575, 210, 10, DARKGRAY);
        DrawText("PRESS RIGHT/LEFT KEYS to CHANGE SPEED!", 290, 240, 10, DARKGRAY);

        for (int i = 0; i < MAX_FRAME_SPEED; i++)
        {
            if (i < framesSpeed) DrawRectangle(250 + 21 * i, 205, 20, 20, RED);
            //DrawRectangleLines(250 + 21 * i, 205, 20, 20, MAROON);
        }

        DrawTextureRec(scarfy, frameRec, position, WHITE);  // Draw part of the texture

        DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(scarfy);       // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

