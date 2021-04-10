#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator()
{
	//levels = new float [0., 0.25, 0.75, 1. ];
}

void TerrainGenerator::DrawInterface()
{
	Rectangle position = { 600, 40, 120, 20 };

	for (int i = 1; i < sizeof(levels)/sizeof(levels[0]) - 1; i++) // 6 - т.к. 7 уровней -1
	{
		position.y += (i - 1) * 30;
		levels[i] = GuiSliderBar(
						position,
						TextFormat("%f", levels[i-1]),
						TextFormat("%f", levels[i+1]),
						levels[i],
						levels[i - 1],
						levels[i+1]
						);
	}


	//lakeLevel = GuiSliderBar(
	//	Rectangle{ 600, 40, 120, 20 },
	//	TextFormat("%f", 0.),
	//	TextFormat("%f", mountainLevel),
	//	lakeLevel,
	//	0.,
	//	mountainLevel);

	//mountainLevel = GuiSliderBar(
	//	Rectangle{ 600, 70, 120, 20},
	//	TextFormat("%f", lakeLevel),
	//	TextFormat("%f", 1.),
	//	mountainLevel,
	//	lakeLevel,
	//	1.);
}


TerrainGenerator::~TerrainGenerator()
{
	//delete levels;
}