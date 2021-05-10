#include "TerrainGenerator.h"
#include <omp.h>

//»—œ–¿¬»“‹ » ƒŒ¡¿¬»“‹ ÕŒ¬€≈  ŒÃÃ≈Õ“¿–»» (Ì‡ ËÌ„ÎË¯Â!)

TerrainGenerator::TerrainGenerator()
{
    closed = false;

    levelNames = new const char* [] { "LAKE", "SWAMP", "SAND", "PLAIN", "TREE", "STONE", "MOUNTAIN" };
    levels = new float[] { 0., 0.35, 0.4, 0.45, 0.5, 0.6, 0.70, 1. };
    palette = new Color[]{ DARKBLUE, BLUE,	YELLOW, GREEN,	DARKGREEN,	GRAY,	BLACK };

    //TODO: make preset maps (empty map, for path finding etc.)
    width = 128, height = 128, seed = 12345, octaves = 16;
    frequency = 16.;

    //TODO: replace memory allocation to regenerate/rerender
}

void TerrainGenerator::DrawInterface()
{
    closed = GuiButton(Rectangle{ 20.f, 20.f, 100.f, 50.f }, "Back") || IsKeyPressed(KEY_ESCAPE);

    if (isGenerated())
    {
        DrawTextureEx(
            colorPreview, 
            Vector2{ 50,100 }, 
            0, (GetScreenWidth() * 0.25f) / static_cast<float>(colorPreview.width),
            WHITE);
        DrawTextureEx(
            grayPreview,
            Vector2{ (GetScreenWidth() * 0.25f) + 100, 100 },
            0, (GetScreenWidth() * 0.25f) / static_cast<float>(grayPreview.width), WHITE);
    }
    else
    {
        RegenerateTerrain();
        RerenderTerrain();
    }

    //REPLACE IN CONSTRUCTOR PARAMETERS(??)
    Vector2 guiZeroPoint = { GetScreenWidth()*0.75, 0 };

    //drawing levels sliders and retriving values from them
    Rectangle position;
    std::string buf;
    for (int i = 1; i < 7; i++) // 7 - `cause 8 terrain levels, 8-1
    {
        buf = levelNames[i] + std::string(": %.2f");
        position = { guiZeroPoint.x, guiZeroPoint.y + 35.f *i, 120, 20 };
        levels[i] = GuiSliderBar(
                        position,
                        TextFormat(buf.c_str(), levels[i-1]),
                        TextFormat("%.2f", levels[i+1]),
                        levels[i],
                        levels[i - 1],
                        levels[i+1]
                        );
    }

    //level distribution visualisation
    //TODO: solve problem with mountain visual
    for (int i = 0; i < 8; i++)
    {
        position = { levels[i] * 240.f + guiZeroPoint.x-60, guiZeroPoint.y + 250, (levels[i + 1] - levels[i]) * 300, 30.f };

        DrawText(TextFormat("%.1f", levels[i]), position.x, position.y - 15, 14, palette[i]);
        DrawRectangleRec(position, palette[i]);
    }

    octaves = GuiSliderBar(
                Rectangle{ guiZeroPoint.x, guiZeroPoint.y+ 410, 120, 20},
                "Octaves: 1",
                "16",
                octaves,
                1,
                16
                );

    frequency = GuiSliderBar(
        Rectangle{ guiZeroPoint.x, guiZeroPoint.y + 440, 120, 20 },
        "Frequency: 1.0",
        "64.0",
        frequency,
        0.1f,
        64.f
    );
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        position = { guiZeroPoint.x, guiZeroPoint.y + 370, 60, 30 };
        if (CheckCollisionPointRec(GetMousePosition(), position))
        {
            valueChanging[0] = true;
            valueChanging[1] = false;
            valueChanging[2] = false;
        }
        position = { guiZeroPoint.x, guiZeroPoint.y + 470, 60, 30 };
        if (CheckCollisionPointRec(GetMousePosition(), position))
        {
            valueChanging[0] = false;
            valueChanging[1] = true;
            valueChanging[2] = false;
        }
        position = { guiZeroPoint.x + 120, guiZeroPoint.y + 470, 60, 30 };
        if (CheckCollisionPointRec(GetMousePosition(), position))
        {
            valueChanging[0] = false;
            valueChanging[1] = false;
            valueChanging[2] = true;
        }
    }

    //numerable params
    position = { guiZeroPoint.x, guiZeroPoint.y + 370, 60, 30 };
    GuiValueBox(position, "Generation seed:", &seed, 0, INT_MAX, valueChanging[0]);

    position = { guiZeroPoint.x, guiZeroPoint.y + 470, 60, 30 };
    GuiValueBox(position, "Width:", &width, 1, 4048, valueChanging[1]);

    position = { guiZeroPoint.x + 120, guiZeroPoint.y + 470, 60, 30 };
    GuiValueBox(position, "Height:", &height, 1, 4048, valueChanging[2]);

    //TODO: lock button, if width/height was changed and not regenerated, to prevent access violation
    //Rerender button
    if (GuiButton(Rectangle{ guiZeroPoint.x, guiZeroPoint.y + 300, 150.f, 50.f }, "Rerender"))
        RerenderTerrain();

    //Regenerate button
    if (GuiButton(Rectangle{ guiZeroPoint.x, guiZeroPoint.y+ 510, 150.f, 50.f }, "Regenerate"))
    {
        RegenerateTerrain();
        RerenderTerrain();
    }

    //TODO: add random seed button (with "refresh" icon)

    //TODO: Add regenerate animation(??)

    //TODO: ADD HISTOGRAM BELOW MAP PREVIEWS
}

void TerrainGenerator::RegenerateTerrain()
{
    if (isGenerated())
        delete[] noiseMap;
    noiseMap = new double[width * height];

    int index;
    const siv::PerlinNoise perlin((unsigned int)seed);
    
    //TODO: regenerate according to chosen terrain size (with pointers)

    fx = width / frequency;
    fy = height / frequency;

#pragma omp parallel for private(index)
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            index = width * y + x;
            noiseMap[index] = perlin.accumulatedOctaveNoise2D_0_1(x / fx, y / fy, octaves);
        }
    }
}

void TerrainGenerator::RerenderTerrain()
{
    if (isGenerated())
    {
        UnloadTexture(colorPreview);
        UnloadTexture(grayPreview);
        delete[] terrainMap;
    }
    
    terrainMap = new TerrainType[width * height];

    //updating color preview
    Color* colorPixels = new Color[width * height];
    unsigned char* grayPixels = new unsigned char[width * height];

#pragma omp parallel for
    for (int i = 0; i < width * height; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            grayPixels[i] = (unsigned char)(noiseMap[i] * 255.);
            if (noiseMap[i] <= levels[j+1])
            {
                colorPixels[i] = palette[j];
                terrainMap[i] = static_cast<TerrainType>(j); 
                break;
            }
        }
    }

    Image colorImage = {
        colorPixels,
        width,
        height,
        1,
        UNCOMPRESSED_R8G8B8A8
    };
    
    Image grayImage = {
        grayPixels,
        width,
        height,
        1,
        UNCOMPRESSED_GRAYSCALE
    };

    colorPreview = LoadTextureFromImage(colorImage);
    grayPreview = LoadTextureFromImage(grayImage);

    UnloadImage(colorImage);
    UnloadImage(grayImage);
}

TerrainGenerator::~TerrainGenerator()
{
    delete[] levels;
    delete[] noiseMap;
    delete[] palette;
    delete[] levelNames;
}

bool TerrainGenerator::isGenerated()
{
    return terrainMap != nullptr;
}

Terrain TerrainGenerator::getMap()
{
    return Terrain{width, height, terrainMap};
}