//
///*******************************************************************************************
//*
//*   raylib [shaders] example - Texture Waves
//*
//*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
//*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
//*
//*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3), to test this example
//*         on OpenGL ES 2.0 platforms (Android, Raspberry Pi, HTML5), use #version 100 shaders
//*         raylib comes with shaders ready for both versions, check raylib/shaders install folder
//*
//*   This example has been created using raylib 2.5 (www.raylib.com)
//*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
//*
//*   Example contributed by Anata (@anatagawa) and reviewed by Ramon Santamaria (@raysan5)
//*
//*   Copyright (c) 2019 Anata (@anatagawa) and Ramon Santamaria (@raysan5)
//*
//********************************************************************************************/
//
//#include "raylib.h"
//
//#if defined(PLATFORM_DESKTOP)
//#define GLSL_VERSION            330
//#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
//#define GLSL_VERSION            100
//#endif
//
//int main(void)
//{
//    // Initialization
//    //--------------------------------------------------------------------------------------
//    const int screenWidth = 1024;
//    const int screenHeight = 1024;
//
//    InitWindow(screenWidth, screenHeight, "raylib [shaders] example - texture waves");
//
//    // Load texture texture to apply shaders
//    Texture2D texture = LoadTexture("resources/creep_better.jpg");
//
//    // Load shader and setup location points and values
//    Shader shader = LoadShader(0, TextFormat("resources/shaders/glsl%i/wave.fs", GLSL_VERSION));
//
//    int secondsLoc = GetShaderLocation(shader, "secondes");
//    int freqXLoc = GetShaderLocation(shader, "freqX");
//    int freqYLoc = GetShaderLocation(shader, "freqY");
//    int ampXLoc = GetShaderLocation(shader, "ampX");
//    int ampYLoc = GetShaderLocation(shader, "ampY");
//    int speedXLoc = GetShaderLocation(shader, "speedX");
//    int speedYLoc = GetShaderLocation(shader, "speedY");
//
//    // Shader uniform values that can be updated at any time
//    float freqX = 30.0f;
//    float freqY = 30.0f;
//    float ampX = 5.0f;
//    float ampY = 5.0f;
//    float speedX = 1.0f;
//    float speedY = 1.0f;
//
//    float screenSize[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
//    SetShaderValue(shader, GetShaderLocation(shader, "size"), &screenSize, UNIFORM_VEC2);
//    SetShaderValue(shader, freqXLoc, &freqX, UNIFORM_FLOAT);
//    SetShaderValue(shader, freqYLoc, &freqY, UNIFORM_FLOAT);
//    SetShaderValue(shader, ampXLoc, &ampX, UNIFORM_FLOAT);
//    SetShaderValue(shader, ampYLoc, &ampY, UNIFORM_FLOAT);
//    SetShaderValue(shader, speedXLoc, &speedX, UNIFORM_FLOAT);
//    SetShaderValue(shader, speedYLoc, &speedY, UNIFORM_FLOAT);
//
//    float seconds = 0.0f;
//
//    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
//    // -------------------------------------------------------------------------------------------------------------
//
//    // Main game loop
//    while (!WindowShouldClose())    // Detect window close button or ESC key
//    {
//        // Update
//        //----------------------------------------------------------------------------------
//        seconds += GetFrameTime();
//
//        SetShaderValue(shader, secondsLoc, &seconds, UNIFORM_FLOAT);
//        //----------------------------------------------------------------------------------
//
//        // Draw
//        //----------------------------------------------------------------------------------
//        BeginDrawing();
//
//        ClearBackground(RAYWHITE);
//
//        BeginShaderMode(shader);
//
//        DrawTexture(texture, 0, 0, WHITE);
//        //DrawTexture(texture, texture.width, 0, WHITE);
//
//        EndShaderMode();
//
//        EndDrawing();
//        //----------------------------------------------------------------------------------
//    }
//
//    // De-Initialization
//    //--------------------------------------------------------------------------------------
//    UnloadShader(shader);         // Unload shader
//    UnloadTexture(texture);       // Unload texture
//
//    CloseWindow();              // Close window and OpenGL context
//    //--------------------------------------------------------------------------------------
//
//    return 0;
//}


#include "raylib.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] examples - texture source and destination rectangles");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    Texture2D scarfy = LoadTexture("resources/scarfy.png");        // Texture loading

    int frameWidth = scarfy.width / 6;
    int frameHeight = scarfy.height;

    // Source rectangle (part of the texture to use for drawing)
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };

    // Destination rectangle (screen rectangle where drawing part of texture)
    Rectangle destRec = { screenWidth / 2.0f, screenHeight / 2.0f, frameWidth * 2.0f, frameHeight * 2.0f };

    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    Vector2 origin = { (float)frameWidth, (float)frameHeight };

    int rotation = 0;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        rotation++;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // NOTE: Using DrawTexturePro() we can easily rotate and scale the part of the texture we draw
        // sourceRec defines the part of the texture we use for drawing
        // destRec defines the rectangle where our texture part will fit (scaling it to fit)
        // origin defines the point of the texture used as reference for rotation and scaling
        // rotation defines the texture rotation (using origin as rotation point)
        DrawTexturePro(scarfy, sourceRec, destRec, origin, (float)rotation, WHITE);

        DrawLine((int)destRec.x, 0, (int)destRec.x, screenHeight, GRAY);
        DrawLine(0, (int)destRec.y, screenWidth, (int)destRec.y, GRAY);

        DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(scarfy);        // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}