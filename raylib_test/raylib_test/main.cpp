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
#include <math.h>

int Vector2Angle(Vector2 v1, Vector2 v2)
{
    int result = atan2f(v2.y - v1.y, v2.x - v1.x) * (180.0f / PI);
    if (result < 0) result += 360.0f;
    return result;
}

float Vector2AngleFloat(Vector2 v1, Vector2 v2)
{
    float result = atan2f(v2.y - v1.y, v2.x - v1.x);
    return result;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] examples - texture source and destination rectangles");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    int angle = 0; //= Vector2Angle(center, mousePos);
    float angleTan = 0;
    Vector2 center{ 100.f, 100.f }, mousePos;

    float rotationSpeed = 0.05f;
    float destinationRotation = 0;
    int timeCount = 0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        BeginDrawing();
        
        timeCount++;
        if (timeCount > 60)
            timeCount = 0;

        mousePos = GetMousePosition();
        //angle = Vector2Angle(center, mousePos);
        //angleTan = Vector2AngleFloat(center, mousePos);

        if (timeCount % 5 == 0)
        {
            destinationRotation = atan2f(center.y - mousePos.y, center.x - mousePos.x) + PI;

            if (destinationRotation != angleTan)
            {
                if (fmaxf(destinationRotation - angleTan, angleTan - destinationRotation) < rotationSpeed)
                {
                    angleTan = destinationRotation;
                }
                else
                {
                    if (destinationRotation > angleTan)
                    {
                        if (angleTan < destinationRotation - PI)
                            angleTan -= rotationSpeed;
                        else
                            angleTan += rotationSpeed;
                    }
                    else
                    {
                        if (angleTan > destinationRotation + PI)
                            angleTan += rotationSpeed;
                        else
                            angleTan -= rotationSpeed;
                    }

                    if (angleTan > PI * 2.0f) angleTan = 0;
                    if (angleTan < 0) angleTan = PI * 2.0f;
                }
                angle = angleTan * 180.0 / PI;
            }
        }

        ClearBackground(RAYWHITE);

        DrawCircle(center.x, center.y, 16, SKYBLUE);
        DrawCircle(mousePos.x, mousePos.y, 4, RED);
        DrawLine(center.x, center.y, mousePos.x, mousePos.y, BLACK);

        DrawText(TextFormat("DestAngle: %f, currentAngle: %f, IntAngle: %d", destinationRotation, angleTan, angle), 50, 50, 18, BLACK);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}