#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator()
{
	width = 512, height = 512, seed = 12345, octaves = 8;
	frequency = 8.;
	fx = width / frequency;
	fy = height / frequency;

	levels = new float[] { 0., 0.35, 0.4, 0.45, 0.5, 0.6, 0.70, 1. };

	palette = new Color[]{DARKBLUE, BLUE, YELLOW, GREEN, DARKGREEN, GRAY, BLACK};

	noiseMap = new double[width * height];
}

void TerrainGenerator::DrawInterface()
{
	if (generated)
	{
		DrawTexture(colorPreview, 0, 0, WHITE);
	}
	else
	{
		RegenerateTerrain();
		RerenderTerrain();
	}

	Rectangle position; //ползунки для изменения уровней
	for (int i = 1; i < 7; i++) // 7 - т.к. 8 уровней -1
	{
		position = { 600, 35.f *i, 120, 20 };
		levels[i] = GuiSliderBar(
						position,
						TextFormat("%f", levels[i-1]),
						TextFormat("%f", levels[i+1]),
						levels[i],
						levels[i - 1],
						levels[i+1]
						);
	}

	for (int i = 0; i < 8; i++) //визуализация уровней в виде разноцветной линии
	{
		position = { levels[i]*240.f + 500.f, 350.f, (levels[i + 1] - levels[i]) *240.f, 30.f};
		DrawText(TextFormat("%.2f", levels[i]), position.x, position.y - 15, 14, palette[i]);
		DrawRectangleRec(position, palette[i]);
	}

}

void TerrainGenerator::RegenerateTerrain()
{
	int index;
	const siv::PerlinNoise perlin(seed);
	
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
	//наполнение новыми цветами
	Color* pixels = new Color[width * height];

	for (int i = 0; i < width * height; i++)
	{
		for (int j = 1; j < 7; j++)
		{
			if (noiseMap[i] <= levels[j]) //исправить проблему с последним участком уровня - скалы
			{
				pixels[i] = palette[j];
				break;
			}
		}
	}

	Image image = {
		pixels,
		width,
		height,
		1,
		UNCOMPRESSED_R8G8B8A8
	};

	colorPreview = LoadTextureFromImage(image);
	generated = true;
	delete[] pixels;
}

TerrainGenerator::~TerrainGenerator()
{
	delete[] levels;
	delete[] noiseMap;
	delete[] palette;
}