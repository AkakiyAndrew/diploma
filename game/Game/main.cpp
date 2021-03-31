//#include "raylib.h"
//
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
//    const int numOfRows = 10;
//    const int numOfColumns = 10;
//
//    int cellByX, cellByY;
//
//    bool obstacles[numOfRows][numOfColumns] = {};
//
//    bool buildMode = false;
//    Vector2 corner = { cornerX, cornerY };
//    Vector2 rectangleSize = { numOfRows * pixelsPerCell, numOfColumns * pixelsPerCell };
//
//    InitWindow(screenWidth, screenHeight, "raylib [core] simple map");
//
//    /*Camera2D camera = { 0 };
//    camera.target = { screenWidth / 2, screenHeight / 2 };
//    camera.offset = { screenWidth / 2, screenHeight / 2 };
//    camera.rotation = 0.0f;
//    camera.zoom = 1.0f;*/
//    
//    //пределы перемещения камеры: не более ~50 пикселей от камеры до края игрового поля
//    
//    //функция, выдающая "прямоугольник" с координатами углов клетки, в ответ на произвольные координаты юнита/постройки
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
//        for (int i = 1; i < numOfRows; i++)
//        {
//            DrawLine(cornerX, cornerY + 32 * i, cornerX + rectangleSize.x, cornerY + 32 * i, GREEN);
//        }
//
//        for (int j = 1; j < numOfColumns; j++)
//        {
//            DrawLine(cornerX+pixelsPerCell*j, cornerY, cornerX + pixelsPerCell * j, cornerY+rectangleSize.y, GREEN);
//        }
//
//        //drawing non-empty cells
//        for (int i = 0; i < numOfRows; i++)
//        {
//            for (int j = 0; j < numOfColumns; j++)
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