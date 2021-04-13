#include "raylib.h"
#include <math.h>

int main(void)
{
    // Initialization
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "image levels visualiser");
    SetTargetFPS(60);
    ToggleFullscreen();

    Image mainImage = LoadImage("input.png");
    Image grayImage = ImageCopy(mainImage);

    ImageColorGrayscale(&grayImage);
    //ImageFormat(&grayImage, UNCOMPRESSED_GRAY_ALPHA);

    Texture mainTexture = LoadTextureFromImage(mainImage);
    Texture grayTexture = LoadTextureFromImage(grayImage);

    Camera2D camera = { 0 };
    camera.target = { screenWidth / 2, screenHeight / 2 };
    camera.offset = { screenWidth / 2, screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    Vector2 cursor = GetMousePosition();

    // Calculating levels
    float levels[256] = {};
    float maxNum = 0, sum = 0;
    float levelsNormalised[256] = {};
    int arrSize = mainImage.width * mainImage.height;
    
    Color* grayData = LoadImageColors(mainImage);
    int brightness;

    for (int i = 0; i < arrSize; i++)
    {
        brightness = grayData[i].r * 0.299f + grayData[i].g * 0.587f + grayData[i].b * 0.114f;
        sum += brightness;
        levels[brightness]++;
    }

    for (int i = 0; i < 256; i++) if (levels[i] > maxNum) maxNum = levels[i];

    for (int i = 0; i < 256; i++) levelsNormalised[i] = levels[i] / maxNum;

    // Statistic
    float grayMean = sum / arrSize;
    float contrast = 0;
    for (int i = 0; i < arrSize; i++)
    {
        brightness = grayData[i].r * 0.299f + grayData[i].g * 0.587f + grayData[i].b * 0.114f;
        contrast += pow(brightness - grayMean, 2);
    }
    contrast = sqrt(contrast/arrSize);
    float entropy = 0;
    float buf;
    for (int i = 0; i < 256; i++)
    {
        buf = levels[i] / arrSize;
        if(buf!=0) entropy += buf * log2(buf);
    }
    entropy *= -1;

    // Histogram position
    int histHeight = 200;
    Vector2 histZeropoint = { 40, mainTexture.height + 120 + histHeight };
    Vector2 histOrdinate = { histZeropoint.x, histZeropoint.y-(histHeight+50)};
    Vector2 histAxis = { histZeropoint.x+300, histZeropoint.y};

    Vector2* dots = new Vector2[256];

    for (int i = 0; i < 256; i++)
    {
        int x = i + 1 + histZeropoint.x;
        int y = histZeropoint.y - (levelsNormalised[i] * histHeight);
        dots[i].x = x;
        dots[i].y = y-1;
        //DrawPixel(x, y, BLACK);
    }

    // Main loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Cursor moving
        cursor = GetMousePosition();

        if (cursor.x < 50 && camera.target.x > 100) camera.target.x -= 10;
        if (cursor.x > screenWidth - 50 && camera.target.x < mainTexture.width + grayTexture.width) camera.target.x += 10;
        if (cursor.y < 50 && camera.target.y > 100) camera.target.y -= 10;
        if (cursor.y > screenHeight - 50 && camera.target.y < histZeropoint.y) camera.target.y += 10;

        // Camera zoom controls
        camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

        if (camera.zoom > 5.0f) camera.zoom = 5.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // Background
        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(DARKGRAY);
        DrawRectangle(0, 0, mainTexture.width + grayTexture.width + 100, histZeropoint.y+50, GRAY);

        // Pictures
        DrawTexture(mainTexture, 20, 20, WHITE);
        DrawTexture(grayTexture, mainTexture.width+50, 20, WHITE);

        // Histogram
        DrawLineEx(histZeropoint, histOrdinate, 2, BLACK);
        DrawLineEx({ histOrdinate.x + 7, histOrdinate.y + 7 }, histOrdinate, 2, BLACK);
        DrawLineEx({ histOrdinate.x - 7, histOrdinate.y + 7 }, histOrdinate, 2, BLACK);
        DrawLineEx(histZeropoint, histAxis, 2, BLACK);
        DrawLineEx({ histAxis.x - 7, histAxis.y - 7 }, histAxis, 2, BLACK);
        DrawLineEx({ histAxis.x - 7, histAxis.y + 7 }, histAxis, 2, BLACK);
        DrawLineStrip(dots, 256, GREEN);

        // Legend 0X
        DrawText("0", histZeropoint.x-10, histZeropoint.y, 15, BLACK);
        DrawText("255", histZeropoint.x + 245, histZeropoint.y + 5, 15, BLACK);
        // Legend 0Y
        DrawText("0.25", histZeropoint.x - 35, histZeropoint.y-(histHeight/4), 15, BLACK);
        DrawText("0.5", histZeropoint.x - 30, histZeropoint.y - (histHeight / 2), 15, BLACK);
        DrawText("0.75", histZeropoint.x - 35, histZeropoint.y - (histHeight * 3 / 4), 15, BLACK);
        DrawText("1.0", histZeropoint.x - 30, histZeropoint.y - histHeight , 15, BLACK);
        
        // Statistic
        DrawText(FormatText("Mean: %f", grayMean), mainTexture.width, histZeropoint.y-histHeight, 15, BLACK);
        DrawText(FormatText("Contrast: %f", contrast), mainTexture.width, histZeropoint.y - histHeight + 70, 15, BLACK);
        DrawText(FormatText("Entropy: %f", entropy), mainTexture.width, histZeropoint.y - histHeight + 140, 15, BLACK);

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}