#define _CRT_SECURE_NO_WARNINGS
#define RAYGUI_IMPLEMENTATION

#include "PerlinNoise.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


#include "raygui.h"
#include "raylib.h"
#include "TerrainGenerator.h"

//int main(void)
//{
//    // Initialization
//    //--------------------------------------------------------------------------------------
//    const int screenWidth = 800;
//    const int screenHeight = 600;
//
//    const int cornerX = 200;
//    const int cornerY = 200;
//
//    const int pixelsPerCell = 32;
//    const int height = 10;
//    const int width = 10;
//
//    int cellByX, cellByY;
//
//    bool obstacles[height][width] = {};
//
//    bool buildMode = false;
//    Vector2 corner = { cornerX, cornerY };
//    Vector2 rectangleSize = { height * pixelsPerCell, width * pixelsPerCell };
//
//    InitWindow(screenWidth, screenHeight, "raylib [core] simple map");
//
//    /*Camera2D camera = { 0 };
//    camera.target = { screenWidth / 2, screenHeight / 2 };
//    camera.offset = { screenWidth / 2, screenHeight / 2 };
//    camera.rotation = 0.0f;
//    camera.zoom = 1.0f;*/
//    
//    //??????? ??????????? ??????: ?? ????? ~50 ???????? ?? ?????? ?? ???? ???????? ????
//    
//    //???????, ???????? "?????????????" ? ???????????? ????? ??????, ? ????? ?? ???????????? ?????????? ?????/?????????
//
//    Vector2 cursor = GetMousePosition();
//
//
//    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
//    //--------------------------------------------------------------------------------------
//
//    // Main game loop
//    while (!WindowShouldClose())        // Detect window close button or ESC key
//    {
//        BeginDrawing();
//        
//        // Background
//        ClearBackground(DARKGRAY);
//        DrawRectangleV(corner, rectangleSize, RAYWHITE);
//        for (int i = 1; i < height; i++)
//        {
//            DrawLine(cornerX, cornerY + 32 * i, cornerX + rectangleSize.x, cornerY + 32 * i, GREEN);
//        }
//
//        for (int j = 1; j < width; j++)
//        {
//            DrawLine(cornerX+pixelsPerCell*j, cornerY, cornerX + pixelsPerCell * j, cornerY+rectangleSize.y, GREEN);
//        }
//
//        //drawing non-empty cells
//        for (int i = 0; i < height; i++)
//        {
//            for (int j = 0; j < width; j++)
//            {
//                if (obstacles[i][j])
//                {
//                    DrawRectangle(i * pixelsPerCell + cornerX, j * pixelsPerCell + corner.y, 32, 32, BLACK);
//                }
//            }
//        }
//
//        if (IsKeyPressed(KEY_E))
//        {
//            if (buildMode) buildMode = false;
//            else buildMode = true;
//        }
//
//        if (buildMode)
//        {
//            // drawing cursor
//            cursor = GetMousePosition();
//            if (cursor.x > cornerX && cursor.x< cornerX + rectangleSize.x && cursor.y> cornerY && cursor.y < cornerY + rectangleSize.y)
//            {
//                cellByX = (cursor.x - cornerX) / pixelsPerCell;
//                cellByY = (cursor.y - cornerY) / pixelsPerCell;
//
//                if (obstacles[cellByX][cellByY])
//                {
//                    //if build can't be placed
//                    DrawRectangle(cellByX * pixelsPerCell + cornerX, cellByY * pixelsPerCell + corner.y, 32, 32, RED);
//                }
//                else
//                {
//                    //if there's no obstacle
//                    DrawRectangle(cellByX * pixelsPerCell + cornerX, cellByY * pixelsPerCell + corner.y, 32, 32, GREEN);
//                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
//                    {
//                        obstacles[cellByX][cellByY] = true;
//                        buildMode = false;
//                    }
//                }
//            }
//            DrawText("BUILD MODE ON", 540, 10, 20, RED);
//        }
//        else
//        {
//            DrawText("BUILD MODE OFF", 540, 10, 20, RED);
//        }
//        
//        /*
//        DrawRectangle(cursor.x, cursor.y, 32, 32, BLUE);*/
//
//        EndDrawing();
//    }
//
//    // De-Initialization
//    //--------------------------------------------------------------------------------------
//    CloseWindow();        // Close window and OpenGL context
//    //--------------------------------------------------------------------------------------
//
//    return 0;
//}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib [core] map generation");

    SetTargetFPS(60);

    const int cornerX = 200;
    const int cornerY = 200;

  //  const int height = 512;
  //  const int width = 512;

  //  int cellByX, cellByY;

  //  bool obstacles[height][width] = {};

  //  bool buildMode = false;
  ///*  Vector2 corner = { cornerX, cornerY };
  //  Vector2 rectangleSize = { height * pixelsPerCell, width * pixelsPerCell };*/

  //  Color* pixels = new Color[height * width];

  //  double frequency = 8;
  //  int octaves = 8;

  //  const siv::PerlinNoise perlin(48454);
  //  const double fx = width / frequency;
  //  const double fy = height / frequency;
  //  double noise;
  //  Color tileColor;

  //  int index = 0;
  //  
  //  for (int y = 0; y < height; ++y)
  //  {
  //      for (int x = 0; x < width; ++x)
  //      {
  //          index = width * y + x;
  //          noise = perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves);

  //          if (noise > 0.5)
  //          {
  //              if (noise > 0.6)
  //              {
  //                  if (noise > 0.7)
  //                  {
  //                      tileColor = BLACK;
  //                  }
  //                  else
  //                  {
  //                      tileColor = DARKGRAY;
  //                  }
  //              }
  //              else
  //              {
  //                  tileColor = DARKGREEN;
  //              }
  //          }
  //          else
  //          {
  //              if (noise > 0.4)
  //              {
  //                  if (noise > 0.45)
  //                  {
  //                      tileColor = YELLOW;
  //                  }
  //                  else
  //                  {
  //                      tileColor = BLUE;
  //                  }
  //              }
  //              else
  //              {
  //                  tileColor = DARKBLUE;
  //              }
  //          }

  //          pixels[index] = tileColor;
  //          
  //      }
  //  }

  //  Image preview = {
  //      pixels,
  //      width,
  //      height,
  //      1,
  //      UNCOMPRESSED_R8G8B8A8
  //  };

  //  Texture2D texture = LoadTextureFromImage(preview);

  //  delete[] pixels;

    TerrainGenerator terrain{};
    
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        BeginDrawing();

        // Background
        ClearBackground(DARKGRAY);
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