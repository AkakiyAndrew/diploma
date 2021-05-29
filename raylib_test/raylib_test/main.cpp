////#include "raylib.h"
////
////#define MAX_BUILDINGS   100
////
////int main()
////{
////    // Initialization
////    //--------------------------------------------------------------------------------------
////    const int screenWidth = 800;
////    const int screenHeight = 450;
////
////    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");
////
////    Rectangle player = { 400, 280, 40, 40 };
////    Rectangle buildings[MAX_BUILDINGS] = { 0 };
////    Color buildColors[MAX_BUILDINGS] = { 0 };
////
////    int spacing = 0;
////
////    for (int i = 0; i < MAX_BUILDINGS; i++)
////    {
////        buildings[i].width = GetRandomValue(50, 200);
////        buildings[i].height = GetRandomValue(100, 800);
////        buildings[i].y = screenHeight - 130 - buildings[i].height;
////        buildings[i].x = -6000 + spacing;
////
////        spacing += buildings[i].width;
////
////        buildColors[i] = { (unsigned char)GetRandomValue(200, 240), (unsigned char)GetRandomValue(200, 240), (unsigned char)GetRandomValue(200, 250), 255 };
////    }
////
////    Camera2D camera = { 0 };
////    camera.target = { player.x + 20, player.y + 20 };
////    camera.offset = { screenWidth / 2, screenHeight / 2 };
////    camera.rotation = 0.0f;
////    camera.zoom = 1.0f;
////    Vector2 cursor = GetMousePosition();
////
////
////    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
////    //--------------------------------------------------------------------------------------
////
////    // Main game loop
////    while (!WindowShouldClose())        // Detect window close button or ESC key
////    {
////        // Update
////        //----------------------------------------------------------------------------------
////
////        // Player movement
////        if (IsKeyDown(KEY_RIGHT)) player.x += 2;
////        else if (IsKeyDown(KEY_LEFT)) player.x -= 2;
////
////        // CUSTOM: MOVE VIA CURSOR POSITION
////        cursor = GetMousePosition();
////        if (cursor.x < screenWidth / 4) player.x -= 6;
////        else if (cursor.x > (screenWidth * 3) / 4) player.x += 6;
////
////        // Camera target follows player
////        camera.target = { player.x + 20, player.y + 20 };
////
////        // Camera rotation controls
////        if (IsKeyDown(KEY_A)) camera.rotation--;
////        else if (IsKeyDown(KEY_S)) camera.rotation++;
////
////        // Limit camera rotation to 80 degrees (-40 to 40)
////        if (camera.rotation > 40) camera.rotation = 40;
////        else if (camera.rotation < -40) camera.rotation = -40;
////
////        // Camera zoom controls
////        camera.zoom += ((float)GetMouseWheelMove() * 0.05f);
////
////        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
////        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;
////
////        // Camera reset (zoom and rotation)
////        if (IsKeyPressed(KEY_R))
////        {
////            camera.zoom = 1.0f;
////            camera.rotation = 0.0f;
////        }
////        //----------------------------------------------------------------------------------
////
////        // Draw
////        //----------------------------------------------------------------------------------
////        BeginDrawing();
////
////        ClearBackground(RAYWHITE);
////
////        BeginMode2D(camera);
////
////        DrawRectangle(-6000, 320, 13000, 8000, DARKGRAY);
////
////        for (int i = 0; i < MAX_BUILDINGS; i++) DrawRectangleRec(buildings[i], buildColors[i]);
////
////        DrawRectangleRec(player, RED);
////
////        DrawLine(camera.target.x, -screenHeight * 10, camera.target.x, screenHeight * 10, GREEN);
////        DrawLine(-screenWidth * 10, camera.target.y, screenWidth * 10, camera.target.y, GREEN);
////
////        EndMode2D();
////
////        DrawText("SCREEN AREA", 640, 10, 20, RED);
////        // CUSTOM: DRAW CURSOR POSITION
////        DrawText(TextFormat("Cursor position: %f,%f", cursor.x, cursor.y), 400, 30, 20, RED);
////
////        DrawRectangle(0, 0, screenWidth, 5, RED);
////        DrawRectangle(0, 5, 5, screenHeight - 10, RED);
////        DrawRectangle(screenWidth - 5, 5, 5, screenHeight - 10, RED);
////        DrawRectangle(0, screenHeight - 5, screenWidth, 5, RED);
////
////        DrawRectangle(10, 10, 250, 113, Fade(SKYBLUE, 0.5f));
////        DrawRectangleLines(10, 10, 250, 113, BLUE);
////
////        // окошко слева сверху
////        DrawText("Free 2d camera controls:", 20, 20, 10, BLACK);
////        DrawText("- Right/Left to move Offset", 40, 40, 10, DARKGRAY);
////        DrawText("- Mouse Wheel to Zoom in-out", 40, 60, 10, DARKGRAY);
////        DrawText("- A / S to Rotate", 40, 80, 10, DARKGRAY);
////        DrawText("- R to reset Zoom and Rotation", 40, 100, 10, DARKGRAY);
////
////        EndDrawing();
////        //----------------------------------------------------------------------------------
////    }
////
////    // De-Initialization
////    //--------------------------------------------------------------------------------------
////    CloseWindow();        // Close window and OpenGL context
////    //--------------------------------------------------------------------------------------
////
////    return 0;
////}
//
//
//#include "raylib.h"
//
//#define MAX_FRAME_SPEED     60
//#define MIN_FRAME_SPEED      1
//
//int main(void)
//{
//    // Initialization
//    //--------------------------------------------------------------------------------------
//    const int screenWidth = 1400;
//    const int screenHeight = 1200;
//
//    InitWindow(screenWidth, screenHeight, "raylib [texture] example - texture rectangle");
//
//    const char* filenames[15] = { 
//        "0.png", "1.png", "2.png", "3.png", "4.png", 
//        "5.png", "6.png", "7.png", "8.png", "9.png", 
//        "10.png", "11.png", "12.png", "13.png", "14.png", };
//
//    Texture2D frames[15];
//
//    for (int i = 0; i < 15; i++)
//    {
//        frames[i] = LoadTexture(TextFormat("resources/%s", filenames[i]));
//    }
//
//    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
//    //Texture2D scarfy = LoadTexture("resources/scarfy.png");        // Texture loading
//
//    Vector2 position = { 350.0f, 280.0f };
//    Rectangle frameRec = { 0.0f, 0.0f, (float)frames[0].width / 6, (float)frames[0].height };
//    int currentFrame = 0;
//
//    int framesCounter = 0;
//    int framesSpeed = 8;            // Number of spritesheet frames shown by second
//
//    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
//    //--------------------------------------------------------------------------------------
//
//    // Main game loop
//    while (!WindowShouldClose())    // Detect window close button or ESC key
//    {
//        // Update
//        //----------------------------------------------------------------------------------
//        framesCounter++;
//
//        if (framesCounter >= (60 / framesSpeed))
//        {
//            framesCounter = 0;
//            currentFrame++;
//
//            if (currentFrame > 14) currentFrame = 0;
//        }
//
//        if (IsKeyPressed(KEY_RIGHT)) framesSpeed++;
//        else if (IsKeyPressed(KEY_LEFT)) framesSpeed--;
//
//        if (framesSpeed > MAX_FRAME_SPEED) framesSpeed = MAX_FRAME_SPEED;
//        else if (framesSpeed < MIN_FRAME_SPEED) framesSpeed = MIN_FRAME_SPEED;
//        //----------------------------------------------------------------------------------
//
//        // Draw
//        //----------------------------------------------------------------------------------
//        BeginDrawing();
//
//        ClearBackground(RAYWHITE);
//
//        DrawTexture(frames[currentFrame], 15, 40, WHITE);
//
//        //DrawText("FRAME SPEED: ", 165, 210, 10, DARKGRAY);
//        //DrawText(TextFormat("%02i FPS", framesSpeed), 575, 210, 10, DARKGRAY);
//        //DrawText("PRESS RIGHT/LEFT KEYS to CHANGE SPEED!", 290, 240, 10, DARKGRAY);
//
//        //for (int i = 0; i < MAX_FRAME_SPEED; i++)
//        //{
//        //    if (i < framesSpeed) DrawRectangle(250 + 21 * i, 205, 20, 20, RED);
//        //    DrawRectangleLines(250 + 21 * i, 205, 20, 20, MAROON);
//        //}
//
//        EndDrawing();
//        //----------------------------------------------------------------------------------
//    }
//
//    // De-Initialization
//    //--------------------------------------------------------------------------------------
//    for (int i = 0; i < 15; i++)
//    {
//        UnloadTexture(frames[i]);
//    }
//
//    CloseWindow();                // Close window and OpenGL context
//    //--------------------------------------------------------------------------------------
//
//    return 0;
//}


/*******************************************************************************************
*
*   raylib [shaders] example - Texture Waves
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3), to test this example
*         on OpenGL ES 2.0 platforms (Android, Raspberry Pi, HTML5), use #version 100 shaders
*         raylib comes with shaders ready for both versions, check raylib/shaders install folder
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Example contributed by Anata (@anatagawa) and reviewed by Ramon Santamaria (@raysan5)
*
*   Copyright (c) 2019 Anata (@anatagawa) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1024;
    const int screenHeight = 1024;

    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - texture waves");

    // Load texture texture to apply shaders
    Texture2D texture = LoadTexture("resources/creep_better.jpg");

    // Load shader and setup location points and values
    Shader shader = LoadShader(0, TextFormat("resources/shaders/glsl%i/wave.fs", GLSL_VERSION));

    int secondsLoc = GetShaderLocation(shader, "secondes");
    int freqXLoc = GetShaderLocation(shader, "freqX");
    int freqYLoc = GetShaderLocation(shader, "freqY");
    int ampXLoc = GetShaderLocation(shader, "ampX");
    int ampYLoc = GetShaderLocation(shader, "ampY");
    int speedXLoc = GetShaderLocation(shader, "speedX");
    int speedYLoc = GetShaderLocation(shader, "speedY");

    // Shader uniform values that can be updated at any time
    float freqX = 30.0f;
    float freqY = 30.0f;
    float ampX = 5.0f;
    float ampY = 5.0f;
    float speedX = 1.0f;
    float speedY = 1.0f;

    float screenSize[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
    SetShaderValue(shader, GetShaderLocation(shader, "size"), &screenSize, UNIFORM_VEC2);
    SetShaderValue(shader, freqXLoc, &freqX, UNIFORM_FLOAT);
    SetShaderValue(shader, freqYLoc, &freqY, UNIFORM_FLOAT);
    SetShaderValue(shader, ampXLoc, &ampX, UNIFORM_FLOAT);
    SetShaderValue(shader, ampYLoc, &ampY, UNIFORM_FLOAT);
    SetShaderValue(shader, speedXLoc, &speedX, UNIFORM_FLOAT);
    SetShaderValue(shader, speedYLoc, &speedY, UNIFORM_FLOAT);

    float seconds = 0.0f;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    // -------------------------------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        seconds += GetFrameTime();

        SetShaderValue(shader, secondsLoc, &seconds, UNIFORM_FLOAT);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginShaderMode(shader);

        DrawTexture(texture, 0, 0, WHITE);
        //DrawTexture(texture, texture.width, 0, WHITE);

        EndShaderMode();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(shader);         // Unload shader
    UnloadTexture(texture);       // Unload texture

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
