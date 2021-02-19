#include "raylib.h"
#include <math.h>

int getBrightness(Image* image, int x, int y)
{
    int size = image->height * image->width;
    int actualIndex = image->width * y + x;
    unsigned char result = 0;

    if (actualIndex < size && actualIndex >= 0)
        result = ((unsigned char*)image->data)[actualIndex];
    
    return result;
}

int main(void)
{
    // Initialization
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "image levels visualiser");
    SetTargetFPS(60);
    //ToggleFullscreen();

    Image mainImage = LoadImage("input_photo.png");
    Image grayImage = ImageCopy(mainImage);
    ImageColorGrayscale(&grayImage);

    int arrSize = mainImage.width * mainImage.height;
    Image grayRoberts = ImageCopy(grayImage);

    // Processing images
    
    int values[3][3] = {}, G, G1, G2;
    int index;

    for (int y = 0; y < grayImage.height; y++)
    {
        for (int x = 0; x < grayImage.width; x++)
        {
            index = grayImage.width * y + x;

            values[0][0] = getBrightness(&grayImage, x, y);
            values[0][1] = getBrightness(&grayImage, x+1, y);
            values[1][0] = getBrightness(&grayImage, x, y+1);;
            values[1][1] = getBrightness(&grayImage, x+1, y+1);;

            G1 = values[0][0] - values[1][1];
            G2 = values[0][1] - values[1][0];

            G = sqrt(pow(G1, 2) + pow(G2, 2));

            if (G < 0) G = 0;
            if (G > 255) G = 255;

            ((unsigned char*)grayRoberts.data)[index] = G;

        }
    }

    Image grayPrevite = ImageCopy(grayImage);
    int operatorPreviteX[3][3] = { { 1, 1, 1},
                                   { 0, 0, 0}, 
                                   {-1,-1,-1}};

    int operatorPreviteY[3][3] = { {-1, 0, 1},
                                   {-1, 0, 1},
                                   {-1, 0, 1}};

    for (int y = 0; y < grayImage.height; y++)
    {
        for (int x = 0; x < grayImage.width; x++)
        {
            index = grayImage.width * y + x;
            G1 = 0;
            G2 = 0;

            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++)
                    G1 += operatorPreviteX[i + 1][j + 1] * getBrightness(&grayImage, x + i, y + j);
                    
            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++)
                    G2 += operatorPreviteY[i + 1][j + 1] * getBrightness(&grayImage, x + i, y + j);

            G = sqrt(pow(G1, 2) + pow(G2, 2));

            if (G < 0) G = 0;
            if (G > 255) G = 255;

            ((unsigned char*)grayPrevite.data)[index] = G;
        }
    }

    Image graySobel = ImageCopy(grayImage);
    int operatorSobelX[3][3] = { { 1, 2, 1},
                                 { 0, 0, 0},
                                 {-1,-2,-1} };

    int operatorSobelY[3][3] = { {1, 0, -1},
                                 {2, 0, -2},
                                 {1, 0, -1} };

    for (int y = 0; y < grayImage.height; y++)
    {
        for (int x = 0; x < grayImage.width; x++)
        {
            index = grayImage.width * y + x;
            G1 = 0;
            G2 = 0;

            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++)
                    G1 += operatorSobelX[i + 1][j + 1] * getBrightness(&grayImage, x + i, y + j);

            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++)
                    G2 += operatorSobelY[i + 1][j + 1] * getBrightness(&grayImage, x + i, y + j);

            G = sqrt(pow(G1, 2) + pow(G2, 2));

            if (G < 0) G = 0;
            if (G > 255) G = 255;

            ((unsigned char*)graySobel.data)[index] = G;
        }
    }

    Image grayKirsh = ImageCopy(grayImage);
    int operatorKirshX[3][3] = { { 0, 1, 1},
                                 {-1, 0, 1},
                                 {-1,-1, 0} };

    int operatorKirshY[3][3] = { {1, 1,  0},
                                 {1, 0, -1},
                                 {0,-1, -1} };

    for (int y = 0; y < grayImage.height; y++)
    {
        for (int x = 0; x < grayImage.width; x++)
        {
            index = grayImage.width * y + x;
            G1 = 0;
            G2 = 0;

            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++)
                    G1 += operatorKirshX[i + 1][j + 1] * getBrightness(&grayImage, x + i, y + j);

            for (int i = -1; i < 2; i++)
                for (int j = -1; j < 2; j++)
                    G2 += operatorKirshY[i + 1][j + 1] * getBrightness(&grayImage, x + i, y + j);

            G = sqrt(pow(G1, 2) + pow(G2, 2));

            if (G < 0) G = 0;
            if (G > 255) G = 255;

            ((unsigned char*)grayKirsh.data)[index] = G;
        }
    }

    Texture mainTexture = LoadTextureFromImage(mainImage);
    Texture grayTexture = LoadTextureFromImage(grayImage);
    Texture grayRobertsTexture = LoadTextureFromImage(grayRoberts);
    Texture grayPreviteTexture = LoadTextureFromImage(grayPrevite);
    Texture graySobelTexture = LoadTextureFromImage(graySobel);
    Texture grayKirshTexture = LoadTextureFromImage(grayKirsh);

    Camera2D camera = { 0 };
    camera.target = { screenWidth / 2, screenHeight / 2 };
    camera.offset = { screenWidth / 2, screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    Vector2 cursor = GetMousePosition();
    Vector2 lowerCorner = { 20, mainTexture.height*3 + 20 };


    // Main loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Cursor moving
        cursor = GetMousePosition();

        if (cursor.x < 50 && camera.target.x > 100) camera.target.x -= 10;
        if (cursor.x > screenWidth - 50 && camera.target.x < mainTexture.width + grayTexture.width) camera.target.x += 10;
        if (cursor.y < 50 && camera.target.y > 100) camera.target.y -= 10;
        if (cursor.y > screenHeight - 50 && camera.target.y < lowerCorner.y) camera.target.y += 10;

        // Camera zoom controls
        camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

        if (camera.zoom > 5.0f) camera.zoom = 5.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // Background
        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(DARKGRAY);
        DrawRectangle(0, 0, mainTexture.width + grayTexture.width + 100, lowerCorner.y, GRAY);

        // Pictures
        DrawTexture(mainTexture, 20, 20, WHITE);
        DrawTexture(grayTexture, mainTexture.width + 50, 20, WHITE); 
        DrawTexture(grayRobertsTexture, 20, mainTexture.height + 20, WHITE);
        DrawTexture(grayPreviteTexture, mainTexture.width + 50, mainTexture.height + 20, WHITE);
        DrawTexture(graySobelTexture, 20, mainTexture.height * 2 + 20, WHITE);
        DrawTexture(grayKirshTexture, mainTexture.width + 50, mainTexture.height * 2 + 20, WHITE);

        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}