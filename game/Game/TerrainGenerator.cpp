#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator()
{
	width = 512, height = 512, seed = 12345, octave = 8;
	levels = new float[] { 0., 0.35, 0.4, 0.45, 0.5, 0.6, 0.70, 0.8, 1. };

	heightMap = new double[width * height];
}

void TerrainGenerator::DrawInterface()
{
	for (int i = 1; i < 8; i++) // 8 - т.к. 9 уровней -1
	{
		Rectangle position = { 600, 40 + (i - 1) * 30, 120, 20 };
		levels[i] = GuiSliderBar(
						position,
						TextFormat("%f", levels[i-1]),
						TextFormat("%f", levels[i+1]),
						levels[i],
						levels[i - 1],
						levels[i+1]
						);
	}
}


TerrainGenerator::~TerrainGenerator()
{
	delete[] levels;
	delete[] heightMap;
}