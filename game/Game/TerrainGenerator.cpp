#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator()
{
	width = 512, height = 512, seed = 12345, octave = 8;
	levels = new float[] { 0., 0.35, 0.4, 0.45, 0.5, 0.6, 0.70, 0.8, 1. };

	palette = new Color[]{DARKBLUE, BLUE, YELLOW, GREEN, DARKGREEN, GRAY, BLACK};

	heightMap = new double[width * height];
}

void TerrainGenerator::DrawInterface()
{
	Rectangle position;
	for (int i = 1; i < 8; i++) // 8 - т.к. 9 уровней -1
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

	for (int i = 0; i < 8; i++)
	{
		position = { levels[i]*240.f + 500.f, 350.f, (levels[i + 1] - levels[i]) *240.f, 30.f};
		DrawText(TextFormat("%.2f", levels[i]), position.x, position.y - 15, 14, palette[i]);
		DrawRectangleRec(position, palette[i]);
	}

}


TerrainGenerator::~TerrainGenerator()
{
	delete[] levels;
	delete[] heightMap;
	delete[] palette;
}